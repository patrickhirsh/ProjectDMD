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
#include "Error.h"
#include "Render.h"
#include "ResourceManager.h"


////////////////////////////////////////////////////////////////////////////////
// PanelSource

class PanelSource
{
    public:
        virtual void Reset() = 0;
        virtual void Update() = 0;   
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Clock

class Clock : public PanelSource
{
				public:
								Clock();
								~Clock();
								void Reset();
								void Update();

    private:
        Font font;
        Color color;
        const char* time_format;
        char text_buffer[256];
        struct tm tm;
        time_t* t;
};

////////////////////////////////////////////////////////////////////////////////

