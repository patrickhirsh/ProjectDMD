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