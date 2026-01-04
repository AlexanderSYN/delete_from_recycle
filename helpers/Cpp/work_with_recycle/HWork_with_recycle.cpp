
#include "../../Headers/work_with_recycle/HWork_with_recycle.h"

/**
 * сравнение расширений
 *
 * @param ext1 расширение 1
 * @param ext2 расширение 2
 * @return если расширение совпадают тогда true, иначе false
 */
bool HWork_with_recycle::compare_extension_ignore_case(const std::string &ext1, const std::string &ext2) {
    if (ext1.length() != ext2.length()) return false;

    return std::equal(ext1.begin(), ext1.end(), ext2.begin(),
        [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        });
}


bool HWork_with_recycle::clear_recycle(std::string path_to_recycle, std::vector<std::string> all_extensions_to_del) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path_to_recycle)) {
            if (fs::is_regular_file(entry.path())) {
                std::string ext = entry.path().extension().string();

                if (ext.empty()) continue;

                if (ext[0] == '.') {
                    ext = ext.substr(1);
                }

                // проверяем все целевые расширения
                for (const auto& target_ext : all_extensions_to_del) {
                    if (compare_extension_ignore_case(ext, target_ext)) {
                        if (fs::remove(entry.path())) {
                            std::cout << "файл [" << entry.path() << "] успешно удален!" << std::endl;
                            return true;
                        } else {
                            std::cout << "файл [" << entry.path() << "] не удалось удалить!" << std::endl;
                            return false;
                        }
                    }
                }
            }
        }
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

