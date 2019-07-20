////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <vector>

// Linux Only
#if __linux__
#include <getopt.h>
#include <unistd.h>
#include <dirent.h>
#endif

/* RGB Matrix Library */
// https://github.com/hzeller/rpi-rgb-led-matrix
#include "../lib/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../lib/rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"
#include "../lib/rpi-rgb-led-matrix/include/pixel-mapper.h"
#include "../lib/rpi-rgb-led-matrix/include/graphics.h"

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"

// Resources
class DMDF;
class DMDFC;
class DMDColorPalette;

////////////////////////////////////////////////////////////////////////////////
// ResourceManager

class ResourceManager
{
public:
    static void Initialize();
    static DMDF* GetFont(std::string fontName);

    static void SetSystemColorPalette(rgb_matrix::Color color);
    static bool SetSystemFont(std::string fontName);
    static DMDColorPalette* GetSystemColorPalette();
    static DMDF* GetSystemFont();

private:
    static DMDColorPalette*																									_systemColorPalette;
    static DMDF*																																				_systemFont;
    static std::map<std::string, DMDF*>*												_fonts;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DMDFont

class DMDF
{
public:
    DMDF(std::string file);
    DMDFC* GetCharacter(const char c);
    std::string GetName();
    int GetCount();
    int GetFontHeight();
    bool IsLoaded();

private:
    bool _loaded;
    std::map<char, DMDFC*>*					_characters;
    int																									_fontHeight;
    std::string																	_fontName;
};

class DMDFC
{
public:
    DMDFC(char character, std::tuple<int, int> dimmensions, std::vector<std::vector<unsigned char>>* raster);

    char																																								Character;														// Character that this DMDFC represents
    std::tuple<int, int>																								CharacterDimensions;				// (width, height)
    std::vector<std::vector<unsigned char>>*				CharacterRaster;								// (right to left, top to bottom)
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DMDColor

/* For use with DMDObjects utilizing the 255 + 0-15 color space. Allows a color palette
to be created that maps colors with brightness intensities matching any of these color
space values. This lets us store color-agnostic intensity data in DMDObjects while
caching actual display colors within these objects to not incur computational overhead
required to convert these values during rendering.

NOTE: 255 is not included in this palette since it should NOT be drawn (to be transparent).
Classes utilizing color palettes should check for values of 255 and skip drawing that pixel. */
class DMDColorPalette
{
public:
    DMDColorPalette(rgb_matrix::Color color);
    ~DMDColorPalette();
    rgb_matrix::Color* GetColor(unsigned char sternValue);

private:
    static std::map<unsigned char, float>* _sternPixelMap;
    static float getBrightnessFromSternValue(unsigned char sternValue);

    /* stores mappings from stern values (255 and 0-15) to Colors based on this DMDColorPalette*/
    std::map<unsigned char, rgb_matrix::Color*> _colorMapping;
};

////////////////////////////////////////////////////////////////////////////////