////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/PanelMode.h"


////////////////////////////////////////////////////////////////////////////////
// MODE: BootSequence

MBootSequence::MBootSequence()
{
    _hueShiftModifier = new HueShiftModifier(0.0, 360.0, 1.0, 1.0, 10.0, false);
    rgb_matrix::Color color = _hueShiftModifier->GetColor();
    _color = new rgb_matrix::Color(color);

    _logo = new SText(
        std::string("ProjectDMD"),
        std::tuple<int, int>(Render::GetDisplayWidth() / 2, ((Render::GetDisplayHeight() / 2) - (ResourceManager::GetFont("StarTrek_20.dmdf")->GetFontHeight() / 2))),
        ResourceManager::GetFont("StarTrek_20.dmdf"));

    /*
    SRectangle* rect = new SRectangle(
        std::tuple<int, int>(0, 0),
        std::tuple<int, int>(Render::GetDisplayWidth() - 1, Render::GetDisplayHeight() - 1),
        ResourceManager::GetSystemColorPalette()->GetColor(1),
        1.0f,
        false,
        2);
    */

    //_sources.push_back(rect);
    _sources.push_back(_logo);
}

MBootSequence::~MBootSequence()
{
    for (PanelSource* source : _sources)
    {
        delete source;
    }
    delete _hueShiftModifier;
    delete _color;
}

void MBootSequence::internalUpdate()
{
    delete _color;
    rgb_matrix::Color color = _hueShiftModifier->GetColor();
    _color = new rgb_matrix::Color(color);
    _logo->SetColor(_color);
}

////////////////////////////////////////////////////////////////////////////////