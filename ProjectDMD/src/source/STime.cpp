////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelSource.h"


////////////////////////////////////////////////////////////////////////////////
// SOURCE: Time

STime::STime(
    std::tuple<int, int>            origin,
    STime::TimeFormat               timeFormat,
    Render::TextJustification       justification,
    int                             horizontalTextSpacing,
    const rgb_matrix::Color*        timeColor,
    const DMDF*                     timeFont
)
{
    _origin = origin;
    _format = timeFormat;
    _justification = justification;
    _horizontalTextSpacing = horizontalTextSpacing;
    _color = timeColor;
    _font = timeFont;
    _t = (time_t*)malloc(sizeof(time_t));
    _renderPeriodDot = false;

    switch (timeFormat)
    {
    case TimeFormat::HH_MM_12_PERIOD_DOT:
        _renderPeriodDot = true;
        // intentional fall through
    case TimeFormat::HH_MM_12:
        _formatInternal = "%l:%M";
        break;
    case TimeFormat::HH_MM_SS_12_PERIOD_DOT:
        _renderPeriodDot = true;
        // intentional fall through
    case TimeFormat::HH_MM_SS_12:
        _formatInternal = "%l:%M:%S";
        break;
    case TimeFormat::HH_MM_12_PERIOD:
        _formatInternal = "%l:%M %P";
        break;
    case TimeFormat::HH_MM_SS_12_PERIOD:
        _formatInternal = "%l:%M:%S %P";
        break;
    case TimeFormat::HH_MM_24:
        _formatInternal = "%k:%M";
        break;
    case TimeFormat::HH_MM_SS_24:
        _formatInternal = "%k:%M:%S";
        break;
    default:
        _formatInternal = "%l:%M";
        break;
    }
}

STime::~STime()
{
    delete _t;
}

void STime::Update()
{
    updateCurrentTime();
    Render::Text(_currentTime, _font, _origin, _color, 1.0f, _justification, _horizontalTextSpacing);
}

void STime::updateCurrentTime()
{
#if __linux__
    // get local time
    time(_t);
    localtime_r(_t, &_tm);
    int rawCount = strftime(_timeBuffer, sizeof(_currentTime), _formatInternal, &_tm);

    // write to _currentTime (ignore first leading ' ' if applicable)
    if (rawCount > 0)
    {
        char* bufferIndex = &_timeBuffer[0];
        if (_timeBuffer[0] == ' ') { bufferIndex++; rawCount--; }
        int i = 0;
        while (i < rawCount)
        {
            _currentTime[i] = *bufferIndex;
            bufferIndex++;
            i++;
        }
        // _timeBuffer out string doesn't include null terminator in rawCount
        _currentTime[i] = '\0';
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
