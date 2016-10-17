#pragma once

#include "common.h"

namespace CSGO {
	class IVEngine;

	IVEngine* GetEngine();

	class IVEngine
	{
	public:
		virtual void vm00();
		virtual void vm01();
		virtual void vm02();
		virtual void vm03();
		virtual void vm04();
		virtual void GetScreenSize(int& width, int& height);
		virtual void vm06();
		virtual void ClientCmd(const char *szCmdString);
		virtual void vm08();
		virtual void vm09();
		virtual void vm10();
		virtual void vm11();
		virtual int GetLocalPlayer();

		void ExecuteClientCmd(const char *szCmdString);
	};

}