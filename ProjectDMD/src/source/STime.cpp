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
    this->Origin = origin;
    this->_format = timeFormat;
    this->_justification = justification;
    this->_horizontalTextSpacing = horizontalTextSpacing;
    this->_color = timeColor;
    this->_font = timeFont;
    this->_t = (time_t*)malloc(sizeof(time_t));
    this->_renderPeriodDot = false;

    switch (timeFormat)
    {
    case TimeFormat::HH_MM_12_PERIOD_DOT:
        this->_renderPeriodDot = true;
        // intentional fall through
    case TimeFormat::HH_MM_12:
        this->_formatInternal = "%l:%M";
        break;
    case TimeFormat::HH_MM_SS_12_PERIOD_DOT:
        this->_renderPeriodDot = true;
        // intentional fall through
    case TimeFormat::HH_MM_SS_12:
        this->_formatInternal = "%l:%M:%S";
        break;
    case TimeFormat::HH_MM_12_PERIOD:
        this->_formatInternal = "%l:%M %P";
        break;
    case TimeFormat::HH_MM_SS_12_PERIOD:
        this->_formatInternal = "%l:%M:%S %P";
        break;
    case TimeFormat::HH_MM_24:
        this->_formatInternal = "%k:%M";
        break;
    case TimeFormat::HH_MM_SS_24:
        this->_formatInternal = "%k:%M:%S";
        break;
    default:
        this->_formatInternal = "%l:%M";
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
    Render::Text(_currentTime, _font, Origin, _color, 1.0f, _justification, _horizontalTextSpacing);
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
