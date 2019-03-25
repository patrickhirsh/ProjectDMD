#include "../../include/DMDF.h"

DMDF::DMDF()
{
    this->_loaded = false;
}

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
    try { source.open(file); }
    catch (std::ifstream::failure e) { LogError("DMDF", "Error opening " + file + " : " + std::strerror(errno)); }    
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
        uint bufferIndex = 0;

        // initialize font data
        this->_characters = new std::map<char, std::vector<std::vector<unsigned char>>*>();
        this->_fontDimensions = std::tuple<int, int>(buffer[bufferIndex + 1], buffer[bufferIndex + 2]);

        // read and store all characters
        while(bufferIndex < length)
        {           
            // read header
            char character = buffer[bufferIndex];         
            std::tuple<int, int> dimensions(buffer[bufferIndex + 1], buffer[bufferIndex + 2]);
            bufferIndex += 3;

            // initialize payload vector
            std::vector<std::vector<unsigned char>>* payload = new std::vector<std::vector<unsigned char>>(
                std::get<1>(dimensions), std::vector<unsigned char>(std::get<0>(dimensions), 0));

            // read payload
            for (int row = 0; row < std::get<1>(dimensions); row++)
                for (int col = 0; col < std::get<0>(dimensions); col++)
                {
                    (*payload)[row][col] = buffer[bufferIndex];
                    bufferIndex++;
                }
            (*this->_characters)[character] = payload;       
        }
    }
}

std::vector<std::vector<unsigned char>>* DMDF::GetCharacter(char c)
{
    if ((this->_loaded) && ((*this->_characters).find(c) != (*this->_characters).end())) { return (*this->_characters)[c]; }       
    else { return NULL; }     
}

std::tuple<int, int> DMDF::GetDimensions()
{
    return this->_fontDimensions;
}

std::string DMDF::GetName()
{
    return this->_fontName;
}

bool DMDF::IsLoaded()
{
    return this->_loaded;
}

int DMDF::GetCount()
{
    return (*this->_characters).size();
}