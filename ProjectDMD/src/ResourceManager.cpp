#include "../include/ResourceManager.h"
#include "../include/DMDF.h"

std::vector<std::string>* ResourceManager::_DMDFPaths = new std::vector<std::string>();
std::map<std::string, DMDF*>* ResourceManager::_fonts = new std::map<std::string, DMDF*>();
std::string DMDFPathBase = "resources/fonts/dmdf/";

/*  Maps all "stern spec" values to their 0-255 pixel values. These input DMDF values and
    their mappings come straight from the DMDF/DMDA ProjectDMD Resource Packager. 255 represents
    transparent (and maps to 1 in our system). This map should be used to store actual 0-255 intensity
    values within a DMDF/DMDA object when constructing a new font from a .dmdf/.dmda file. 
    DMDobject classes chould check for a value of 1 within a DMDF character to handle transparency. 

    Note: a mapping to "9" is missing because 203 maps to both 6 and 9 in the stern spec.
*/  
std::map<unsigned char, unsigned char>* ResourceManager::_sternPixelMap = new std::map<unsigned char, unsigned char>({
    { 255, 1 },
    { 15, 255 },
    { 14, 247 },
    { 13, 240 },
    { 12, 232 },
    { 11, 222 },
    { 10, 212 },
    { 8, 190 },
    { 7, 214 },
    { 6, 203 },
    { 5, 191 },
    { 4, 177 },
    { 3, 161 },
    { 2, 140 },
    { 1, 113 },
    { 0, 0 },   
});


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

unsigned char ResourceManager::ConvertDMDFValue(unsigned char value)
{
    if (_sternPixelMap->find(value) != _sternPixelMap->end()) { return (*_sternPixelMap)[value]; }
    else { LogError("Resource Manager", "unrecognized pixel value dectected: " + std::to_string((uint)value) + ". expected value: 0-15 or 255"); return (*_sternPixelMap)[255]; }
}