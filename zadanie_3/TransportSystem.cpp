#include "TransportSystem.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Структуры для хранения данных
map<string, vector<string>> trolleyRoutes;  // троллейбус -> остановки
map<string, set<string>> stopToTrolleys;    // остановка -> троллейбусы

Command parseCommand(const string& input) {
    if (input == "CREATE_TRL") return Command::CREATE_TRL;
    if (input == "TRL_IN_STOP") return Command::TRL_IN_STOP;
    if (input == "STOPS_IN_TRL") return Command::STOPS_IN_TRL;
    if (input == "SHOW_ALL") return Command::SHOW_ALL;
    if (input == "EXIT") return Command::EXIT;
    return Command::UNKNOWN;
}

void createTrolley(const string& name, const vector<string>& stops) {
    setlocale(LC_ALL, "Russian");
    trolleyRoutes[name] = stops;
    for (const auto& stop : stops) {
        stopToTrolleys[stop].insert(name);
    }
    cout << "Троллейбус " << name << " создан с " << stops.size() << " остановками" << endl;
}

void showTrolleysForStop(const string& stop) {
    setlocale(LC_ALL, "Russian");
    if (stopToTrolleys.find(stop) != stopToTrolleys.end()) {
        cout << "Через остановку " << stop << " проходят: ";
        for (const auto& trl : stopToTrolleys[stop]) {
            cout << trl << " ";
        }
        cout << endl;
    }
    else {
        cout << "Остановка " << stop << " не найдена" << endl;
    }
}

void showStopsForTrolley(const string& trolley) {
    setlocale(LC_ALL, "Russian");
    if (trolleyRoutes.find(trolley) != trolleyRoutes.end()) {
        cout << "Троллейбус " << trolley << " проходит через:" << endl;
        for (const auto& stop : trolleyRoutes[trolley]) {
            cout << stop << " (другие троллейбусы: ";
            for (const auto& other : stopToTrolleys[stop]) {
                if (other != trolley) cout << other << " ";
            }
            cout << ")" << endl;
        }
    }
    else {
        cout << "Троллейбус " << trolley << " не найден" << endl;
    }
}

void showAllTrolleys() {
    setlocale(LC_ALL, "Russian");
    cout << "Все троллейбусы:" << endl;
    for (const auto& route : trolleyRoutes) {
        const string& trolleyName = route.first;
        const vector<string>& stops = route.second;

        cout << trolleyName << ": ";
        for (size_t i = 0; i < stops.size(); ++i) {
            cout << stops[i];
            if (i != stops.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void processCommand(Command cmd) {
    setlocale(LC_ALL, "Russian");
    string input;
    vector<string> stops;

    switch (cmd) {
    case Command::CREATE_TRL: {
        cout << "Введите название троллейбуса: ";
        cin >> input;
        cout << "Введите количество остановок: ";
        int count;
        cin >> count;
        stops.resize(count);
        cout << "Введите остановки через пробел: ";
        for (int i = 0; i < count; ++i) {
            cin >> stops[i];
        }
        createTrolley(input, stops);
        break;
    }
    case Command::TRL_IN_STOP: {
        cout << "Введите название остановки: ";
        cin >> input;
        showTrolleysForStop(input);
        break;
    }
    case Command::STOPS_IN_TRL: {
        cout << "Введите название троллейбуса: ";
        cin >> input;
        showStopsForTrolley(input);
        break;
    }
    case Command::SHOW_ALL: {
        showAllTrolleys();
        break;
    }
    case Command::EXIT: {
        exit(0);
    }
    case Command::UNKNOWN: {
        cout << "Неизвестная команда" << endl;
        break;
    }
    }
}
