#ifndef HWORKWITHRECYCLE_H
#define HWORKWITHRECYCLE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <comdef.h>

namespace fs = std::filesystem;

static std::chrono::seconds time_to_delete_files_from_recycle = static_cast<std::chrono::seconds>(3);// in sec

class HWork_with_recycle {
public:
    static bool clear_recycle(std::string path_to_recycle, std::vector<std::string> all_extensions_to_del);
    static bool compare_extension_ignore_case(const std::string& ext1, const std::string& ext2);

};

#endif