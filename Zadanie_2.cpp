#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <sstream>  // Добавлен для работы с ostringstream

using namespace std;

// Структура для хранения информации о посетителе
struct Visitor {
    string ticket;      // Номер талона
    int duration;      // Продолжительность посещения
};

// Функция для генерации случайного номера талона
string generateTicket() {
    static const char charset[] = "0123456789";
    string ticket = "T";
    for (int i = 0; i < 3; ++i) {
        ticket += charset[rand() % (sizeof(charset) - 1)];
    }
    return ticket;
}

// Функция для распределения очереди по окнам
void distributeQueue(vector<Visitor>& visitors, vector<vector<Visitor>>& windows) {
    // Создаем вектор для хранения суммарного времени каждого окна
    vector<int> windowTimes(windows.size(), 0);
    size_t currentVisitor = 0; // Индекс текущего посетителя

    // Пока есть посетители в очереди
    while (currentVisitor < visitors.size()) {
        Visitor current = visitors[currentVisitor];
        currentVisitor++;

        // Находим окно с минимальным временем
        auto minIt = min_element(windowTimes.begin(), windowTimes.end());
        int windowIndex = distance(windowTimes.begin(), minIt);

        // Добавляем посетителя в это окно
        windows[windowIndex].push_back(current);
        windowTimes[windowIndex] += current.duration;
    }

    // Выводим распределение
    for (size_t i = 0; i < windows.size(); ++i) {
        if (!windows[i].empty()) {
            cout << ">>> Окно " << i + 1 << " ("
                << windowTimes[i] << " минут): ";

            for (size_t j = 0; j < windows[i].size(); ++j) {
                cout << windows[i][j].ticket;
                if (j != windows[i].size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
}

void bags(int mbBags){
    if (mbBags <= 0) {
        cout << "введена невозможная продолжительность посещения или количество окон." << endl;
        exit(0);
    }

}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0)); // Инициализация генератора случайных чисел
    cout << "Команда ENQUEUE - добавить продолжительность приема человека, чтобы получить талон." << endl;
    cout << "Команда DISTRIBUTE - узнать информацию об очереди." << endl;
    int windowsCount;   // Количество окон
    vector<Visitor> visitors;  // Вектор посетителей вместо очереди
    vector<vector<Visitor>> windows;  // Вектор окон с распределенными посетителями

    cout << ">>> Введите кол-во окон" << endl;
    cout << "<<< ";
    cin >> windowsCount;
    bags(windowsCount);

    windows.resize(windowsCount);

    string command;
    while (true) {
        cout << "<<< ";
        cin >> command;

        if (command == "ENQUEUE") {
            int duration;
            cin >> duration;
            bags(duration);

            // Создаем нового посетителя
            Visitor visitor;
            visitor.ticket = generateTicket();
            visitor.duration = duration;

            // Добавляем в конец вектора
            visitors.push_back(visitor);

            cout << ">>> " << visitor.ticket << endl;
        }
        else if (command == "DISTRIBUTE") {
            // Распределяем очередь по окнам
            distributeQueue(visitors, windows);
            break;
        }
        else {
            cout << ">>> Неизвестная команда" << endl;
        }
    }

    return 0;
}
