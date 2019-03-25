#pragma once
#include "libraries.h"
#include "DMDF.h"

class ResourceManager
{
    private:
        // paths to all DMDF fonts
        static std::vector<std::string>* _DMDFPaths;
        static std::map<std::string, DMDF*>* _fonts;

    public:
        static void LoadFonts(std::string path);
        static DMDF* GetFont(std::string fontName);
};