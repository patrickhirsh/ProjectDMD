#pragma once
#include "Libraries.h"

class DMDFC
{
public:
				DMDFC(char character, std::tuple<int, int> dimmensions, std::vector<std::vector<unsigned char>>* raster);			
				
				char Character;																																													// Character that this DMDFC represents
				std::tuple<int, int> CharacterDimensions;																			// This DMDFC's dimensions (width, height)
				std::vector<std::vector<unsigned char>>* CharacterRaster;			// This DMDFC's raster (start top left)
};

class DMDF
{
    public:
        DMDF(std::string file);																					// Create a new DMDF based on a .dmdf file
        DMDFC* GetCharacter(const char c);										// Get this DMDF character's raster
        std::string GetName();																						// Get the string name of this DMDF
        int GetCount();																													// Get the number of characters in this DMDF
								int GetFontHeight();																								// DMDF fonts are required to have a uniform raster height																																											
        
								/* DMDFs remian unloaded if the constructor fails to load the font. (non-uniform character height,
								inclad characters, invalid dmdf file, etc..) */
								bool IsLoaded();																												

    private:
        bool _loaded;
        std::map<char, DMDFC*>* _characters;
								int _fontHeight;
        std::string _fontName;
};