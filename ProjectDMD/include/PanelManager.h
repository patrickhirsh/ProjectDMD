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
#include "PanelMode.h"
#include "PanelSource.h"
#include "Render.h"
#include "ResourceManager.h"


////////////////////////////////////////////////////////////////////////////////
// PanelManager

/* Responsible for updating the panel as well as listening for signals to change
mode / shut down the panel. Manages all PanelModes and initializes all systems
upon startup. */
class PanelManager
{
public:

    /* Master Panel Mode List */
    enum Mode
    {
		PanelTest,
        BasicClock
    };

    /* initialize all systems, run startup, and enter the main update loop. */
    static int Run(int argc, char *argv[]);

private:
    static Mode _currentMode;
    static std::map<Mode, PanelMode*> _panelModes;
    static void initializeSystems(int argc, char *argv[]);
    static void initializeModes();
};

////////////////////////////////////////////////////////////////////////////////