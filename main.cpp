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
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

//
// console
//
#include "helpers/Headers/console/HConsole.h"
#include "helpers/Headers/helper.h"

//
// work with recycle
//
#include <condition_variable>

#include "helpers/Headers/work_with_recycle/HWork_with_recycle.h"

namespace fs = std::filesystem;

bool g_running = true;
std::mutex g_timer_mutex;
std::condition_variable g_timer_cv;

std::chrono::steady_clock::time_point g_next_fire;

std::vector<std::string> exten_files_to_delete_from_recycle;

void start_program();
void timer_thread();
void console_thread();

int main() {
    try {
        std::locale::global(std::locale(""));
        setlocale(LC_ALL, "ru");
        SetConsoleOutputCP(CP_UTF8);

        int choice = 1;
        std::string tmp_extenstion;

        std::cout << "Пишите расширения без точки!" << std::endl;
        std::cout << "выберите - расширений будет 1 - одно 2 - больше одного:";
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "введите>>";
            std::cin >> tmp_extenstion;
            if (tmp_extenstion.starts_with(".")) {
                std::cout << "написано же, без точки, переписывай путь!" << std::endl;
                std::cout << "нажмите любую клавишу для перезахода" << std::endl;
                std::cin >> tmp_extenstion;
                return 0;
            }
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
                if (tmp_extenstion.starts_with(".")) {
                    std::cout << "написано же, без точки, переписывай путь!" << std::endl;
                    std::cout << "нажмите любую символы и <Enter>" << std::endl;
                    std::cin >> tmp_extenstion;
                    return 0;
                }
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

//
// поток таймера
//
void timer_thread() {
    std::unique_lock<std::mutex> lock(g_timer_mutex);

    g_next_fire = std::chrono::steady_clock::now()
                    + time_to_delete_files_from_recycle;

    while (g_running) {
        g_timer_cv.wait_until(lock, g_next_fire);

        if (!g_running) break;

        auto now = std::chrono::steady_clock::now();
        if (now < g_next_fire) continue; // если разбудили из-за time / exit

        lock.unlock();
        std::cout << "\nАвтоматическая проверка корзины...\n";
        HWork_with_recycle::clear_recycle(exten_files_to_delete_from_recycle);
        std::cout << "$ ";
        std::cout.flush();
        lock.lock();

        g_next_fire = std::chrono::steady_clock::now()
                        + time_to_delete_files_from_recycle;

    }
}

//
// поток консоли
//
void console_thread() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string command;

    std::cout << "$ ";
    while (g_running && std::getline(std::cin, command)) {
        if (command == "help") {
            std::cout << "каждые " << time_to_delete_files_from_recycle << " корзина будет очищаться (можно изменить время)" << std::endl;
            std::cout << "help - для помощи" << std::endl;
            std::cout << "info - для инфы" << std::endl;
            std::cout << "add (расширение) - добавить расширения" << std::endl;
            std::cout << "del (расширение) - удалить расширение" << std::endl;
            std::cout << "time (время в секундах) - изменить время проверки расширений в корзине, для удалений" <<
                std::endl;
            std::cout << "ext - вывод всех расширений которые вы удаляете" << std::endl;
            std::cout << "clrec - очистить корзину" << std::endl;
            std::cout << "cls / clear - очистить консоль" << std::endl;
            std::cout << "ex / exit (или Ctrl + C) - для выхода из проги" << std::endl;
        }
        //
        // info
        //
        else if (command == "info") {
            std::cout << "____DELETE_FILES_FROM_RECYCLE____" << std::endl;
            std::cout << "______PROGRAMMER: Alexander______" << std::endl;
            std::cout << "______Github: AlexanderSYN_______" << std::endl;
            std::cout << "__________VERSION: 1.0___________" << std::endl;
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
        else if (command.rfind("time ", 0) == 0) {
            try {
                int sec = std::stoi(command.substr(5));

                if (sec <= 0) {
                    std::cerr << "Нельзя писать меньше 1 сек";
                    return;
                }

                {
                    std::lock_guard<std::mutex> lock(g_timer_mutex);
                    time_to_delete_files_from_recycle = std::chrono::seconds(sec);
                    g_next_fire = std::chrono::steady_clock::now()
                                    + time_to_delete_files_from_recycle;
                }
                g_timer_cv.notify_one(); // разбудить таймер

                std::cout << "время изменилось на [" << time_to_delete_files_from_recycle << "]" << std::endl;

            } catch (const std::exception &e) {
                std::cerr << "[ERROR] " << e.what() << std::endl;
                std::cout << "возможно вы не ввели неправильно время, надо писать примерно так (в секундах) - 12" << std::endl;
            }
        } else if (command == "clrec") {
            // очистка корзины
            char choice;
            std::cout << "вы уверены удалить файлы? (Y/N): ";
            std::cin >> choice;
            if (std::tolower(choice) == 'y') {
                HWork_with_recycle::clear_recycle(exten_files_to_delete_from_recycle);
            } else {
                std::cout << "действие отменено!" << std::endl;
            }
        }
        //
        // add
        //
        else if (command.rfind("command ", 0) == 0) {
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
        else if (command.rfind("del ", 0) == 0) {
            if (command.substr(4) == "all") {
                exten_files_to_delete_from_recycle.clear();
                std::cout << "все расширения удалены!" << std::endl;
            }
            else if (helper::check_extension(command.substr(4), exten_files_to_delete_from_recycle)) {
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
            {
                std::lock_guard<std::mutex> lock(g_timer_mutex);
                g_running = false;
            }
            g_timer_cv.notify_one();
            break;
        }

        //
        // else
        //
        else {
            std::cout << "Неизвестная команда, для помощи напишите help" << std::endl;
        }

        std::cout << "$ ";
    }
}


void start_program() {
    std::cout << "Програма запущена" << std::endl;
    std::cout << "каждые " << time_to_delete_files_from_recycle << " корзина будет очищаться (можно изменить время)" <<
            std::endl;
    std::cout << "для помощи напишите help" << std::endl;

    std::thread t_timer(timer_thread);
    std::thread t_console(console_thread);

    t_console.join();
    {
        std::lock_guard<std::mutex> lock(g_timer_mutex);
        g_running = false;
    }
    g_timer_cv.notify_one();
    t_timer.join();
}
