#pragma once
#include "libraries.h"

using namespace rgb_matrix;


class PanelSource
{
    public:
        virtual void Restart() = 0;
        virtual void Update(Canvas *canvas) = 0;   
};


class Demo : public PanelSource
{
    private:
        int rVal;

    public: 
        void Restart();
        void Update(Canvas *canvas);
};


