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
#include "Render.h"

// Resources
class DMDF;
class DMDFC;
class DMDColorPalette;

////////////////////////////////////////////////////////////////////////////////
// ResourceManager

class ResourceManager
{
public:
    /* initialize all resources. Should be called on startup before any rendering is done */
    static void Initialize();

    /* get font by font name (includes file extension) */
    static const DMDF* GetFont(std::string fontName);

    /* system resources*/
    static void SetSystemColorPalette(rgb_matrix::Color color);     // set the system color palette
    static bool SetSystemFont(std::string fontName);                // set the system font
    static const DMDColorPalette* GetSystemColorPalette();          // get the system color palette
    static const DMDF* GetSystemFont();                             // get the system font

private:
    static DMDColorPalette*									_systemColorPalette;
    static DMDF*											_systemFont;
    static std::map<std::string, DMDF*>*					_fonts;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DMDFont

/* Represents a special kind of font that utilizes 0-15 pixel intensity in mono
colorspace. These fonts are generated with the PinExporter tool and packaged
to be as small on disk/in memory as possible */
class DMDF
{
public:
    /* Create a new DMDF based on a .dmdf file */
    DMDF(std::string file);
    DMDF(const DMDF* fontToCopy);

    /* Get this DMDF character's raster */
    const DMDFC* GetCharacter(const char c) const;

    /* Get the string name of this DMDF */
    const std::string GetName() const;

    /* Get the number of characters in this DMDF */
    int GetCount() const;

    /* DMDF fonts are required to have a uniform raster height */
    int GetFontHeight() const;

    /* DMDFs remain unloaded if the constructor fails to load the font. (non-uniform
    character height, invalid characters, invalid dmdf file, etc..) */
    bool IsLoaded() const;

private:
    bool _loaded;
    std::map<char, DMDFC*>*					                _characters;
    int														_fontHeight;
    std::string												_fontName;
};

class DMDFC
{
public:
    DMDFC(char character, std::tuple<int, int> dimmensions, std::vector<std::vector<unsigned char>>* raster);

    char													Character;							// Character that this DMDFC represents
    std::tuple<int, int>									CharacterDimensions;				// (width, height)
    std::vector<std::vector<unsigned char>>*				CharacterRaster;					// (right to left, top to bottom)
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
    DMDColorPalette(const rgb_matrix::Color* color);
    ~DMDColorPalette();
    const rgb_matrix::Color* GetColor(unsigned char sternValue) const;

private:
    static std::map<unsigned char, float>* _sternPixelMap;
    static float getBrightnessFromSternValue(unsigned char sternValue);

    /* stores mappings from stern values (255 and 0-15) to Colors based on this DMDColorPalette*/
    mutable std::map<unsigned char, rgb_matrix::Color*> _colorMapping;
};

////////////////////////////////////////////////////////////////////////////////