////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/ResourceManager.h"


////////////////////////////////////////////////////////////////////////////////
// DMDColorPalette

/* Maps all "stern spec" values (255 and 0-15) to their 0.0f-1.0f brightness values.
These input DMDF values and their mappings come straight from the DMDF/DMDA ProjectDMD
Resource Packager. 255 represents transparent (and maps to -1 in our system). */
std::map<unsigned char, float>* DMDColorPalette::_sternPixelMap = new std::map<unsigned char, float>({
    { 255, -1.0f },
    { 15, (255.0f / 255.0f) },
    { 14, (247.0f / 255.0f) },
    { 13, (240.0f / 255.0f) },
    { 12, (232.0f / 255.0f) },
    { 11, (222.0f / 255.0f) },
    { 10, (212.0f / 255.0f) },
    { 9, (203.0f / 255.0f) },
    { 8, (190.0f / 255.0f) },
    { 7, (214.0f / 255.0f) },
    { 6, (203.0f / 255.0f) },
    { 5, (191.0f / 255.0f) },
    { 4, (177.0f / 255.0f) },
    { 3, (161.0f / 255.0f) },
    { 2, (140.0f / 255.0f) },
    { 1, (113.0f / 255.0f) },
    { 0, 0.0f }
    });

/* Creates a color palette (store intensity values in the form of rgb_matrix colors)
for use when rendering for improved performance. Do this once for any mono-color render,
then call GetColor() to get any color in the palette without the computational overhead
of per-pixel transcoding. */
DMDColorPalette::DMDColorPalette(rgb_matrix::Color color)
{
    _colorMapping = std::map<unsigned char, rgb_matrix::Color*>();
    for (unsigned char i = 0; i < 16; i++)
    {
        rgb_matrix::Color* newColor = new rgb_matrix::Color(
            (uint8_t)((float)color.r * getBrightnessFromSternValue(i)),
            (uint8_t)((float)color.g * getBrightnessFromSternValue(i)),
            (uint8_t)((float)color.b * getBrightnessFromSternValue(i)));
        _colorMapping[i] = newColor;
    }
}

/* Creates a color palette (store intensity values in the form of rgb_matrix colors)
for use when rendering for improved performance. Do this once for any mono-color render,
then call GetColor() to get any color in the palette without the computational overhead
of per-pixel transcoding. */
DMDColorPalette::DMDColorPalette(const rgb_matrix::Color* color)
{
    _colorMapping = std::map<unsigned char, rgb_matrix::Color*>();
    for (unsigned char i = 0; i < 16; i++)
    {
        rgb_matrix::Color* newColor = new rgb_matrix::Color(
            (uint8_t)((float)color->r * getBrightnessFromSternValue(i)),
            (uint8_t)((float)color->g * getBrightnessFromSternValue(i)),
            (uint8_t)((float)color->b * getBrightnessFromSternValue(i)));
        _colorMapping[i] = newColor;
    }
}

DMDColorPalette::~DMDColorPalette()
{
    for (unsigned char i = 0; i < 16; i++)
        delete _colorMapping[i];
}

/* Given a stern value (expects 0-15 or 255), returns the equivalent rgb_matrix::Color */
const rgb_matrix::Color* DMDColorPalette::GetColor(unsigned char sternValue) const
{
    try { return _colorMapping[sternValue]; }
    catch (const std::out_of_range& oor)
    {
        ErrorHandler::Log("DMDColorPalette", 
            "unrecognized sternValue detected: " + std::to_string((unsigned int)sternValue) + ". expected value: 0-15 or 255", 
            ErrorNum::WARNING_INVALID_STERN_COLOR_VAL);
        return ResourceManager::GetSystemColorPalette()->GetColor(255);
    }
}

/* Returns an intensity value (0.0f-1.0f) from a stern 255 or 0-15 value.
Returns -1.0f for transparent (or unknown value) */
float DMDColorPalette::getBrightnessFromSternValue(unsigned char value)
{
    if (_sternPixelMap->find(value) != _sternPixelMap->end()) { return (*_sternPixelMap)[value]; }
    else 
    { 
        ErrorHandler::Log("DMDColorPalette", 
            "unrecognized sternValue dectected: " + std::to_string((unsigned int)value) + ". expected value: 0-15 or 255", 
            ErrorNum::WARNING_INVALID_STERN_COLOR_VAL);         
        return (*_sternPixelMap)[255]; 
    }
}

////////////////////////////////////////////////////////////////////////////////