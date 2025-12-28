//
// Created by haris on 12/23/2025.
//

#include "struct.h"

#include <iostream>
using namespace std;

struct SensorData {
    float temperature;   // °C
    float voltage;       // V
    bool error;          // fault flag
};

SensorData readSensor() {
    SensorData data;

    // Simulated sensor values
    data.temperature = 36.5;
    data.voltage = 2.9;

    // Error condition
    data.error = (data.voltage < 3.0);

    return data;
}

void printStatus(const SensorData& data) {
    cout << "Temperature: " << data.temperature << " C" << endl;
    cout << "Voltage: " << data.voltage << " V" << endl;

    if (data.error) {
        cout << "⚠ ERROR: Voltage too low!" << endl;
    } else {
        cout << "Status: OK" << endl;
    }
}
int main() {

    SensorData hdd = readSensor();
    printStatus(hdd);
}

