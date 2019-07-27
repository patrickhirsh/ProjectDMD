////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelMode.h"


////////////////////////////////////////////////////////////////////////////////
// PanelMode

void PanelMode::Update()
{
    // mode-wide updates (update mode state, etc.)
    internalUpdate();

    // update all sources pertaining to this mode
    for (PanelSource* source : _activeSources)
    {
        source->Update();
    }

    // apply panel-wide modifiers
    if (_activeModifiers.size() > 0)
    {
        Render::ModifyPanel(_activeModifiers);
    }
}

PanelMode::~PanelMode()
{
    // calls pure virtual destructor
}

void PanelMode::AddModifier(Modifier* modifier)
{
    _activeModifiers.push_back(modifier);
}

////////////////////////////////////////////////////////////////////////////////