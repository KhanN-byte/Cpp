//
// Created by haris on 12/22/2025.
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

static wstring toWString(const string& s) {
    if (s.empty()) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    wstring out(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), out.data(), len);
    return out;
}

static string actionToString(DWORD action) {
    switch (action) {
        case FILE_ACTION_ADDED: return "CREATED";
        case FILE_ACTION_REMOVED: return "DELETED";
        case FILE_ACTION_MODIFIED: return "MODIFIED";
        case FILE_ACTION_RENAMED_OLD_NAME: return "RENAMED_FROM";
        case FILE_ACTION_RENAMED_NEW_NAME: return "RENAMED_TO";
        default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: intrusion_watch.exe <folder_to_watch>\n";
        cout << "Example: intrusion_watch.exe C:\\Users\\haris\\Desktop\\Confidential\n";
        return 1;
    }

    wstring folder = toWString(argv[1]);

    HANDLE hDir = CreateFileW(
            folder.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            nullptr
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        cout << "[ERROR] Failed to open directory. Check path and permissions.\n";
        return 1;
    }

    cout << "[OK] Watching: " << argv[1] << "\n";
    cout << "Press Ctrl+C to stop.\n\n";

    // Simple "burst" detector: if a file is modified too often in a short window, alert.
    unordered_map<wstring, pair<int, chrono::steady_clock::time_point>> burst;

    constexpr DWORD BUF_SIZE = 16 * 1024;
    alignas(4) char buffer[BUF_SIZE];
    DWORD bytesReturned = 0;

    while (true) {
        BOOL ok = ReadDirectoryChangesW(
                hDir,
                buffer,
                BUF_SIZE,
                TRUE, // watch subdirectories
                FILE_NOTIFY_CHANGE_FILE_NAME |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_LAST_WRITE |
                FILE_NOTIFY_CHANGE_SIZE |,
                &bytesReturned,
                nullptr,
                nullptr
        );

        if (!ok) {
            cout << "[ERROR] ReadDirectoryChangesW failed.\n";
            break;
        }

        char* base = buffer;
        while (true) {
            auto* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(base);

            wstring filename(info->FileName, info->FileNameLength / sizeof(WCHAR));
            DWORD action = info->Action;

            // Print event (alert-style)
            wcout << L"[ALERT] " << toWString(actionToString(action))
                  << L"  " << filename << L"\n";

            // Burst heuristic: many modifications quickly
            if (action == FILE_ACTION_MODIFIED) {
                auto now = chrono::steady_clock::now();
                auto& entry = burst[filename];

                // Reset window if > 10 seconds old
                if (entry.second.time_since_epoch().count() == 0 ||
                    chrono::duration_cast<chrono::seconds>(now - entry.second).count() > 10) {
                    entry = {1, now};
                } else {
                    entry.first++;
                }

                if (entry.first >= 6) {
                    wcout << L"[!!!] Suspicious: rapid modifications to " << filename
                          << L" (" << entry.first << L" changes in ~10s)\n";
                }
            }

            if (info->NextEntryOffset == 0) break;
            base += info->NextEntryOffset;
        }

        wcout << flush;
    }

    CloseHandle(hDir);
    return 0;
}
