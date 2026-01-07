//
// Created by AlexanderSYN on 03.01.2026.
//

#ifndef HCONSOLE_H
#define HCONSOLE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class HConsole {
public:
    static void clear_console();
    static std::wstring read_original_path(std::string& i_path);
    static std::wstring get_I_from_R(const std::wstring& r_path);
};

#endif //HCONSOLE_H
