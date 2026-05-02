#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ios>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <unistd.h>


std::size_t get_index(const std::vector<std::string>& strings) {
    std::size_t idx;
    std::cout << "Enter index:" << std::endl;
    std::cin >> idx;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (idx < strings.size() && idx < 42) {
        return idx;
    }
    std::cout << "You're playing with fire..." << std::endl;
    exit(42);
}

void do_add(std::vector<std::string>& strings) {
    strings.emplace_back();
}

void do_getline(std::vector<std::string>& strings) {
    std::size_t idx = get_index(strings);
    std::cout << "String content:" << std::endl;
    std::getline(std::cin, strings[idx]);
}

void do_cout(const std::vector<std::string>& strings) {
    std::size_t idx = get_index(strings);
    std::cout << strings[idx] << std::endl;
}

void do_edit(const std::vector<std::string>& strings) {
    std::size_t idx = get_index(strings);
    std::cout << "String content:" << std::endl;
    read(0, (char*)&strings[idx], 0x20); // Strings are a char* anyway, so why not cast them
}

void challenge() {
    std::vector<std::string> strings;
    std::string option;
    while (true) {
        std::cout << "add/getline/cout/edit\n"
                  << ">> " << std::flush;
        std::getline(std::cin, option);
        if (option == "add") {
            do_add(strings);
        }
        else if (option == "getline") {
            do_getline(strings);
        }
        else if (option == "cout") {
            do_cout(strings);
        }
        else if (option == "edit") {
            do_edit(strings);
        }
        else return;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Welcome to C++ Exploitation!\n";
    std::cout << "In this challenge, you will learn how to abuse std::string to gain arbitrary read and write for virtually zero effort.\n";
    challenge();

    return 0;
}
