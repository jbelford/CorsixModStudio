/*
** Corsix's Mod Studio — Rainman library precompiled header
**
** Precompile headers included by the majority of rainman translation units
** to reduce redundant parsing. Lua headers are intentionally excluded because
** different TUs use different (conflicting) Lua versions.
*/
#pragma once

// Standard library
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

// Core rainman headers (included by 28-31 of ~40 TUs)
#include "rainman/core/Exception.h"
#include "rainman/core/memdebug.h"
#include "rainman/core/RainmanLog.h"
