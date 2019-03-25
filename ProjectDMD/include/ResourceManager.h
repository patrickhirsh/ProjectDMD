#pragma once
#include "Libraries.h"
#include "DMDF.h"

class ResourceManager
{
    private:
        // paths to all DMDF fonts
        static std::vector<std::string>* _DMDFPaths;
        static std::map<std::string, DMDF*>* _fonts;
        static std::map<unsigned char, unsigned char>* _sternPixelMap;

    public:
        static void LoadFonts(std::string path);
        static DMDF* GetFont(std::string fontName);
        static unsigned char ConvertDMDFValue(unsigned char);
};