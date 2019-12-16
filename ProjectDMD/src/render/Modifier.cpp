////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Modifier.h"


////////////////////////////////////////////////////////////////////////////////
// HueShiftModifier

HueShiftModifier::HueShiftModifier(
    double startHue, 
    double endHue,       
    double saturation,       
    double value,     
    double shiftFactor, 
    bool pingPong
)
{
    _startHue = startHue;
    _endHue = endHue;
    _saturation = saturation;
    _value = value;
    _shiftFactor = shiftFactor;
    _pingPong = pingPong;
    _startTime = std::chrono::high_resolution_clock::now();
}

rgb_matrix::Color HueShiftModifier::GetColor()
{
    // get absolute hue shift
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(now-_startTime).count();
    double absoluteShift = (elapsed / 1000.0) * _shiftFactor;

    // relative shift is 2* the shift span, with the second half representing the backwards movement (if PingPong)
    double relativeShift = std::fmod(absoluteShift, std::fabs(_startHue - _endHue) * 2.0); 
    double hue;

    if (_pingPong)
    {
        // if we're in the second half of the span, mirror and subtract to get poisition. Else, return relativeShift
        hue = relativeShift > std::fabs(_startHue - _endHue) ?
            std::fabs(_startHue - _endHue) - (relativeShift - std::fabs(_startHue - _endHue)) :
            relativeShift;
    }

    else
    {
        // don't PingPong. mod by hue span to get position
        hue = std::fmod(relativeShift, std::fabs(_startHue - _endHue));
    }

    return GraphicsUtility::HSV2RGB(hue);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// LinearInterpolationModifier

LinearInterpolationModifier::LinearInterpolationModifier()
{
    _isActive = false;
}

void LinearInterpolationModifier::Start(
    std::tuple<int, int> origin,           
    std::tuple<int, int> destination,
    double duration
)
{
    _isActive = true;
    _origin = origin;
    _destination = destination;
    _duration = duration;
    _startTime = std::chrono::high_resolution_clock::now();
}

bool LinearInterpolationModifier::IsActive()
{
    if (!_isActive) { return false; }
    else
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(now-_startTime).count();
        if (elapsed < (_duration * 1000.0)) { return true; }
        else 
        { 
            _isActive = false; 
            return false; 
        }
    }
}

std::tuple<int, int> LinearInterpolationModifier::GetPoint()
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(now-_startTime).count();
    if (elapsed >= (_duration * 1000.0)) { return _destination; }
    else
    {
        double progress = elapsed / (_duration * 1000.0);
        return std::tuple<int, int>(
            round(std::get<0>(_origin) + ((std::get<0>(_destination) - std::get<0>(_origin)) * progress)),
            round(std::get<1>(_origin) + ((std::get<1>(_destination) - std::get<1>(_origin)) * progress)));
    }
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// CosineInterpolationModifier

CosineInterpolationModifier::CosineInterpolationModifier()
{
    _isActive = false;
}

void CosineInterpolationModifier::Start(
    std::tuple<int, int> origin,           
    std::tuple<int, int> destination,
    double duration
)
{
    _isActive = true;
    _origin = origin;
    _destination = destination;
    _duration = duration;
    _startTime = std::chrono::high_resolution_clock::now();
}

bool CosineInterpolationModifier::IsActive()
{
    if (!_isActive) { return false; }
    else
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(now-_startTime).count();
        if (elapsed < (_duration * 1000.0)) { return true; }
        else 
        { 
            _isActive = false; 
            return false; 
        }
    }
}

std::tuple<int, int> CosineInterpolationModifier::GetPoint()
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(now-_startTime).count();
    if (elapsed >= (_duration * 1000.0)) { return _destination; }
    else
    {
        double progress = elapsed / (_duration * 1000.0);
        double mu2 = (1 - cos(progress * M_PI)) / 2;
        return std::tuple<int, int>(
            round((double)std::get<0>(_origin) * (1.0-mu2) + (double)std::get<0>(_destination) * mu2),
            round((double)std::get<1>(_origin) * (1.0-mu2) + (double)std::get<1>(_destination) * mu2));
    }
}