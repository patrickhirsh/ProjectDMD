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

// Linux Only
#if __linux__
#include <getopt.h>
#include <unistd.h>
#include <dirent.h>
#endif

// All Platforms
#include <signal.h>

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"
#include "PanelSource.h"
#include "Render.h"
#include "ResourceManager.h"


class PanelManager
{
    public:
        static int Run(int argc, char *argv[]);
};