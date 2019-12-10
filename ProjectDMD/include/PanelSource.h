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
#include <sstream>
#include <vector>

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"
#include "Render.h"
#include "ResourceManager.h"


////////////////////////////////////////////////////////////////////////////////
// PanelSource

class PanelSource
{
public:
    /* Should this source be rendered? */
    bool IsActive = true;

    /* called once per frame. */
    virtual void Update() = 0;
    
    /* virtual destructor. */
    virtual ~PanelSource();
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Generic Sources

class SFill : public PanelSource
{
public:
	SFill							    (rgb_matrix::Color color, float opacity = 1.0f);
	~SFill							    ();
	void Update						    ();
	void SetColor					    (rgb_matrix::Color color, float opacity = 1.0f);

private:
	rgb_matrix::Color*				    _color;
	float       					    _opacity;
};

class SRectangle : public PanelSource
{
public:
    SRectangle(
        std::tuple<int, int>			origin1,
        std::tuple<int, int>			origin2,                                   
        const rgb_matrix::Color*        color,
        float                           opactiy = 1.0f,
        bool                            fill = true,
        int                             borderWidth = 1);
    
    ~SRectangle                         () { return; }
    void Update                         ();
    void Translate                      (std::tuple<int, int> offset);
    void SetColor                       (const rgb_matrix::Color* color, float opacity = 1.0f);
    void SetOrigin1                     (std::tuple<int, int> origin1) { _origin1 = origin1; }
    void SetOrigin2                     (std::tuple<int, int> origin2) { _origin2 = origin2; }

private:
    std::tuple<int, int>                _origin1;
    std::tuple<int, int>                _origin2;
    const rgb_matrix::Color*            _color;
    float                               _opacity;
    bool                                _fill;
    int                                 _borderWidth;
};

class SText : public PanelSource
{
public:
    SText(
        std::string                     text,
        std::tuple<int, int>            origin,
        const DMDF*                     font = ResourceManager::GetSystemFont(),
        Render::TextJustification       justification = Render::TextJustification::Center,
        const rgb_matrix::Color*        color = ResourceManager::GetSystemColorPalette()->GetColor(15),
        float                           opacity = 1.0f,
        int                             horizontalTextSpacing = 0,
        int                             verticalTextSpacing = 2,
        bool                            wrapOnNewline = true);

    ~SText                              () { return; }
    void Update                         ();
    void SetText                        (std::string text) { _text = text;}
    void SetColor                       (const rgb_matrix::Color* color, float opacity = 1.0f);
    void SetOrigin                      (std::tuple<int, int> origin) { _origin = origin; }

private:
    std::string                         _text;
    std::tuple<int, int>                _origin;
    const DMDF*                         _font;
    Render::TextJustification           _justification;
    const rgb_matrix::Color*		    _color;
	float       					    _opacity;
    int                                 _horizontalTextSpacing;
    int                                 _verticalTextSpacing;
    bool                                _wrapOnNewline;
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

    ~STime                              ();
    void Update                         ();
    void SetColor                       (const rgb_matrix::Color* color, float opacity = 1.0f);
    void SetOrigin                      (std::tuple<int, int> origin) { _origin = origin; }

private:
    time_t*                             _t;
    struct tm                           _tm;
    char                                _timeBuffer[256];
    char                                _currentTime[256];
    TimeFormat                          _format;
    std::tuple<int, int>                _origin;
    Render::TextJustification           _justification;
    int                                 _horizontalTextSpacing;
    const rgb_matrix::Color*            _color;
    float                               _opacity;
    const DMDF*                         _font;
    const char*                         _formatInternal;
    bool                                _renderPeriodDot;
    
    void updateCurrentTime();
};

////////////////////////////////////////////////////////////////////////////////

