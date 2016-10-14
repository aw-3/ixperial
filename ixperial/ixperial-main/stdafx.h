// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <lua.hpp>
#include <LuaBridge.h>

#include "baseclient.h"
#include "netvar-manager.h"
#include "vmt-hook.h"
#include "lua-api.h"
#include "ixperial-main.h"