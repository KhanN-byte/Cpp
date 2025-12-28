//
// Created by haris on 12/22/2025.
//

#include "er_triage.h"

#include <iostream>
#include <limits>
#include <string>

using namespace std;

static int readInt(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        int x;
        if (cin >> x && x >= minVal && x <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid input. Enter a number between " << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

static void menu() {
    cout << "\n===== ER TRIAGE CLI =====\n"
         << "1) Add patient (arrival)\n"
         << "2) Update triage score (re-triage)\n"
         << "3) Call next patient (treat)\n"
         << "4) Show lobby\n"
         << "5) Advance time (minutes)\n"
         << "6) Quick simulate: 'critical' patient arrives\n"
         << "0) Exit\n";
}

int main() {
    cout << "ER Triage System\n";
    int agingRate = readInt("Set aging rate per 10 minutes (0-10 recommended): ", 0, 50);

    er_triage er(agingRate);

    while (true) {
        menu();
        int choice = readInt("Choose option: ", 0, 6);

        if (choice == 0) {
            cout << "Goodbye.\n";
            break;
        }

        if (choice == 1) {
            string name = readLine("Patient name: ");
            int age = readInt("Age (0-120): ", 0, 120);
            string complaint = readLine("Complaint: ");
            int triage = readInt("Triage score (0-100, higher = more urgent): ", 0, 100);

            int id = er.addPatient(name, age, complaint, triage);
            cout << "[OK] Added patient id=" << id << "\n";
            er.showLobby();
        }
        else if (choice == 2) {
            int id = readInt("Patient id to update: ", 1, 1'000'000);
            int triage = readInt("New triage score (0-100): ", 0, 100);
            er.updateTriage(id, triage);
            er.showLobby();
        }
        else if (choice == 3) {
            er.callNext();
            er.showLobby();
        }
        else if (choice == 4) {
            er.showLobby();
        }
        else if (choice == 5) {
            int minutes = readInt("Advance time by minutes (1-1440): ", 1, 1440);
            er.advanceTime(minutes);
            er.showLobby();
        }
        else if (choice == 6) {
            // This is your “someone critical arrives and cuts the line” demo
            cout << "\n[SIM] Critical patient arriving...\n";
            string name = readLine("Critical patient name: ");
            int age = readInt("Age (0-120): ", 0, 120);
            string complaint = readLine("Complaint (e.g., Chest pain, unresponsive): ");
            int triage = readInt("Triage score (80-100 recommended): ", 0, 100);

            int id = er.addPatient(name, age, complaint, triage);
            cout << "[ALERT] Critical patient added id=" << id << " — should jump near the top.\n";
            er.showLobby();
        }
    }

    return 0;
}
