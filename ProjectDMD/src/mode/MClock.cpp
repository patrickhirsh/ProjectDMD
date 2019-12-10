////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelMode.h"


////////////////////////////////////////////////////////////////////////////////
// MODE: Clock

MClock::MClock(
    std::tuple<int, int>            origin,
    STime::TimeFormat               timeFormat,
    Render::TextJustification       justification,
    int                             horizontalTextSpacing,
    const rgb_matrix::Color*        timeColor,
    const DMDF*                     timeFont
)
{
    _hueShiftModifier = new HueShiftModifier(0.0, 360.0, 1.0, 1.0, 10.0, false);
    rgb_matrix::Color color = _hueShiftModifier->GetColor();
    _color = new rgb_matrix::Color(color);

    _time = new STime(
        origin, 
        timeFormat, 
        justification, 
        horizontalTextSpacing, 
        _color, 
        timeFont);
        
    _sources.push_back(_time);
}

MClock::~MClock()
{
    for (PanelSource* source : _sources)
    {
        delete source;
    }
    delete _hueShiftModifier;
    delete _color;
}

void MClock::internalUpdate()
{
    delete _color;
    rgb_matrix::Color color = _hueShiftModifier->GetColor();
    _color = new rgb_matrix::Color(color);
    _time->SetColor(_color);
}

////////////////////////////////////////////////////////////////////////////////