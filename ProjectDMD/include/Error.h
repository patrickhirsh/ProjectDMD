////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <vector>

// Local
#include "../SYSTEMGLOBALS.h"
#include "ResourceManager.h"
#include "Render.h"

enum ErrorNum
{
    // FATAL (0-499)
    FATAL_INVALID_CANVAS = 0,
    FATAL_DIR_READ_ERROR = 1,
    FATAL_INVALID_RESOURCE_PATH = 2,
    FATAL_RESOURCE_NOT_LOADED = 3,

    // ERROR (500-999)
    ERROR_RESOURCE_NOT_FOUND = 500,
    ERROR_INVALID_FONT = 501,

    // WARNING (1000+)
    WARNING_UNSUPPORTED_CHARACTER = 1000,
    WARNING_INVALID_STERN_COLOR_VAL = 1001,
};

class ErrorHandler
{
public:
    static void					Log(std::string source, std::string reason, ErrorNum errorNumber);
    static int						GetErrorCount() { return _errorCount; }
    static int						GetWarningCount() { return _warningCount; }

private:
    static int						_errorCount;
    static int						_warningCount;
};