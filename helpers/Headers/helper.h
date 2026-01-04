//
// Created by 00077 on 03.01.2026.
//

#ifndef HELPER_H
#define HELPER_H

#ifdef _IOSTREAM_
    #define IOSTREAM_INCLUDED 1
#else
    #include <iostream>
    #define IOSTREAM_INCLUDED 0
#endif
#include <vector>

class helper {
public:
    static int get_index_where_space(std::string text);
    static bool check_extension(std::string extension, std::vector<std::string> all_extensions);
    static int check_extension_and_get_index(std::string extension, std::vector<std::string> all_extensions);
};

#endif //HELPER_H
