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
#include <propkey.h>
#include <propsys.h>

#include "../console/HConsole.h"

namespace fs = std::filesystem;

static std::chrono::seconds time_to_delete_files_from_recycle = static_cast<std::chrono::seconds>(300);// in sec
static int tmp_time_in_sec = 300;

class HWork_with_recycle {
public:
    static void clear_recycle(std::vector<std::string> all_extensions_to_del);
    static bool compare_extension_ignore_case(const std::string& ext1, const std::string& ext2);
private:
    static std::vector<std::string> get_files_from_recycles();
};

#endif