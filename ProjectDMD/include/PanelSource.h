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
#include "Transition.h"


////////////////////////////////////////////////////////////////////////////////
// PanelSource

class PanelSource
{
public:
    virtual void Update() = 0;
    virtual ~PanelSource() = 0;
    void AddTransition(Transition* transition);

protected:
    std::vector<Transition*> _activeTransitions;
    virtual void TransitionCallback(Transition* transition, int framesRemaining) = 0;

};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Time

class STime : public PanelSource
{
public:
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

    STime(
        std::tuple<int, int>            origin,
        STime::TimeFormat               timeFormat = STime::TimeFormat::HH_MM_12_PERIOD,
        Render::TextJustification       justification = Render::TextJustification::Center,
        int                             horizontalTextSpacing = 0,
        const rgb_matrix::Color*        timeColor = ResourceManager::GetSystemColorPalette()->GetColor(15),
        const DMDF*                     timeFont = ResourceManager::GetSystemFont());

    ~STime();

    void Update();
    virtual void TransitionCallback(Transition* transition, int framesRemaining);

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

