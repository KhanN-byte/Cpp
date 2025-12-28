//
// Created by haris on 12/24/2025.
//

#include "log_analyzer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>
#include <filesystem>

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: log_analyzer <logfile>\n";
        return 1;
    }

    const std::string path = argv[1];

    std::ifstream logs(path, std::ios::in);
    if (!logs.is_open()) {
        std::cerr << "ERROR OPENING FILE: " << path << "\n";
        return 1;
    }

    long long countDEBUG = 0;
    long long countINFO = 0;
    long long countWARNING = 0;
    long long countERROR = 0;
    long long countCRITICAL = 0;
    long long countUNKNOWN = 0;

    auto classifyAndCount = [&](const std::string& rawLine) {
        std::string lower = toLower(rawLine);

        if (lower.find("critical") != std::string::npos)      countCRITICAL++;
        else if (lower.find("error") != std::string::npos)    countERROR++;
        else if (lower.find("warning") != std::string::npos)  countWARNING++;
        else if (lower.find("info") != std::string::npos)     countINFO++;
        else if (lower.find("debug") != std::string::npos)    countDEBUG++;
        else                                                  countUNKNOWN++;
    };

    auto printSummary = [&]() {
        std::cout << "\n===== Live Log Summary =====\n"
                  << "DEBUG:    " << countDEBUG << "\n"
                  << "INFO:     " << countINFO << "\n"
                  << "WARNING:  " << countWARNING << "\n"
                  << "ERROR:    " << countERROR << "\n"
                  << "CRITICAL: " << countCRITICAL << "\n"
                  << "UNKNOWN:  " << countUNKNOWN << "\n"
                  << "============================\n";
    };

    // 1) BASELINE: read the whole file first
    std::string line;
    while (std::getline(logs, line)) {
        classifyAndCount(line);
    }

    // We are now at EOF; clear state so future reads work
    logs.clear();

    // Position where "new" content begins
    logs.seekg(0, std::ios::end);

    printSummary();

    using clock = std::chrono::steady_clock;
    auto lastPrint = clock::now();
    auto lastActivity = clock::now();                 // when we last saw a NEW line appended
    constexpr auto PRINT_INTERVAL = std::chrono::seconds(2);
    constexpr auto IDLE_TIMEOUT   = std::chrono::minutes(1);

    // 2) FOLLOW: keep reading new lines appended to the file
    while (true) {
        bool readAny = false;

        while (std::getline(logs, line)) {
            readAny = true;
            lastActivity = clock::now();              // reset idle timer on new line
            classifyAndCount(line);
        }

        // If we hit EOF, clear flags so we can read new data later
        if (logs.eof()) {
            logs.clear();
        }

        // Handle truncation/rotation: if file size shrank, reopen and re-baseline.
        try {
            auto pos = logs.tellg();
            if (pos != std::streampos(-1)) {
                auto fileSize = std::filesystem::file_size(path);
                if (static_cast<std::uintmax_t>(pos) > fileSize) {
                    logs.close();
                    logs.open(path, std::ios::in);

                    if (!logs.is_open()) {
                        std::cerr << "Lost access to file; retrying...\n";
                    } else {
                        // Reset counts and re-read baseline from scratch
                        countDEBUG = countINFO = countWARNING = countERROR = countCRITICAL = countUNKNOWN = 0;

                        while (std::getline(logs, line)) {
                            classifyAndCount(line);
                        }

                        logs.clear();
                        logs.seekg(0, std::ios::end);

                        printSummary();
                        lastPrint = clock::now();
                        lastActivity = clock::now();
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error&) {
            // File might be temporarily unavailable; ignore and keep trying
        }

        // Print summary every 2 seconds
        auto now = clock::now();
        if (now - lastPrint >= PRINT_INTERVAL) {
            printSummary();
            lastPrint = now;
        }

        // Exit if no new log lines have been appended for 1 minute
        if (now - lastActivity >= IDLE_TIMEOUT) {
            std::cout << "\nNo log activity for 60 seconds. Exiting...\n";
            printSummary();
            break;
        }

        if (!readAny) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    logs.close();
    return 0;
}


