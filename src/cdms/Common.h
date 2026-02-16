#pragma once

#include "Construct.h"
#include "Utility.h"
#include "strconv.h"
#include "strings.h"
#include <wx/wxprec.h>

// Debug memory tracking disabled — #define new conflicts with C++17 headers
#ifdef _DEBUG
#include <crtdbg.h>
#endif
