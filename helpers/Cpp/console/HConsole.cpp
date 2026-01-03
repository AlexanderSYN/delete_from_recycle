//
// Created by AlexanderSYN on 03.01.2026.
//

#include <cstdlib>

#include "../../Headers/console/HConsole.h"

/**
 * очистка консоли, раотает на всех ОС
 */
void HConsole::clear_console() {
#ifdef _WIN32 // for windows
   system("cls");
#else // for Mac OS / Linux
   system("clear");
#endif
}


