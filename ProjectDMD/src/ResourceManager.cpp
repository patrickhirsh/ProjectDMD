#include "../include/ResourceManager.h"
#include "../include/DMDF.h"

std::vector<std::string>* ResourceManager::_DMDFPaths = new std::vector<std::string>();
std::map<std::string, DMDF*>* ResourceManager::_fonts = new std::map<std::string, DMDF*>();

std::string DMDFPathBase = "resources/fonts/dmdf/";

void ResourceManager::LoadFonts(std::string path)
{
    // load fonts from resources directory
    _DMDFPaths = new std::vector<std::string>();
    _DMDFPaths->push_back(DMDFPathBase + "StarTrek.dmdf");

    // cache fonts in _fonts map
    _fonts = new std::map<std::string, DMDF*>();
    for (std::size_t i = 0; i < _DMDFPaths->size(); i++)
    {
        DMDF* font = new DMDF((*_DMDFPaths)[i]);
        (*_fonts)[font->GetName()] = font;
    }    
}

DMDF* ResourceManager::GetFont(std::string fontName)
{
    if (_fonts->find(fontName) != _fonts->end()) { return (*_fonts)[fontName]; }
    else { return NULL; }
}