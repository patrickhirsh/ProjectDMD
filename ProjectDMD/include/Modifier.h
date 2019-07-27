////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <vector>

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"


////////////////////////////////////////////////////////////////////////////////
// Modifier

class Modifier
{
public:
    Modifier();
    virtual ~Modifier();

    /* Augments the values of color based on this Modifier's defined behavior. */
    virtual void Modify(rgb_matrix::Color& color) const;

    /* adjusts the position of a render object's origin based on this Modifier's defined behavior. */
    virtual void Transform(std::tuple<int, int>& origin) const;

};

////////////////////////////////////////////////////////////////////////////////