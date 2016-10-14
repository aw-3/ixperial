#pragma once

class Netvars
{
public:
	Netvars();
	~Netvars();

	int GetOffset(const char *tableName, const char *propName);
	bool HookProp(const char *tableName, const char *propName, RecvVarProxyFn function);

private:
	int GetProperty(const char *tableName, const char *propName, RecvProp **prop = 0);
	int GetProperty(RecvTable *recvTable, const char *propName, RecvProp **prop = 0);
	RecvTable *GetTable(const char *tableName);

	std::vector<RecvTable*>    m_tables;
};

extern Netvars *g_pVars;