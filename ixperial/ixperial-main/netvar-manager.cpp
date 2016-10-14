#include "stdafx.h"

Netvars *g_pVars = nullptr;

Netvars::Netvars(void)
{
	m_tables.clear();

	CSGO::ClientClass *pClientClasses = CSGO::GetClient()->GetAllClasses();
	if (!pClientClasses)
		return;

	while (pClientClasses)
	{
		RecvTable* recvTable = pClientClasses->m_pRecvTable;
		m_tables.push_back(recvTable);
		pClientClasses = pClientClasses->m_pNext;
	}
}

Netvars::~Netvars()
{
	m_tables.clear();
}

// calls GetProperty wrapper to get the absolute offset of the prop
int Netvars::GetOffset(const char *tableName, const char *propName)
{
	int offset = GetProperty(tableName, propName);
	if (!offset)
	{
		return 0;
	}
	return offset;
}


// calls GetProperty wrapper to get prop and sets the proxy of the prop
bool Netvars::HookProp(const char *tableName, const char *propName, RecvVarProxyFn function)
{
	RecvProp *recvProp = 0;
	GetProperty(tableName, propName, &recvProp);
	if (!recvProp)
		return false;

	recvProp->SetProxyFn(function);

	return true;
}


// wrapper so we can use recursion without too much performance loss
int Netvars::GetProperty(const char *tableName, const char *propName, RecvProp **prop)
{
	RecvTable *recvTable = GetTable(tableName);
	if (!recvTable)
		return 0;

	int offset = GetProperty(recvTable, propName, prop);
	if (!offset)
		return 0;

	return offset;
}

// uses recursion to return a the relative offset to the given prop and sets the prop param
int Netvars::GetProperty(RecvTable *recvTable, const char *propName, RecvProp **prop)
{
	int extraOffset = 0;
	for (int i = 0; i < recvTable->m_nProps; ++i)
	{
		RecvProp *recvProp = &recvTable->m_pProps[i];
		RecvTable *child = recvProp->GetDataTable();

		if (child && (child->m_nProps > 0))
		{
			int tmp = GetProperty(child, propName, prop);
			if (tmp)
				extraOffset += (recvProp->GetOffset() + tmp);
		}

		if (stricmp(recvProp->m_pVarName, propName))
			continue;

		if (prop)
			*prop = recvProp;

		return (recvProp->GetOffset() + extraOffset);
	}

	return extraOffset;
}

RecvTable *Netvars::GetTable(const char *tableName)
{
	if (m_tables.empty())
		return 0;

	for each (RecvTable *table in m_tables)
	{
		if (!table)
			continue;

		if (stricmp(table->m_pNetTableName, tableName) == 0)
			return table;
	}

	return 0;
}


