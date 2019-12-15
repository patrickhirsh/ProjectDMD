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

    _linearInterpolationModifier = new LinearInterpolationModifier();
    _point1 = std::tuple<int, int>(Render::GetDisplayWidth() / 2 - 50, ((20) - (ResourceManager::GetFont("StarTrek_20.dmdf")->GetFontHeight() / 2)));
    _point2 = std::tuple<int, int>(Render::GetDisplayWidth() / 2 + 50, ((20) - (ResourceManager::GetFont("StarTrek_20.dmdf")->GetFontHeight() / 2)));
    _direction = true;

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

    if (_linearInterpolationModifier->IsActive())
    {
        _logo->SetOrigin(_linearInterpolationModifier->GetPoint());
    }
    else
    {
        if (_direction)
        {
            _linearInterpolationModifier->Start(
                _point1,
                _point2,
                0.5);
        }
        else
        {
            _linearInterpolationModifier->Start(
                _point2,
                _point1,
                0.5);
        }
        _logo->SetOrigin(_linearInterpolationModifier->GetPoint());
        _direction = !_direction;
    }
}

////////////////////////////////////////////////////////////////////////////////