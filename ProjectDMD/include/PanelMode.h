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

/* a PanelMode represents a managed collection of PanelSources. A PanelMode
should do all the organizing, updating, and positioning of these sources to
achieve an independent "mode" that can be switched to by the PanelManager. */
class PanelMode
{
public:
    /* external "Update" called by the PanelManager */
    void Update();

    /* virtual destructor */
    virtual ~PanelMode() = 0;

protected:
    /* active sources used in this mode */
    std::vector<PanelSource*> _sources;

    /* internal mode update called each frame prior to updating this mode's sources. */
    virtual void internalUpdate() = 0;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// MODE: Clock

/* Simple clock mode... Not fully fleshed out yet. */
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
    virtual void internalUpdate();
};

