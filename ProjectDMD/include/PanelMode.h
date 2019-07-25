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
#include <list>
#include <locale>
#include <map>
#include <vector>

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"
#include "PanelSource.h"


////////////////////////////////////////////////////////////////////////////////
// PanelMode

class PanelMode
{
public:
    void Update();
    virtual ~PanelMode() = 0;

protected:
    std::list<PanelSource*> _activeSources;    
    virtual void PanelSourceCallback(PanelSource* source, int framesRemaining) = 0;
    virtual void internalUpdate() = 0;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// MODE: Clock

class MClock : public PanelMode
{
public:
    MClock(
        std::tuple<int, int>            origin,
        STime::TimeFormat               timeFormat = STime::TimeFormat::HH_MM_12_PERIOD,
        Render::TextJustification       justification = Render::TextJustification::Center,
        int                             horizontalTextSpacing = 1,
        const rgb_matrix::Color*        timeColor = ResourceManager::GetSystemColorPalette()->GetColor(15),
        const DMDF*                     timeFont = ResourceManager::GetSystemFont());

    virtual ~MClock();

protected:
    virtual void PanelSourceCallback(PanelSource* source, int framesRemaining);
    virtual void internalUpdate();
};

