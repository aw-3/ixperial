#pragma once

#include "checksum.h"

class CUserCmd
{
public:
	virtual ~CUserCmd();//0x00
	int		command_number;//0x04
	int		tick_count;//0x08
	float	viewangles[3];//0x0C
	float	aimdirection[3];
	float	forwardmove;
	float	sidemove;
	float	upmove;
	int		buttons; //0x30
	char    impulse; //0x35
	int		weaponselect;//0x36	
	int		weaponsubtype;//0x3A
	int		random_seed; //0x3E
	short	mousedx;//0x42		
	short	mousedy;//0x43		
	bool	hasbeenpredicted;
	float	headangles[3];
	float	headoffset[3];

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_ProcessBuffer(&crc, &hasbeenpredicted, sizeof(hasbeenpredicted));
		CRC32_ProcessBuffer(&crc, &headangles, sizeof(headangles));
		CRC32_ProcessBuffer(&crc, &headoffset, sizeof(headoffset));
		CRC32_Final(&crc);

		return crc;
	}
};

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};


class LuaUCmd
{
public:
	LuaUCmd(CUserCmd* cmd) : pCmd(cmd) {};

	CUserCmd *pCmd;

public:
	int GetButtons() const;

	void SetButtons(const int&);
};