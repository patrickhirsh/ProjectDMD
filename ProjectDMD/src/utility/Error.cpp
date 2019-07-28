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
        std::ostringstream oss;
        oss << "FATAL ERROR [" << (int)errorNumber << "]";
        Render::Clear();
        Render::Notification(
            oss.str(),
            ResourceManager::GetSystemFont(),
            std::tuple<int, int>((PANEL_WIDTH*PANEL_COUNT_X) / 2, (((PANEL_HEIGHT*PANEL_COUNT_Y) / 2) - (ResourceManager::GetSystemFont()->GetFontHeight() / 2))),
            ResourceManager::GetSystemColorPalette()->GetColor(15),
            1.0f,
            Render::TextJustification::Center,
            1,
            2);
        Render::FinalizeFrame();
        
        // halt program before exit
        printf("\n");
        printf("Press any key to exit...\n");
        printf("\n");
        while (std::cin.get() != '\n')
        {
            // wait..
        }
        exit(EXIT_FAILURE);
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

