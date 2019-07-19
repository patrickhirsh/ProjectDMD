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

class ErrorHandler
{
public:
				static void					FatalError(std::string source, std::string reason);
				static void					LogError(std::string source, std::string reason);
				static void					LogWarning(std::string source, std::string reason);
				static int						GetErrorCount() { return _errorCount; }
				static int						GetWarningCount() { return _warningCount; }

private:
				static int						_errorCount;
				static int						_warningCount;
};