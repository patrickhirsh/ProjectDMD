////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Error.h"


////////////////////////////////////////////////////////////////////////////////
// ErrorHandler

/* Statics */
int ErrorHandler::_errorCount = 0;
int ErrorHandler::_warningCount = 0;

/* Fatal errors stop execution immediately and attempt to draw to the
panel (if possible) to describe the error */
void ErrorHandler::FatalError(std::string source, std::string reason)
{
    printf("%s FATAL ERROR: %s\n", source.c_str(), reason.c_str());
}

/* Errors don't stop execution, but they are displayed on-screen */
void ErrorHandler::LogError(std::string source, std::string reason)
{

    printf("%s ERROR: %s\n", source.c_str(), reason.c_str());
    // TODO: render temporary animation on-screen to indicate an error
}

/* Warnings are logged, but not displayed on screen */
void ErrorHandler::LogWarning(std::string source, std::string reason)
{
    printf("%s WARNING: %s\n", source.c_str(), reason.c_str());
}

////////////////////////////////////////////////////////////////////////////////

