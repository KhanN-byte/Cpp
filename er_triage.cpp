//
// Created by haris on 12/22/2025.
//

#include "er_triage.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

struct HeapEntry {
    int priority{}, triageScore{}, arrivalMin{}, id{}, version{};

    bool operator<(const HeapEntry& o) const {
        if (priority != o.priority)        return priority < o.priority;
        if (triageScore != o.triageScore)  return triageScore < o.triageScore;
        return arrivalMin > o.arrivalMin;  // earlier arrival first
    }
};

class er_triage::Impl {
public:
    explicit Impl(int agingRatePer10Min) : agingRate(agingRatePer10Min) {}

    int addPatient(const string& name, int age, const string& complaint, int triageScore) {
        Patient p{
                .id = nextId++,
                .name = name,
                .age = age,
                .complaint = complaint,
                .arrivalMin = currentMin,
                .triageScore = triageScore,
                .version = 1
        };
        patients[p.id] = p;
        push(p);
        return p.id;
    }

    void updateTriage(int id, int newScore) {
        auto it = patients.find(id);
        if (it == patients.end()) {
            cout << "[WARN] No patient with id " << id << "\n";
            return;
        }
        auto& p = it->second;
        p.triageScore = newScore;
        ++p.version;
        push(p);

        cout << "[INFO] Re-triage: id=" << id << " newScore=" << newScore << "\n";
    }

    void advanceTime(int minutes) {
        if (minutes <= 0) return;
        currentMin += minutes;
        cout << "[TIME] +" << minutes << "m (minute " << currentMin << ")\n";
        // No heap refresh required; callNext() recomputes & re-pushes if stale.
    }

    void showLobby() const {
        if (patients.empty()) {
            cout << "\n=== ER Lobby (minute " << currentMin << ") ===\n"
                 << "[INFO] Lobby is empty.\n\n";
            return;
        }

        // Average minutes per patient seen (tweak to whatever you want)
        constexpr int AVG_MIN_PER_PATIENT = 15;

        vector<int> ids;
        ids.reserve(patients.size());
        for (const auto& [id, _] : patients) ids.push_back(id);

        auto pr = [&](int id) { return priority(patients.at(id)); };

        // Sort by: priority desc, triage desc, arrival asc, id asc
        sort(ids.begin(), ids.end(), [&](int a, int b) {
            const auto& pa = patients.at(a);
            const auto& pb = patients.at(b);

            int pra = pr(a), prb = pr(b);
            if (pra != prb) return pra > prb;
            if (pa.triageScore != pb.triageScore) return pa.triageScore > pb.triageScore;
            if (pa.arrivalMin != pb.arrivalMin) return pa.arrivalMin < pb.arrivalMin;
            return pa.id < pb.id;
        });

        cout << "\n=== ER Lobby (minute " << currentMin << ") ===\n"
             << left
             << setw(5)  << "Rank"
             << setw(4)  << "ID"
             << setw(16) << "Name"
             << setw(6)  << "Age"
             << setw(8)  << "Triage"
             << setw(6)  << "Wait"
             << setw(9)  << "Priority"
             << setw(6)  << "Ahead"
             << setw(8)  << "EstWait"
             << "Complaint\n"
             << string(110, '-') << "\n";

        // Print with position-based waiting estimate
        for (size_t i = 0; i < ids.size(); ++i) {
            const auto& p = patients.at(ids[i]);
            int waitSoFar = max(0, currentMin - p.arrivalMin);
            int ahead = static_cast<int>(i);                 // number of patients ahead
            int estAdditionalWait = ahead * AVG_MIN_PER_PATIENT;

            cout << left
                 << setw(5)  << (i + 1)
                 << setw(4)  << p.id
                 << setw(16) << p.name
                 << setw(6)  << p.age
                 << setw(8)  << p.triageScore
                 << setw(6)  << waitSoFar
                 << setw(9)  << priority(p)
                 << setw(6)  << ahead
                 << setw(8)  << estAdditionalWait
                 << p.complaint << "\n";
        }

        // Extra clarity line: who is currently "next"
        const auto& nextP = patients.at(ids.front());
        cout << "\n[NEXT UP] " << nextP.name << " (id=" << nextP.id
             << ", triage=" << nextP.triageScore
             << ", complaint=\"" << nextP.complaint << "\")\n\n";
    }



    void callNext() {
        while (!pq.empty()) {
            auto top = pq.top(); pq.pop();

            auto it = patients.find(top.id);
            if (it == patients.end()) continue;

            const auto& p = it->second;

            // stale due to re-triage
            if (top.version != p.version) continue;

            // stale due to time passing -> refresh once and retry
            int nowPr = priority(p);
            if (top.priority != nowPr) { push(p); continue; }

            cout << "\n🚑 [NEXT] " << p.name
                 << " (id=" << p.id
                 << ", triage=" << p.triageScore
                 << ", wait=" << (currentMin - p.arrivalMin) << "m"
                 << ", complaint=\"" << p.complaint << "\")\n\n";

            patients.erase(p.id);
            return;
        }
        cout << "[INFO] Lobby is empty.\n";
    }

private:
    int currentMin{0}, nextId{1}, agingRate{1};
    unordered_map<int, Patient> patients;
    priority_queue<HeapEntry> pq;

    int priority(const Patient& p) const {
        int wait = currentMin - p.arrivalMin;
        return p.triageScore + (wait / 10) * agingRate;
    }

    void push(const Patient& p) {
        pq.push(HeapEntry{
                .priority = priority(p),
                .triageScore = p.triageScore,
                .arrivalMin = p.arrivalMin,
                .id = p.id,
                .version = p.version
        });
    }
};

// ---- er_triage public wrapper ----

er_triage::er_triage(int agingRatePer10Min)
        : impl(make_unique<Impl>(agingRatePer10Min)) {}

er_triage::~er_triage() = default;

int er_triage::addPatient(const string& name, int age, const string& complaint, int triageScore) {
    return impl->addPatient(name, age, complaint, triageScore);
}

void er_triage::updateTriage(int id, int newScore) {
    impl->updateTriage(id, newScore);
}

void er_triage::advanceTime(int minutes) {
    impl->advanceTime(minutes);
}

void er_triage::showLobby() const {
    impl->showLobby();
}

void er_triage::callNext() {
    impl->callNext();
}
