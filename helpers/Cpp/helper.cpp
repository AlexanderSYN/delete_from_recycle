//
// Created by AlexanderSYN on 03.01.2026.
//

#include "../Headers/helper.h"

/**
 * функция находит идекс пробела в тексте
 *
 * @param text текст в котором вам надо найти пробел
 * @return если есть пробел возвращает индекс из текста, иначе 0
 */
int helper::get_index_where_space(std::string text) {
    try {
        for (int i = 0; i <= text.length(); i++) {
            if (text[i] != ' ') continue;

            return i;
        }

        return 0;
    } catch (const std::exception &e) {
        return 0;
    }
}

/**
 * проверить vector со всеми расширениями и если
 * есть такое расширение вернуть индекс элемента
 *
 * @param extension расширение которую надо проверить
 * @param all_extensions vector где хранятся расширения
 * @return если функция нашла расширение тогда вернуть true иначе false
 */
bool helper::check_extension(std::string extension, std::vector<std::string> all_extensions) {
    try {
        for (int i = 0; i < all_extensions.size(); i++) {
            if (all_extensions.at(i) == extension) {
                return true;
            }
        }
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return false;
    }
}

/**
 * проверить vector со всеми расширениями и если
 * есть такое расширение вернуть индекс элемента
 *
 * @param extension расширение которую надо проверить
 * @param all_extensions vector где хранятся расширения
 * @return если функция нашла расширение тогда вернуть индекса иначе -1
 */
int helper::check_extension_and_get_index(std::string extension, std::vector<std::string> all_extensions) {
    try {
        for (int i = 0; i < all_extensions.size(); i++) {
            if (all_extensions.at(i) == extension) {
                return i;
            }
        }
        return -1;
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return -1;
    }
}

