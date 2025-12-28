#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>   // for remove(), rename()

int main() {
    const char* filename = "slackadmin.txt";
    const char* tempFile = "slackadmin_temp.txt";

    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cout << "Could not open input file.\n";
        return 1;
    }

    std::ofstream output(tempFile);
    if (!output.is_open()) {
        std::cout << "Could not create temp file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(input, line)) {
        output << line << "\n\n";   // add spacing here
    }

    input.close();
    output.close();

    // Replace original file
    std::remove(filename);
    std::rename(tempFile, filename);

    std::cout << "File formatted successfully (in-place).\n";
    return 0;
}
