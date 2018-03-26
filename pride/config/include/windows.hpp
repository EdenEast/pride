
#pragma once

#include "../detection/os.hpp"

#if defined(PRI_OS_WINDOWS)
#ifndef NOMINMAX
#define NOMINMAX //prevent windows redefining min/max
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif