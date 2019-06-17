#pragma once
#include "Libraries.h"
#include "Render.h"

using namespace rgb_matrix;


class PanelSource
{
    public:
        virtual void Start(Canvas* canvas) = 0;
        virtual void Restart() = 0;
        virtual void Update(Canvas* canvas) = 0;   
};


class Clock : public PanelSource
{
				public:
								void Start(Canvas* canvas);
								void Restart();
								void Update(Canvas* canvas);

    private:
        Font font;
        Color color;
        const char* time_format;
        char text_buffer[256];
        struct tm tm;
        time_t* t;
};


