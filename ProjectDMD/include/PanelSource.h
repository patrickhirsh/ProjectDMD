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
#include <list>
#include <map>
#include <vector>

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"
#include "Render.h"
#include "ResourceManager.h"
#include "Modifier.h"


////////////////////////////////////////////////////////////////////////////////
// PanelSource

/* a PanelSource represents an "object" that can be drawn to the panel.
PanelSources handle triggering render calls to update these objects as
well as react to modifiers applied to them. 

To keep iterations down, modifiers need to be applied DURING all render 
calls. For this reason, it is the responsibility of each source to apply 
these modifiers to each render call. */
class PanelSource
{
public:
    /* applies a modifier to this source. */
    void AddModifier(Modifier* modifier);

    /* called once per frame. */
    virtual void Update() = 0;
    
    /* virtual destructor. */
    virtual ~PanelSource();

protected:
    /* while PanelMode *does* create theses modifiers and mutate this modifiers 
    vector, if this PanelSource is destroyed, this PanelSource is responsible 
    for cleaning up its remaining _activeModifiers. */
    std::vector<Modifier*> _activeModifiers;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Time

/* a simple clock source used to display the current time. */
class STime : public PanelSource
{
public:

    /* specifies the time format of this clock. "Period" indicates the displaying
    of AM/PM, and "Period_Dot" indicates the use of a dot to represent "PM" instead
    of displaying the period in plain text. HH = hours. MM = minutes. SS = seconds.
    12 = 12 hour time. 24 = 24 hour time. */
    enum TimeFormat
    {
        HH_MM_12,
        HH_MM_SS_12,
        HH_MM_12_PERIOD,
        HH_MM_SS_12_PERIOD,
        HH_MM_12_PERIOD_DOT,
        HH_MM_SS_12_PERIOD_DOT,
        HH_MM_24,
        HH_MM_SS_24
    };

    /* create a clock source at "origin" (a point fixed at the upper left/right/center-most
    point in the source object, depending on "justification") in "timeFormat" using "timeFont"
    and renderd in "timeColor" with characters spaced every "horizontalTextSpacing" pixels. */
    STime(
        std::tuple<int, int>            origin,
        STime::TimeFormat               timeFormat = STime::TimeFormat::HH_MM_12_PERIOD,
        Render::TextJustification       justification = Render::TextJustification::Center,
        int                             horizontalTextSpacing = 0,
        const rgb_matrix::Color*        timeColor = ResourceManager::GetSystemColorPalette()->GetColor(15),
        const DMDF*                     timeFont = ResourceManager::GetSystemFont());

    ~STime();

    void Update();

private:
    time_t*                             _t;
    struct tm                           _tm;
    char                                _currentTime[256];
    std::tuple<int, int>                _origin;
    TimeFormat                          _format;
    Render::TextJustification           _justification;
    int                                 _horizontalTextSpacing;
    const rgb_matrix::Color*            _color;
    const DMDF*                         _font;
    const char*                         _formatInternal;
    bool                                _renderPeriodDot;
    
    void updateCurrentTime();
};

////////////////////////////////////////////////////////////////////////////////

