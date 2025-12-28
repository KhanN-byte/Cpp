//
// Created by haris on 12/22/2025.
//

#ifndef C___ER_TRIAGE_H
#define C___ER_TRIAGE_H

#include <memory>
#include <string>

struct Patient {
    int id{};
    std::string name;
    int age{};
    std::string complaint;
    int arrivalMin{};
    int triageScore{};
    int version{};
};

class er_triage {
public:
    explicit er_triage(int agingRatePer10Min = 1);
    ~er_triage();

    int addPatient(const std::string& name,
                   int age,
                   const std::string& complaint,
                   int triageScore);

    void updateTriage(int id, int newScore);
    void advanceTime(int minutes);
    void showLobby() const;
    void callNext();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};


#endif //C___ER_TRIAGE_H
