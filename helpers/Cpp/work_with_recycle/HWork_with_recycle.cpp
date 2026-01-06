
#include "../../Headers/work_with_recycle/HWork_with_recycle.h"

#include <algorithm>



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

/**
 * получение всех файлов из корзины (с полным путем)
 *
 * @return если найдены то вернуть их, иначе пустой вектор
 */
std::vector<std::string> HWork_with_recycle::get_files_from_recycles() {
    try {
        std::vector<std::string> result;

        if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
            return result;

        IShellItem* recycleItem = nullptr;
        if (FAILED(SHGetKnownFolderItem(
            FOLDERID_RecycleBinFolder,
            KF_FLAG_DEFAULT,
            nullptr,
            IID_PPV_ARGS(&recycleItem))))
        {
            CoUninitialize();
            return result;
        }

        IShellFolder* recycleFolder = nullptr;
        recycleItem->BindToHandler(
            nullptr, BHID_SFObject,
            IID_PPV_ARGS(&recycleFolder)
        );
        recycleItem->Release();

        IEnumIDList* enumItems = nullptr;
        recycleFolder->EnumObjects(
            nullptr, SHCONTF_NONFOLDERS, &enumItems
        );

        LPITEMIDLIST pidl;
        ULONG fetched;

        while (enumItems->Next(1, &pidl, &fetched) == S_OK) {
            IShellItem* item = nullptr;

            if (SUCCEEDED(SHCreateItemWithParent(
                nullptr, recycleFolder,
                pidl, IID_PPV_ARGS(&item))))
            {
                PWSTR path = nullptr;

                if (SUCCEEDED(item->GetDisplayName(
                    SIGDN_FILESYSPATH, &path)))
                {
                    std::wstring ws(path);
                    result.emplace_back(ws.begin(), ws.end());
                }

                CoTaskMemFree(path);
                item->Release();
            }

            CoTaskMemFree(pidl);
        }

        enumItems->Release();
        recycleFolder->Release();
        CoUninitialize();

        return result;

    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return {};
    }
}


/**
 * очистка корзины (путь к корзине получает функция get_files_from_recycles сама)
 *
 * @param all_extensions_to_del vector где находятся все расширения, которые надо удалить
 */
void HWork_with_recycle::clear_recycle(std::vector<std::string> all_extensions_to_del) {
    try {
        std::cout << "Сканирование корзины..." << std::endl;

        for (auto file_path_from_recycle : get_files_from_recycles()) {
            if (all_extensions_to_del.at(0) == "all") {
                fs::remove(file_path_from_recycle);
                std::cout << "Успешно удален файл [" << file_path_from_recycle << "]" << std::endl;
            }
            else {
                for (auto ext : all_extensions_to_del) {
                    if (file_path_from_recycle.ends_with("." + ext)) {
                        fs::remove(file_path_from_recycle);
                        std::cout << "Успешно удален файл [" << file_path_from_recycle << "]" << std::endl;
                    }
                }
            }

            std::cout << "" << std::endl;

        }

    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
}

