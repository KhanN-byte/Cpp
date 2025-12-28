//
// Created by haris on 12/28/2025.
//

#include "fileStorage.h"
#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<limits>

std::string readMultilineNote(const char* prompt) {
    std::cout << prompt << "\n"
              << "(Type ::end on a new line to finish)\n";

    std::string note, line;
    while (true) {
        std::getline(std::cin, line);
        if (line == "::end") break;

        note += line;
        note += "\n";
    }

    // remove trailing newline (optional)
    if (!note.empty() && note.back() == '\n') note.pop_back();

    return note;
}

int main() {
    std::string filename;

    while (true) {
        std::cout << "\n----- Note Application -----\n"
                  << "1) Add a note\n"
                  << "2) View notes\n"
                  << "3) Clear notes\n"
                  << "4) Exit\n"
                  << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::filesystem::path new_dir_path = "C:\\Users\\haris\\OneDrive\\Desktop\\notes";
            std::filesystem::current_path(new_dir_path);
            std::cout << "Current working directory changed to: " << std::filesystem::current_path() << std::endl;
            std::cout << "Label your file: ";
            std::getline(std::cin, filename);

            std::ofstream out(filename, std::ios::app);
            if (!out) {
                std::cout << "Error opening file.\n";
                continue;
            }

            std::string notes = readMultilineNote("Enter your note:");
            if (!notes.empty()) {
                out << "----- NOTE START -----\n";
                out << notes << "\n";
                out << "----- NOTE END -----\n";
                std::cout << "Saved.\n";
            } else {
                std::cout << "Empty note skipped.\n";
            }

        }
        else if (choice == 2) {
            std::ifstream in(filename);
            if (!in) {
                std::cout << "No notes found.\n";
                continue;
            }

            std::string line;
            std::cout << "\n--- Notes ---\n";
            while (std::getline(in, line)) {
                std::cout << line << "\n";
            }
        }
        else if (choice == 3) {
            std::ofstream out(filename, std::ios::trunc);
            std::cout << "Notes cleared.\n";
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::cout << "Unknown option.\n";
        }
    }

    return 0;
}
