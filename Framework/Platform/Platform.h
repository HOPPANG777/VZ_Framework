#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include "Platform_Windows.h"
#else
#error "Unsupported platform"
#endif