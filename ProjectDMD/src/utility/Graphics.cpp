////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Graphics.h"


/* General purpose graphics utility implementations */
namespace GraphicsUtility
{
    /* Ref: https://stackoverflow.com/a/6930407
    h : hue in degrees (0 - 360)
    s : saturation (0.0 - 1.0) 
    v : value (0.0 - 1.0) */ 
    rgb_matrix::Color HSV2RGB(double h, double s, double v)
    {
        double r, g, b;
        double hh, p, q, t, ff;
        long i;

        if (s <= 0.0) { return rgb_matrix::Color((uint8_t)v, (uint8_t)v, (uint8_t)v); }

        hh = h;
        if (hh >= 360.0) { hh = 0.0; }
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = v * (1.0 - s);
        q = v * (1.0 - (s * ff));
        t = v * (1.0 - (s * (1.0 - ff)));

        switch(i) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
        r *= 255.0;
        g *= 255.0;
        b *= 255.0;
        return rgb_matrix::Color((uint8_t)r, (uint8_t)g, (uint8_t)b);
    }
}