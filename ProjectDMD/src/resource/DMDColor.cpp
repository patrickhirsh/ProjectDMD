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
    { 15, (float)(255 / 255) },
    { 14, (float)(247 / 255) },
    { 13, (float)(240 / 255) },
    { 12, (float)(232 / 255) },
    { 11, (float)(222 / 255) },
    { 10, (float)(212 / 255) },
    { 9, (float)(203 / 255) },
    { 8, (float)(190 / 255) },
    { 7, (float)(214 / 255) },
    { 6, (float)(203 / 255) },
    { 5, (float)(191 / 255) },
    { 4, (float)(177 / 255) },
    { 3, (float)(161 / 255) },
    { 2, (float)(140 / 255) },
    { 1, (float)(113 / 255) },
    { 0, 0.0f }
    });

/* Creates a color palette (star intensity values in the form of rgb_matrix colors)
for use when rendering for improved performance. Do this once for any mono-color render,
then call GetColor() to get any color in the palette without the computational overhead
of per-pixel transcoding. */
DMDColorPalette::DMDColorPalette(rgb_matrix::Color color)
{
    _colorMapping = std::map<unsigned char, rgb_matrix::Color*>();
    for (unsigned char i = 0; i < 16; i++)
    {
        rgb_matrix::Color* newColor = new rgb_matrix::Color(
            (uint8_t)(color.r * getBrightnessFromSternValue(i)),
            (uint8_t)(color.g * getBrightnessFromSternValue(i)),
            (uint8_t)(color.b * getBrightnessFromSternValue(i)));
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
            (uint8_t)(color->r * getBrightnessFromSternValue(i)),
            (uint8_t)(color->g * getBrightnessFromSternValue(i)),
            (uint8_t)(color->b * getBrightnessFromSternValue(i)));
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