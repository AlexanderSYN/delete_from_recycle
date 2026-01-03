//
// Created by AlexanderSYN on 03.01.2026.
//

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale>
#include <windows.h>
#include <cstdlib>

//
// console
//
#include "helpers/Headers/console/HConsole.h"
#include "helpers/Headers/helper.h"

std::string path_to_recycle = "none";
std::vector<std::string> exten_files_to_delete_from_recycle;

void start_program();

int main() {
    try {
        std::locale::global(std::locale(""));
        setlocale(LC_ALL, "ru");
        SetConsoleOutputCP(CP_UTF8);

        int choice = 1;
        std::string tmp_extenstion;

        std::string tmp_extens_file_to_del_recycl = "none";
        std::cout << "Введите путь к корзине:";
        std::cin >> path_to_recycle;
        std::cout << "выберите - расширений будет 1) одно 2) больше одного:";
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "введите>>";
            std::cin >> tmp_extenstion;
            exten_files_to_delete_from_recycle.push_back(tmp_extenstion);
            // start program
            start_program();
        } else if (choice == 2) {
            std::cout << "вводите каждый раз новое расширение нажимая <Enter> (когда закончите, напишите 'done', или '!' , или '~' (max 100 расширений))" << std::endl;
            for (int i = 0; i < 100; i++) {
                std::cout << i << ") ";
                std::cin >> tmp_extenstion;
                if (tmp_extenstion == "done" || tmp_extenstion == "!" || tmp_extenstion == "~") {
                    break;
                }
                exten_files_to_delete_from_recycle.push_back(tmp_extenstion);
            }
            // start program
            start_program();
        }

    } catch (const std::exception &e) {
        return 0;
    }

    return 0;
}

void start_program() {
    std::cout << "Програма запущена" << std::endl;
    std::cout << "для помощи напишите help" << std::endl;
    std::string command;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
         // clear buffer
        std::cout << "$ ";
        std::getline(std::cin, command);

        if (command == "help") {
            std::cout << "help - для помощи" << std::endl;
            std::cout << "add (расширение) - добавить расширения" << std::endl;
            std::cout << "del (расширение) - удалить расширение" << std::endl;
            std::cout << "time (время в секундах) - изменить время проверки расширений в корзине, для удалений" << std::endl;
            std::cout << "ext - вывод всех расширений которые вы удаляете" << std::endl;
            std::cout << "cls / clear - очистить консоль" << std::endl;
            std::cout << "ex / exit - для выхода из проги" << std::endl;
        }
        //
        // ext
        //
        else if (command == "ext") {
            for (int i = 0; i < exten_files_to_delete_from_recycle.size(); i++) {
                std::cout << i << ") " << exten_files_to_delete_from_recycle.at(i) << std::endl;
            }
        }
        //
        // cls / clear
        //
        else if (command == "cls" || command == "clear") {
            HConsole::clear_console();
        }
        //
        // add
        //
        else if (command.substr(0, 4) == "add ") {
            if (!helper::check_extension(command.substr(4), exten_files_to_delete_from_recycle)) {
                exten_files_to_delete_from_recycle.push_back(command.substr(4));
                std::cout << "расширение [" << command.substr(4) << "] успешно добавлено!" << std::endl;
            } else {
                std::cout << "расширение уже есть" << std::endl;
            }
        }
        //
        // del
        //
        else if (command.substr(0, 4) == "del ") {
            if (helper::check_extension(command.substr(4), exten_files_to_delete_from_recycle)) {
                std::erase(exten_files_to_delete_from_recycle, command.substr(4));
                std::cout << "расширение [" << command.substr(4) << "] успешно удалено!" << std::endl;
            } else {
                std::cout << "расширение не найдено!" << std::endl;
            }
        }

        else if (command == "ex" || command == "exit") {
            return;
        }

        else {
            std::cout << "для помощи напишите help" << std::endl;
            std::cout << ">>";
            std::cin >> command;
        }

    }
}


