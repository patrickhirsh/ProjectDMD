#include "../../include/ResourceManager.h"
#include "../../include/DMDF.h"

/* Constants*/
const std::string DMDF_PATH_BASE = "resources/fonts/dmdf/";

/* Statics */
std::vector<std::string>* ResourceManager::_DMDFPaths = new std::vector<std::string>();
std::map<std::string, DMDF*>* ResourceManager::_fonts = new std::map<std::string, DMDF*>();

/* System Globals */
DMDColorPalette* ResourceManager::_systemColorPalette;
DMDF* ResourceManager::_systemFont;


////////////////////////////////////////////////////////////////////////////////
// ResourceManager

void ResourceManager::InitializeResources()
{
				// load fonts from resources directory
				_DMDFPaths = new std::vector<std::string>();
				_DMDFPaths->push_back(DMDF_PATH_BASE + "StarTrek.dmdf");

				// cache fonts in _fonts map
				_fonts = new std::map<std::string, DMDF*>();
				for (std::size_t i = 0; i < _DMDFPaths->size(); i++)
				{
								DMDF* font = new DMDF((*_DMDFPaths)[i]);
								(*_fonts)[font->GetName()] = font;
				}

				// Initialize system global resources
				_systemColorPalette = new DMDColorPalette(rgb_matrix::Color(255, 0, 0));
				_systemFont = ResourceManager::GetFont("StarTrek.dmdf");
}

DMDF* ResourceManager::GetFont(std::string fontName)
{
				if (_fonts->find(fontName) != _fonts->end())
				{
								return (*_fonts)[fontName];
				}
				else
				{
								LogError("Resource Manager", "font " + fontName + " not found...");
								return NULL;
				}
}

void ResourceManager::SetSystemColorPalette(rgb_matrix::Color color)
{
				_systemColorPalette = new DMDColorPalette(color);
}

bool ResourceManager::SetSystemFont(std::string fontName)
{
				if (ResourceManager::GetFont(fontName) != NULL) 
				{ 
								_systemFont = ResourceManager::GetFont(fontName); 
								return true;
				}
				else { return false; }
}

DMDColorPalette* ResourceManager::GetSystemColorPalette()
{
				return _systemColorPalette;
}

DMDF* ResourceManager::GetSystemFont()
{
				return _systemFont;
}


////////////////////////////////////////////////////////////////////////////////
// DMDColorPalette

// static
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

DMDColorPalette::DMDColorPalette(rgb_matrix::Color color)
{
				_colorMapping = std::map<unsigned char, rgb_matrix::Color*>();
				for (unsigned char i = 0; i < 16; i++)
				{
								rgb_matrix::Color* newColor = new rgb_matrix::Color(
												(uint8_t)(color.r * GetBrightnessFromSternValue(i)),
												(uint8_t)(color.g * GetBrightnessFromSternValue(i)),
												(uint8_t)(color.b * GetBrightnessFromSternValue(i)));
								_colorMapping[i] = newColor;
				}
}

DMDColorPalette::~DMDColorPalette()
{
				for (unsigned char i = 0; i < 16; i++)
								delete _colorMapping[i];
}

rgb_matrix::Color* DMDColorPalette::GetColor(unsigned char sternValue)
{
				try { return _colorMapping[sternValue]; }
				catch (const std::out_of_range& oor) 
				{ 
								LogError("Resource Manager", "unrecognized sternValue detected: " + std::to_string((unsigned int)sternValue) + ". expected value: 0-15 or 255");
								return NULL; 
				}
}

float DMDColorPalette::GetBrightnessFromSternValue(unsigned char value)
{
    if (_sternPixelMap->find(value) != _sternPixelMap->end()) { return (*_sternPixelMap)[value]; }
    else { LogError("Resource Manager", "unrecognized pixel value dectected: " + std::to_string((unsigned int)value) + ". expected value: 0-15 or 255"); return (*_sternPixelMap)[255]; }
}