#include "extension.h"
#include "simulate.h"
#include "thunks.h"
#include "sp_natives.h"


ExtPathFollower g_Ext;
SMEXT_LINK(&g_Ext);


/* globals */
CGlobalVars *gpGlobals         = nullptr;
CBaseEntityList *g_pEntityList = nullptr;

/* Source interfaces */
ICvar *icvar              = nullptr;
IServerTools *servertools = nullptr;

/* SourceMod interfaces */
IGameConfig *g_pGameConf = nullptr;

/* SourceMod extension interfaces */
ISDKHooks *sdkhooks = nullptr;

/* console variables */
ConVar cv_pathfollower_debug("pathfollower_debug", "0", FCVAR_CHEAT,
	"PathFollower: enable debug stuff, such as drawing the path and avoidance overlays");
ConVar cv_pathfollower_stubs("pathfollower_stubs", "0", FCVAR_NONE,
	"PathFollower: spew developer console messages about stubbed-out virtual overrides");


bool ExtPathFollower::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
	sharesys->AddDependency(myself, "sdkhooks.ext", true, true);
	
	g_pEntityList = reinterpret_cast<CBaseEntityList *>(gamehelpers->GetGlobalEntityList());
	
	if (!gameconfs->LoadGameConfigFile("PathFollower", &g_pGameConf, error, maxlen)) {
		return false;
	}
	
	if (!Thunks::Init()) return false;
	
	g_pSM->AddGameFrameHook(&SimulateFrame);
	
	return true;
}

void ExtPathFollower::SDK_OnAllLoaded()
{
	plsys->AddPluginsListener(this);
	
	SM_GET_LATE_IFACE(SDKHOOKS, sdkhooks);
	sdkhooks->AddEntityListener(this);
	
	sharesys->AddNatives(myself, g_Natives);
}

void ExtPathFollower::SDK_OnUnload()
{
	sdkhooks->RemoveEntityListener(this);
	
	plsys->RemovePluginsListener(this);
	
	g_pSM->RemoveGameFrameHook(&SimulateFrame);
	
	gameconfs->CloseGameConfigFile(g_pGameConf);
}

bool ExtPathFollower::QueryRunning(char *error, size_t maxlength)
{
	SM_CHECK_IFACE(SDKHOOKS, sdkhooks);
	
	return true;
}


bool ExtPathFollower::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	gpGlobals = ismm->GetCGlobals();
	
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	g_pCVar = icvar;
	ConVar_Register(0, this);
	
	GET_V_IFACE_CURRENT(GetServerFactory, servertools, IServerTools, VSERVERTOOLS_INTERFACE_VERSION);
	
	return true;
}


bool ExtPathFollower::RegisterConCommandBase(ConCommandBase *pCommand)
{
	META_REGCVAR(pCommand);
	return true;
}


void ExtPathFollower::OnPluginUnloaded(IPlugin *plugin)
{
	int nBotsRemoved = 0;
	
	for (auto it = bots.cbegin(); it != bots.cend(); ) {
		const PFContext& ctx = (*it).second;
		
		if (ctx.GetPlugin() == plugin) {
			it = bots.erase(it);
			++nBotsRemoved;
		} else {
			++it;
		}
	}
	
	if (nBotsRemoved != 0) {
		g_pSM->LogMessage(myself, "Plugin \"%s\" unloaded; removed %d PFs", plugin->GetPublicInfo()->name, nBotsRemoved);
	}
}

void ExtPathFollower::OnPluginDestroyed(IPlugin *plugin)
{
	this->OnPluginUnloaded(plugin);
}


void ExtPathFollower::OnEntityDestroyed(CBaseEntity *pEntity)
{
	int entindex = ENTINDEX(pEntity);
	
	if (bots.find(entindex) != bots.end()) {
		RemoveBot(entindex);
	}
}
