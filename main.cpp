//
// Created by AlexanderSYN on 03.01.2026.
//

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale>
#include <windows.h>
#include <filesystem>

//
// console
//
#include "helpers/Headers/console/HConsole.h"
#include "helpers/Headers/helper.h"
#include "helpers/Headers/work_with_recycle/HWork_with_recycle.h"

//
// work wit recycle
//
#include <bits/this_thread_sleep.h>

namespace fs = std::filesystem;

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
        std::cout <<
                "[HINT] обычно путь к корзине (windows 10/11), которая на рабочем столе выглядит так: C:\\$Recycle.Bin\\S-1-5-21-4219180456-1581729916-1318582940-1001"
                << std::endl;
        std::cout << "Введите путь к корзине:";
        std::cin >> path_to_recycle;
        if (fs::exists(path_to_recycle)) {
            std::cout << "выберите - расширений будет 1) одно 2) больше одного:";
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "введите>>";
                std::cin >> tmp_extenstion;
                exten_files_to_delete_from_recycle.push_back(tmp_extenstion);
                // start program
                start_program();
            } else if (choice == 2) {
                std::cout <<
                        "вводите каждый раз новое расширение нажимая <Enter> (когда закончите, напишите 'done', или '!' , или '~' (max 100 расширений))"
                        << std::endl;
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
        }
        else {
            std::cerr << "корзина (папка) не найдена" << std::endl;
            std::cout << "" << std::endl;
            main();
        }

    } catch (const std::exception &e) {
        return 0;
    }

    return 0;
}

void start_program() {
    std::cout << "Програма запущена" << std::endl;
    std::cout << "каждые 5м корзина будет очищаться (можно изменить время)" << std::endl;
    std::cout << "для помощи напишите help" << std::endl;

    auto last_check_time = std::chrono::steady_clock::now();
    std::chrono::seconds check_interval(time_to_delete_files_from_recycle);

    std::string command;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        // таймер автоудаление корзины
        auto now = std::chrono::steady_clock::now();
        if (now - last_check_time >= check_interval) {
            std::cout << "\n[Автоматическа проверка корзины...]" << std::endl;
            HWork_with_recycle::clear_recycle(path_to_recycle, exten_files_to_delete_from_recycle);
            last_check_time = now;
        }

        if (std::cin.rdbuf()->in_avail() > 0) {
            std::cout << "$ ";
            std::getline(std::cin, command);

            if (command == "help") {
                std::cout << "help - для помощи" << std::endl;
                std::cout << "add (расширение) - добавить расширения" << std::endl;
                std::cout << "del (расширение) - удалить расширение" << std::endl;
                std::cout << "time (время в секундах) - изменить время проверки расширений в корзине, для удалений" <<
                        std::endl;
                std::cout << "ext - вывод всех расширений которые вы удаляете" << std::endl;
                std::cout << "clrec - очистить корзину" << std::endl;
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
            // time (set time)
            //
            else if (command.substr(0, 5) == "time ") {
                try {
                    int tmp_for_time = std::stod(command.substr(5));
                    time_to_delete_files_from_recycle = static_cast<std::chrono::seconds>(tmp_for_time);
                    std::cout << "время для автоудалени изменено на " << time_to_delete_files_from_recycle << std::endl;
                } catch (const std::exception &e) {
                    std::cerr << "[ERROR] " << e.what() << std::endl;
                    std::cout << "возможно вы не ввели неправильно время, надо писать так - 12" << std::endl;
                }
            } else if (command == "clrec") {
                // очистка корзины
                char choice;
                std::cout << "вы уверены удалить файлы? (Y/N): ";
                std::cin >> choice;
                if (std::tolower(choice) == 'y') {
                    HWork_with_recycle::clear_recycle(path_to_recycle, exten_files_to_delete_from_recycle);
                } else {
                    std::cout << "действие отменено++" << std::endl;
                }
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
            //
            // ex / exit
            //
            else if (command == "ex" || command == "exit") {
                return;
            }

            //
            // else
            //
            else {
                std::cout << "для помощи напишите help" << std::endl;
                std::cout << ">>";
                std::cin >> command;
            }
        } else {
            // небольшая пауза, чтобы не грузить процессор
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
