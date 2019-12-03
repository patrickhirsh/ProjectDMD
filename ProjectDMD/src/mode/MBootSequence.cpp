////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelMode.h"


////////////////////////////////////////////////////////////////////////////////
// MODE: BootSequence

MBootSequence::MBootSequence(const rgb_matrix::Color* color)
{
    SText* logo = new SText(
        std::string("ProjectDMD"),
        std::tuple<int, int>(Render::GetDisplayWidth() / 2, ((Render::GetDisplayHeight() / 2) - (ResourceManager::GetFont("StarTrek_20.dmdf")->GetFontHeight() / 2))),
        ResourceManager::GetFont("StarTrek_20.dmdf")
    );

    _sources.push_back(logo);
}

MBootSequence::~MBootSequence()
{
    for (PanelSource* source : _sources)
    {
        delete source;
    }
}

void MBootSequence::internalUpdate()
{
    return;
}

////////////////////////////////////////////////////////////////////////////////