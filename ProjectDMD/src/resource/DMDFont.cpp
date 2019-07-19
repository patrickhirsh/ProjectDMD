#include "../../include/ResourceManager.h"


////////////////////////////////////////////////////////////////////////////////
// DMDF

/* Create a new DMDF based on a .dmdf file */
DMDF::DMDF(std::string file)
{
    // obtain font name (ignore trailing '/')
    int strIndex = file.length() - 1;
    std::string name = "";
    if  (file[strIndex] == '/') {strIndex--;}
    while  (file[strIndex] != '/') { name += file[strIndex]; strIndex--;}
    std::reverse(name.begin(), name.end());
    this->_fontName = name;

    // check file validity
    std::ifstream source;
    source.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try { source.open(file.c_str()); }
    catch (std::ifstream::failure e) { ErrorHandler::LogError("DMDF", "Error opening " + file + " : " + std::strerror(errno)); }

    if (!source.is_open()) { this->_loaded = false; }       
    else { this->_loaded = true; }
				if (this->_loaded)
				{
								// get end position and reserve buffer
								source.seekg(0, source.end);
								size_t length = source.tellg();
								source.seekg(0, source.beg);
								std::vector<char> buffer(length);

								// read into buffer
								source.read(&buffer[0], length);
								unsigned int bufferIndex = 0;

								// initialize font data
								this->_characters = new std::map<char, DMDFC*>();
								this->_fontHeight = buffer[bufferIndex + 2];

								// read and store all characters
								while (bufferIndex < length)
								{
												// read header
												char character = toupper(buffer[bufferIndex]);
												std::tuple<int, int> dimensions(buffer[bufferIndex + 1], buffer[bufferIndex + 2]);
												bufferIndex += 3;

												// ensure raster height uniformity
												if (std::get<1>(dimensions) != this->_fontHeight)
												{
																this->_loaded = false;
																ErrorHandler::LogError("DMDF", "Font " + _fontName + " contains non-uniform raster heights");
																return;
												}

												// initialize payload vector
												std::vector<std::vector<unsigned char>>* payload = new std::vector<std::vector<unsigned char>>(
																std::get<0>(dimensions), std::vector<unsigned char>(std::get<1>(dimensions), -99));

												// read payload
												for (int row = 0; row < std::get<1>(dimensions); row++)
																for (int col = 0; col < std::get<0>(dimensions); col++)
																{
																				(*payload)[col][row] = buffer[bufferIndex];
																				bufferIndex++;
																}

												// create an store the new DMDFC
												DMDFC* newDMDFC = new DMDFC(character, dimensions, payload);
												(*this->_characters)[character] = newDMDFC;
								}
				}
				source.close();
}

/* Get this DMDF character's raster */
DMDFC* DMDF::GetCharacter(const char c)
{
				if (_loaded)
				{
								auto iter = (*this->_characters).find(toupper(c));
								if (iter != (*this->_characters).end())
								{
												return iter->second;
								}
								return NULL;
				} 
				ErrorHandler::LogError("DMDF", "Tried to access a font that hasn't been loaded");
				return NULL;
}

/* Get the string name of this DMDF */
std::string DMDF::GetName()
{
				if (_loaded) { return this->_fontName; }
				ErrorHandler::LogError("DMDF", "Tried to access a font that hasn't been loaded");
				return "NOT_LOADED";
}

/* Get the number of characters in this DMDF */
int DMDF::GetCount()
{
				if (_loaded) { return (*this->_characters).size(); }
				ErrorHandler::LogError("DMDF", "Tried to access a font that hasn't been loaded");
				return 0;
}

/* DMDF fonts are required to have a uniform raster height */
int DMDF::GetFontHeight()
{
				if (_loaded) { return _fontHeight; }
				ErrorHandler::LogError("DMDF", "Tried to access a font that hasn't been loaded");
				return 0;
}

/* DMDFs remian unloaded if the constructor fails to load the font. (non-uniform 
character height, invalid characters, invalid dmdf file, etc..) */
bool DMDF::IsLoaded()
{
				return this->_loaded;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DMDFC

DMDFC::DMDFC(char character, std::tuple<int, int> dimmensions, std::vector<std::vector<unsigned char>>* raster)
{
				Character = character;
				CharacterDimensions = dimmensions;
				CharacterRaster = raster;
}

////////////////////////////////////////////////////////////////////////////////