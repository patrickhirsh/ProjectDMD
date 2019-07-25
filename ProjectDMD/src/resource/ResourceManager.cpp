////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/ResourceManager.h"


////////////////////////////////////////////////////////////////////////////////
// ResourceManager

/* Statics */
DMDColorPalette* ResourceManager::_systemColorPalette;
DMDF* ResourceManager::_systemFont;
std::map<std::string, DMDF*>* ResourceManager::_fonts = new std::map<std::string, DMDF*>();

/* Constants*/
const std::string DMDF_PATH_BASE = "./resources/fonts/dmdf";


/* initialize all resources. Should be called on startup before any rendering is done*/
void ResourceManager::Initialize()
{
    // Initialize system global resources
    _systemFont = new DMDF("./SYSTEMFONT.dmdf");
    _systemColorPalette = new DMDColorPalette(rgb_matrix::Color(255, 0, 0));

    // load fonts from resources directory
#if __linux__
    DIR* dir;
    class dirent* ent;
    dir = opendir(DMDF_PATH_BASE.c_str());
    if (dir == NULL) { ErrorHandler::Log("ResourceManager", 
        "Couldn't open DMDF_PATH_BASE directory...", ErrorNum::FATAL_INVALID_RESOURCE_PATH); }
    else
    {
        errno = 0;
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name == std::string(".") || ent->d_name == std::string("..")) { continue; }
            std::string fontPath = DMDF_PATH_BASE + '/' + std::string(ent->d_name);
            DMDF* font = new DMDF(fontPath);
            (*_fonts)[font->GetName()] = font;
        }
        if (errno) { ErrorHandler::Log("ResourceManager", 
            std::string("Directory read error occured (errno: %d)", errno), ErrorNum::FATAL_DIR_READ_ERROR); }
    }
    closedir(dir);
#endif
}

/* Get font by font name (includes file extension) */
const DMDF* ResourceManager::GetFont(std::string fontName)
{
    if (_fonts->find(fontName) != _fonts->end()) { return (*_fonts)[fontName]; }
    else
    {
        ErrorHandler::Log("ResourceManager", 
            "font " + fontName + " not found...", ErrorNum::ERROR_RESOURCE_NOT_FOUND);
        
        return _systemFont;
    }
}

/* Sets the system color palette */
void ResourceManager::SetSystemColorPalette(rgb_matrix::Color color)
{
    _systemColorPalette = new DMDColorPalette(color);
}

/* Sets the system font */
bool ResourceManager::SetSystemFont(std::string fontName)
{
    if (ResourceManager::GetFont(fontName) != NULL)
    {
        _systemFont = new DMDF(ResourceManager::GetFont(fontName));
        return true;
    }
    else { return false; }
}

const DMDColorPalette* ResourceManager::GetSystemColorPalette()
{
    return _systemColorPalette;
}

const DMDF* ResourceManager::GetSystemFont()
{
    return _systemFont;
}

////////////////////////////////////////////////////////////////////////////////