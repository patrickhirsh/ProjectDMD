#pragma once
#include "Libraries.h"

class DMDF
{
    public:
        DMDF(std::string file);
        std::vector<std::vector<unsigned char>>* GetCharacter(char c);
        std::tuple<int, int> GetDimensions();
        std::string GetName();
        int GetCount();
        bool IsLoaded();

    private:
        bool _loaded;
        std::map<char, std::vector<std::vector<unsigned char>>*>* _characters;
        std::tuple<int, int> _fontDimensions;
        std::string _fontName;
};