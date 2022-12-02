#ifndef _INCLUDE_FIND_STUFF_H_
#define _INCLUDE_FIND_STUFF_H_


#include "extension.h"


/* multiply instantiated templates + no symbols = eeeeeeeevil */

/* basically, it's impossible to find Windows Path::Compute<IPathCost> with
 * regular SM pattern matching, so what we do here is find a known function that
 * calls both versions of Path::Compute<IPathCost>, and then look for known
 * chunks of that function where it does the calls, and extract the call target
 * from that */


inline uintptr_t Find_Path_Compute_IPathCost_goal()
{
	uintptr_t addr_ChasePath_RefreshPath;
	if (!g_pGameConf->GetMemSig("ChasePath::RefreshPath", reinterpret_cast<void **>(&addr_ChasePath_RefreshPath)) || (void *)addr_ChasePath_RefreshPath == nullptr) {
		g_pSM->LogError(myself, "Failed to locate memory address based on signature for \"ChasePath::RefreshPath\"!");
		return 0;
	}
	
	static const uint8_t seek[] = {
		0x8b, 0x07,                         // +0000  mov eax,[edi]
		0x8b, 0xcf,                         // +0002  mov edx,edi
		0x6a, 0x01,                         // +0004  push 1
		0x8b, 0x80, 0x84, 0x00, 0x00, 0x00, // +0006  mov eax,[eax+0x84]
		0xff, 0xd0,                         // +000C  call eax
		0x51,                               // +000E  push ecx
		0xd9, 0x1c, 0x24,                   // +000F  fstp dword ptr [esp]
		0xff, 0x75, 0x10,                   // +0012  push [ebp+0x10]
		0x8d, 0x45, 0xdc,                   // +0015  lea eax,[ebp-0x24]
		0x8b, 0xcf,                         // +0018  mov ecx,edi
		0x50,                               // +001A  push eax
		0x53,                               // +001B  push ebx
		0xe8,                               // +001C  call Path::Compute<IPathCost> [goal]
	};
	
	std::vector<ptrdiff_t> matches;
	
	for (ptrdiff_t off = 0x000; off < 0x600; ++off) {
		auto ptr1 = reinterpret_cast<const void *>(addr_ChasePath_RefreshPath + off);
		auto ptr2 = reinterpret_cast<const void *>(seek);
		
		if (memcmp(ptr1, ptr2, sizeof(seek)) == 0) {
			matches.push_back(off);
		}
	}
	
	if (matches.size() != 1) {
		g_pSM->LogError(myself, "Found %u matches for Path::Compute<IPathCost> [goal]!", matches.size());
		return 0;
	}
	
	uintptr_t addr_call = addr_ChasePath_RefreshPath + matches.front() + 0x1c;
	
	uint32_t tgt_rel = *reinterpret_cast<uint32_t *>(addr_call + 1);
	uint32_t tgt_abs = (addr_call + 5) + tgt_rel;
	
	g_pSM->LogMessage(myself, "Found \"Path::Compute<IPathCost> [goal]\" @ 0x%08x", tgt_abs);
	
	return tgt_abs;
}


inline uintptr_t Find_Path_Compute_IPathCost_subject()
{
	uintptr_t addr_ChasePath_RefreshPath;
	if (!g_pGameConf->GetMemSig("ChasePath::RefreshPath", reinterpret_cast<void **>(&addr_ChasePath_RefreshPath)) || (void *)addr_ChasePath_RefreshPath == nullptr) {
		g_pSM->LogError(myself, "Failed to locate memory address based on signature for \"ChasePath::RefreshPath\"!");
		return 0;
	}
	
	static const uint8_t seek[] = {
		0x8b, 0x07,                         // +0000  mov eax,[edi]
		0x8b, 0xcf,                         // +0002  mov ecx,edi
		0x6a, 0x01,                         // +0004  push 1
		0x8b, 0x80, 0x84, 0x00, 0x00, 0x00, // +0006  mov eax,[eax+0x84]
		0xff, 0xd0,                         // +000C  call eax
		0x8b, 0x06,                         // +000E  mov eax,[esi]
		0x51,                               // +0010  push ecx
		0xd9, 0x1c, 0x24,                   // +0011  fstp dword ptr [esp]
		0xff, 0x75, 0x10,                   // +0014  push [ebp+0x10]
		0x8b, 0xce,                         // +0017  mov ecx,esi
		0xff, 0x90, 0x24, 0x01, 0x00, 0x00, // +0019  call dword ptr [eax+0x11c]
		0x50,                               // +001F  push eax
		0x53,                               // +0020  push ebx
		0x8b, 0xcf,                         // +0021  mov ecx,edi
		0xe8,                               // +0023  call Path::Compute<IPathCost> [subject]
	};
	
	std::vector<ptrdiff_t> matches;
	
	for (ptrdiff_t off = 0x000; off < 0x600; ++off) {
		auto ptr1 = reinterpret_cast<const void *>(addr_ChasePath_RefreshPath + off);
		auto ptr2 = reinterpret_cast<const void *>(seek);
		
		if (memcmp(ptr1, ptr2, sizeof(seek)) == 0) {
			matches.push_back(off);
		}
	}
	
	if (matches.size() != 1) {
		g_pSM->LogError(myself, "Found %u matches for Path::Compute<IPathCost> [subject]!", matches.size());
		return 0;
	}
	
	uintptr_t addr_call = addr_ChasePath_RefreshPath + matches.front() + 0x23;
	
	uint32_t tgt_rel = *reinterpret_cast<uint32_t *>(addr_call + 1);
	uint32_t tgt_abs = (addr_call + 5) + tgt_rel;
	
	g_pSM->LogMessage(myself, "Found \"Path::Compute<IPathCost> [subject]\" @ 0x%08x", tgt_abs);
	
	return tgt_abs;
}


/* 'friend' hack to get access to private ConCommand::m_fnCommandCallback */
class CCvar
{
public:
	static FnCommandCallback_t GetConCommandCallback(const ConCommand *pConCommand)
	{
		return pConCommand->m_fnCommandCallback;
	}
};

inline uintptr_t Find_TheNavMesh()
{
	const ConCommand *nav_unmark = g_pCVar->FindCommand("nav_unmark");
	if (nav_unmark == nullptr || !nav_unmark->IsCommand()) {
		g_pSM->LogError(myself, "Failed to locate concommand \"nav_unmark\"!");
		return 0;
	}
	
	static const uint8_t seek[] = {
		0xe8, 0x00, 0x00, 0x00, 0x00,       // +0000  call UTIL_IsCommandIssuedByServerAdmin
		0x84, 0xc0,                         // +0005  test al,al
		0x74, 0x0b,                         // +0007  jz +0x0b
		0x8b, 0x0d, 0x00, 0x00, 0x00, 0x00, // +0009  mov ecx,TheNavMesh
		0xe9, 0x00, 0x00, 0x00, 0x00,       // +000F  jmp CNavMesh::CommandNavUnmark
		0xc3,                               // +0x14  ret
	};
	static const uint8_t mask[] = {
		0xff, 0x00, 0x00, 0x00, 0x00,       // +0000  call UTIL_IsCommandIssuedByServerAdmin
		0xff, 0xff,                         // +0005  test al,al
		0xff, 0xff,                         // +0007  jz +0x0b
		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, // +0009  mov ecx,TheNavMesh
		0xff, 0x00, 0x00, 0x00, 0x00,       // +000F  jmp CNavMesh::CommandNavUnmark
		0xff,                               // +0x14  ret
	};
	
	auto fn = reinterpret_cast<uint8_t *>(CCvar::GetConCommandCallback(nav_unmark));
	
	bool fail = false;
	for (size_t i = 0; i < sizeof(seek); ++i) {
		if ((fn[i] & mask[i]) != (seek[i] & mask[i])) {
			g_pSM->LogError(myself, "Mismatch at offset +0x%02x!", i);
			fail = true;
		}
	}
	if (fail) return 0;
	
	uint32_t addr = **reinterpret_cast<uint32_t **>(fn + 0x09 + 2);
	
	g_pSM->LogMessage(myself, "Found \"TheNavMesh\" @ 0x%08x", addr);
	
	return addr;
}

//%d overlapping areas selected\n
inline uintptr_t Find_TheNavAreas()
{
	const ConCommand *nav_check_floor = g_pCVar->FindCommand("nav_check_floor");
	if (nav_check_floor == nullptr || !nav_check_floor->IsCommand()) {
		g_pSM->LogError(myself, "Failed to locate concommand \"nav_check_floor\"!");
		return 0;
	}
	
	static const uint8_t seek[] = {
		0xe8, 0x00, 0x00, 0x00, 0x00,       // +0000  call UTIL_IsCommandIssuedByServerAdmin
		0x84, 0xc0,                         // +0005  test al,al
		0x74, 0x0b,                         // +0007  jz +0x0b
		0x8b, 0x0d, 0x00, 0x00, 0x00, 0x00, // +0009  mov ecx,TheNavMesh
		0xe9, 0x00, 0x00, 0x00, 0x00,       // +000F  jmp CNavMesh::CommandNavUnmark
		0xc3,                               // +0x14  ret
	};
	static const uint8_t mask[] = {
		0xff, 0x00, 0x00, 0x00, 0x00,       // +0000  call UTIL_IsCommandIssuedByServerAdmin
		0xff, 0xff,                         // +0005  test al,al
		0xff, 0xff,                         // +0007  jz +0x0b
		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, // +0009  mov ecx,TheNavMesh
		0xff, 0x00, 0x00, 0x00, 0x00,       // +000F  jmp CNavMesh::CommandNavUnmark
		0xff,                               // +0x14  ret
	};
	
	auto fn = reinterpret_cast<uint8_t *>(CCvar::GetConCommandCallback(nav_check_floor));
	
	bool fail = false;
	for (size_t i = 0; i < sizeof(seek); ++i) {
		if ((fn[i] & mask[i]) != (seek[i] & mask[i])) {
			g_pSM->LogError(myself, "Mismatch at offset +0x%02x!", i);
			fail = true;
		}
	}
	if (fail) return 0;
	
	uint32_t addr = **reinterpret_cast<uint32_t **>(fn + 0x09 + 2);
	
	g_pSM->LogMessage(myself, "Found \"TheNavAreas\" @ 0x%08x", addr);
	
	return addr;
}

#endif // _INCLUDE_FIND_STUFF_H_
