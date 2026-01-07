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

std::wstring HConsole::get_I_from_R(const std::wstring& r_path) {
   std::wstring i_path = r_path;
   i_path[i_path.find(L"$R")] = L'$';
   i_path[i_path.find(L"$R") + 1] = L'I';
   return i_path;
}

/**
 * выводит в консоль оригинальный путь из корзины
 *
 *
 */
std::wstring HConsole::read_original_path(std::string& i_path) {
   std::ifstream file(i_path, std::ios::binary);
   if (!file)
      return L"";

   file.seekg(0x18, std::ios::beg);

   std::wstring result;
   char ch;
   while (file.read(&ch, 1) && ch != L'\0')
      result += ch;

   return result;
}
