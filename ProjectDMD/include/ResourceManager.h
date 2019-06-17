#pragma once
#include "Libraries.h"
#include "DMDF.h"

/* For use with DMDObjects utilizing the 255 + 0-15 color space. Allows a color palette
to be created that maps colors with brightness intensities matching any of these color
space values. This lets us store color-agnostic intensity data in DMDObjects while
caching actual display colors within these objects to not incur computational overhead
required to convert these values during rendering.

NOTE: 255 is not included in this palette since it should NOT be drawn (to be transparent).
Classes utilizing color palettes should check for values of 255 and skipping drawing that pixel. */
class DMDColorPalette
{
public:
				DMDColorPalette(rgb_matrix::Color color);
				~DMDColorPalette();
				rgb_matrix::Color* GetColor(unsigned char sternValue);

private:
				/* Maps all "stern spec" values (255 and 0-15) to their 0.0f-1.0f brightness values.
				These input DMDF values and their mappings come straight from the DMDF/DMDA ProjectDMD
				Resource Packager. 255 represents transparent (and maps to -1 in our system). */
				static std::map<unsigned char, float>* _sternPixelMap;

				/* Returns an intensity value (0.0f-1.0f) from a stern 255 or 0-15 value.
				Returns -1.0f for transparent (or unknown value) */
				static float GetBrightnessFromSternValue(unsigned char sternValue);

				/* stores mappings from stern values (255 and 0-15) to Colors based on this DMDColorPalette*/
				std::map<unsigned char, rgb_matrix::Color*> _colorMapping;
};

class ResourceManager
{
				public:
								/* initialize all resources. Should be called on startup 
								before any rendering is done*/
								static void InitializeResources();

								static DMDF* GetFont(std::string fontName);
								
								/* System Global Resource Setters */
								static void SetSystemColorPalette(rgb_matrix::Color color);
								static bool SetSystemFont(std::string fontName);
								
								/* System Global Resource Getters*/
								static DMDColorPalette* GetSystemColorPalette();
								static DMDF* GetSystemFont();

    private:
        // paths to all DMDF fonts
        static std::vector<std::string>* _DMDFPaths;
        static std::map<std::string, DMDF*>* _fonts;    

								/* System Globals */
								static DMDColorPalette* _systemColorPalette;
								static DMDF* _systemFont;


};