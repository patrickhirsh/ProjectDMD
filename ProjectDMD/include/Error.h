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
#include <sstream>
#include <vector>

// Local
#include "../SYSTEMGLOBALS.h"
#include "ResourceManager.h"
#include "Render.h"


////////////////////////////////////////////////////////////////////////////////
// Error

/* short error number used to identify the origin of an error. */
enum ErrorNum
{
    // FATAL (0-499)
    FATAL_INVALID_MATRIX = 0,
    FATAL_INVALID_BUFFER = 1,
    FATAL_DIR_READ_ERROR = 2,
    FATAL_INVALID_RESOURCE_PATH = 3,
    FATAL_RESOURCE_NOT_LOADED = 4,
    FATAL_INVALID_VIRTUAL_WINDOW = 5,

    // ERROR (500-999)
    ERROR_RESOURCE_NOT_FOUND = 500,
    ERROR_INVALID_FONT = 501,

    // WARNING (1000+)
    WARNING_UNSUPPORTED_CHARACTER = 1000,
    WARNING_INVALID_STERN_COLOR_VAL = 1001,
    WARNING_INVALID_RENDER_GLOBAL = 1002,
    WARNING_INVALID_RENDER_PARAM = 1003
};

/* handles logging of errors, warnings, and fatal errors. */
class ErrorHandler
{
public:
    /* log an error originating from "source" with "reason" and "errorNumber".
    Fatal errors halt the program and display on-screen. */
    static void					Log(std::string source, std::string reason, ErrorNum errorNumber);

    /* error / warning stats. */
    static int						GetErrorCount() { return _errorCount; }
    static int						GetWarningCount() { return _warningCount; }

private:
    static int						_errorCount;
    static int						_warningCount;
};

////////////////////////////////////////////////////////////////////////////////