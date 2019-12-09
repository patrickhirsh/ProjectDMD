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
        ResourceManager::GetFont("StarTrek_20.dmdf"));

    SRectangle* rect = new SRectangle(
        std::tuple<int, int>(0, 0),
        std::tuple<int, int>(Render::GetDisplayWidth() - 1, Render::GetDisplayHeight() - 1),
        ResourceManager::GetSystemColorPalette()->GetColor(1),
        1.0f,
        false,
        2);

    _sources.push_back(rect);
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