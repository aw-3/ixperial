#include "stdafx.h"

typedef void(__stdcall *PaintTraverseFn)(void* vguiPanel, bool forceRepaint, bool allowForce);
typedef void(__fastcall *CreateMoveFn)(void* _this, void* edx, float frameTime, void* ucmd);
//typedef void(__fastcall *CHLCreateMoveFn)(void* _this, void* edx, int sequence, float frameTime, bool active);
//typedef void(__fastcall *FrameStageNotifyFn)(void* _this, void* edx, clientStageType stage);
typedef void(__fastcall *OverrideViewFn)(void* _this, void* edx, void* pSetup);
//typedef void(__fastcall *DrawModelExecuteFn)(void* _this, void* edx, void *ctx, void* state, const ModelRenderInfo_t &pInfo, void *pCustomBoneToWorld);

CreateMoveFn orCreateMove;
void __fastcall hkCreateMove(void* _this, void* edx, float frameTime, CUserCmd* ucmd);

PaintTraverseFn orPaintTraverse;
void __stdcall hkPaintTraverse(void* vgui, bool forceRepaint, bool allowForce);

void* CSGO::GetInterface(std::string dll, std::string name)
{
	CreateInterfaceFn factory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(dll.data()), "CreateInterface");
	return factory(name.data(), 0); // Paying chinese factory workers minimum wage since 2012
}

void CSGO::LoadEngineHooks()
{
	void* clientModeVt = (void*)(*(int**)GetClient())[10]; // 10th method in Client VT
	clientModeVt = (void*)**(int**)((char*)clientModeVt + 5); // hacky way of getting clientmode VT from a method in client vt

	VMTHook::Hook("ClientMode", clientModeVt);
	VMTHook::Hook("IPanel", CSGO::GetVGUI());

	orCreateMove = (CreateMoveFn)VMTHook::WriteVMTMethod("ClientMode", 24, hkCreateMove);


	VMTHook::EnableHook("ClientMode");
	VMTHook::EnableHook("IPanel");
}

void CSGO::UnloadEngineHooks()
{
	VMTHook::DisableHook("ClientMode");
}

void __fastcall hkCreateMove(void* _this, void* edx, float frameTime, CUserCmd* ucmd)
{
	orCreateMove(_this, edx, frameTime, ucmd);

	// Invoke lua event
	lua_getglobal(luacs->L, "invokeEvent");
	lua_pushstring(luacs->L, "createmove");
	luabridge::push(luacs->L, RefCountedPtr<LuaUCmd>(new LuaUCmd(ucmd)));
	lua_pcall(luacs->L, 2, 0, 0);

	// update vcmd
	for (int i = 0; i < 150; i++) {
		CUserCmd *curr = &CSGO::GetInput()->m_pCommands[i];

		if (ucmd == curr) {
			CVerifiedUserCmd *vcmd = &CSGO::GetInput()->m_pVerifiedCommands[i];
			vcmd->m_cmd = *ucmd;
			vcmd->m_crc = ucmd->GetChecksum();
			break;
		}
	}
}

void __stdcall hkPaintTraverse(void* vgui, bool forceRepaint, bool allowForce)
{
	orPaintTraverse(vgui, forceRepaint, allowForce);

	// Invoke lua event
	lua_getglobal(luacs->L, "invokeEvent");
	lua_pushstring(luacs->L, "painttraverse");
	lua_pcall(luacs->L, 1, 0, 0);
}