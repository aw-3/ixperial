#pragma once

#include "common.h"

namespace CSGO {
	class Client;
	class ClientClass;

	Client* GetClient();
	void* GetClientHandle();

	class Client
	{
	public:
		virtual void vm00();

		virtual void vm01();
		virtual void vm02();

		virtual void vm03();
		virtual void vm04();

		virtual void vm05();
		virtual void vm06();
		virtual void vm07();

		virtual ClientClass				*GetAllClasses(void);
	};

	class ClientClass
	{
	public:
		void*			m_pCreateFn;
		void*			m_pCreateEventFn;	// Only called for event objects.
		const char				*m_pNetworkName;
		RecvTable				*m_pRecvTable;
		ClientClass				*m_pNext;
		int						m_ClassID;	// Managed by the engine.
	};

}

