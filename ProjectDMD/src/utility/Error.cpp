////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Error.h"


////////////////////////////////////////////////////////////////////////////////
// ErrorHandler

/* Statics */
int ErrorHandler::_errorCount = 0;
int ErrorHandler::_warningCount = 0;


/* Log a Warning, Error, or Fatal Error. Warnings and Errors are logged to console. 
Fatal Errors halt the program, then display on-screen. */
void ErrorHandler::Log(std::string source, std::string reason, ErrorNum errorNumber)
{   
    // FATAL
    if (errorNumber < 500)
    {
        printf("[%d] FATAL ERROR [%s]: %s\n", (int)errorNumber, source.c_str(), reason.c_str());
    }

    // ERROR
    else if (errorNumber < 1000)
    {
        _errorCount++;
        printf("[%d] ERROR [%s]: %s\n", (int)errorNumber, source.c_str(), reason.c_str());
    }

    // WARNING
    else
    {
        _warningCount++;
        printf("[%d] WARNING [%s]: %s\n", (int)errorNumber, source.c_str(), reason.c_str());
    }
}

////////////////////////////////////////////////////////////////////////////////

