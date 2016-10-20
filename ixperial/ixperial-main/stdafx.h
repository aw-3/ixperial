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

// Luabridge RefCountedPtr uses hash_map which is deprecated
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include <lua.hpp>
#include <LuaBridge.h>
#include <RefCountedPtr.h>

#include "utility.h"
#include "common.h"
#include "base-engine.h"
#include "base-client.h"
#include "base-input.h"
#include "base-vgui.h"
#include "netvar-manager.h"
#include "SignatureScanner.h"
#include "vmt-hook.h"
#include "lua-api.h"
#include "ixperial-main.h"