#include "../include/dateNow.h"
#include <ctime>

std::string dateNow()
{
    time_t timestamp = time(NULL);
    tm *data = localtime(&timestamp);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", data);

    return std::string(buffer);
}
