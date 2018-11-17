#ifndef _INCLUDE_EXTENSION_H_
#define _INCLUDE_EXTENSION_H_


#include <cinttypes>

#include <memory>

#include <map>
#include <vector>


#include "smsdk_ext.h"


#include <icvar.h>
#include <ehandle.h>
#include <itoolentity.h>
#include <ICollideable.h>
#include <dt_send.h>
//#include <tier1/utlvector.h>

//#define DECLARE_PREDICTABLE()
//#include <collisionproperty.h>

#include <ISDKHooks.h>


/* globals */
extern CGlobalVars *gpGlobals;
extern CBaseEntityList *g_pEntityList;

/* Source interfaces */
extern ICvar *icvar;
extern IServerTools *servertools;

/* SourceMod interfaces */
extern IGameConfig *g_pGameConf;

/* SourceMod extension interfaces */
extern ISDKHooks *sdkhooks;

/* console variables */
extern ConVar cv_pathfollower_debug;
extern ConVar cv_pathfollower_stubs;


class ExtPathFollower : public SDKExtension, public IConCommandBaseAccessor, public IPluginsListener, public ISMEntityListener
{
public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlen	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late);
	
	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload();

	/**
	 * @brief This is called once all known extensions have been loaded.
	 * Note: It is is a good idea to add natives here, if any are provided.
	 */
	virtual void SDK_OnAllLoaded();

	/**
	 * @brief Called when the pause state is changed.
	 */
	//virtual void SDK_OnPauseChange(bool paused);

	/**
	 * @brief this is called when Core wants to know if your extension is working.
	 *
	 * @param error		Error message buffer.
	 * @param maxlen	Size of error message buffer.
	 * @return			True if working, false otherwise.
	 */
	virtual bool QueryRunning(char *error, size_t maxlen);
public:
#if defined SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late);

	/**
	 * @brief Called when Metamod is detaching, after the extension version is called.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodUnload(char *error, size_t maxlen);

	/**
	 * @brief Called when Metamod's pause state is changing.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param paused		Pause state being set.
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlen);
#endif
	
	
	virtual bool RegisterConCommandBase(ConCommandBase *pCommand) override;
	
	
	virtual void OnPluginUnloaded(IPlugin *plugin) override;
	virtual void OnPluginDestroyed(IPlugin *plugin) override;
	
	
	virtual void OnEntityDestroyed(CBaseEntity *pEntity) override;
};

#endif // _INCLUDE_EXTENSION_H_
