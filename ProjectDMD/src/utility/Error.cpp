////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Error.h"


////////////////////////////////////////////////////////////////////////////////
// ErrorHandler

/* Statics */
int ErrorHandler::_errorCount = 0;
int ErrorHandler::_warningCount = 0;


/* Log a Warning, Error, or Fatal Error. Warnings are logged to console. Errors are 
displayed on-screen temporarily, and Fatal Errors halt the program, then display on-screen. */
void ErrorHandler::Log(std::string source, std::string reason, ErrorNum errorNumber)
{   
    // FATAL
    if (errorNumber < 500)
    {
        printf("[%s] FATAL ERROR (%d): %s\n", source.c_str(), (int)errorNumber, reason.c_str());
    }

    // ERROR
    else if (errorNumber < 1000)
    {
        _errorCount++;
        printf("[%s] ERROR (%d): %s\n", source.c_str(), (int)errorNumber, reason.c_str());
    }

    // WARNING
    else
    {
        _warningCount++;
        printf("[%s] WARNING (%d): %s\n", source.c_str(), (int)errorNumber, reason.c_str());
    }
}

////////////////////////////////////////////////////////////////////////////////

