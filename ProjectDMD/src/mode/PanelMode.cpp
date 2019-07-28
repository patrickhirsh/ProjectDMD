////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelMode.h"


////////////////////////////////////////////////////////////////////////////////
// PanelMode

void PanelMode::Update()
{
    // mode-wide internal update (for mode logic)
    internalUpdate();

    // update all sources pertaining to this mode
    for (PanelSource* source : _sources)
    {
        source->Update();
    }
}

PanelMode::~PanelMode()
{
    // calls pure virtual destructor
}

////////////////////////////////////////////////////////////////////////////////