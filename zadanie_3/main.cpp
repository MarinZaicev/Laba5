#include "TransportSystem.h"
#include <iostream>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Система управления троллейбусами" << endl;
    cout << "Доступные команды:" << endl;
    cout << "CREATE_TRL - создать троллейбус" << endl;
    cout << "TRL_IN_STOP - троллейбусы через остановку" << endl;
    cout << "STOPS_IN_TRL - остановки троллейбуса" << endl;
    cout << "SHOW_ALL - показать все троллейбусы" << endl;
    cout << "EXIT - выход" << endl;

    while (true) {
        cout << "> ";
        string command;
        cin >> command;

        Command cmd = parseCommand(command);
        processCommand(cmd);
    }

    return 0;
}
