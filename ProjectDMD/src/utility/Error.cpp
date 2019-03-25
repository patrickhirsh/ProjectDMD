#include "../../include/Error.h"

void LogError(std::string source, std::string reason)
{
    printf("%s ERROR: %s\n", source.c_str(), reason.c_str());
    // TODO: render temporary animation on-screen to indicate an error
}