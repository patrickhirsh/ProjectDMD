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
    STime* Clock = new STime(
        origin, 
        timeFormat, 
        justification, 
        horizontalTextSpacing, 
        timeColor, 
        timeFont);
        
    _sources.push_back(Clock);
}

MClock::~MClock()
{
    for (PanelSource* source : _sources)
    {
        delete source;
    }
}

void MClock::internalUpdate()
{
    return;
}

////////////////////////////////////////////////////////////////////////////////