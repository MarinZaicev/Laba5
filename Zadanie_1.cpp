#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_ITEMS = 10; // Максимальное количество товара в ячейке

// Структура для хранения содержимого ячейки
struct CellContent {
    map<string, int> items; // Наименование товара - количество
    int total = 0;          // Общее количество товаров в ячейке
};

// Основная структура хранения данных склада
map<string, CellContent> warehouse; // Ключ - адрес ячейки (например "A0101")

/*
  Проверяет корректность адреса ячейки
  address - адрес в формате "ZRSSHH" (Зона, Стеллаж, Секция, Полка)
  true если адрес корректен, false в противном случае
 */
bool isValidAddress(const string& address) {
    if (address.length() != 6) return false;

    char zone = address[0];
    string rackStr = address.substr(1, 2);
    char section = address[3];
    string shelf = address.substr(4, 5);

    // Проверка зоны (A или B)
    if (zone != 'A' && zone != 'B' && zone != 'C') return false;

    // Проверка стеллажа (01-06)
    try {
        int rack = stoi(rackStr);
        if (rack < 1 || rack > 14) return false;
    }
    catch (...) {
        return false;
    }

    // Проверка секции (0-3)
    if (section < '0' || section > '6') return false;

    try {
        int shelfs = stoi(shelf);
        if (shelfs < 1 || shelfs > 20) return false;
    }
    catch (...) {
        return false;
    }

    return true;
}

/*
  Функция добавления товара на склад
  address - адрес ячейки
  item - наименование товара
  count - количество товара для добавления
 */
void add(string address, string item, int count) {
    if (count <= 0) {
        cout << "Количество должно быть положительным." << endl;
        return;
    }

    if (!isValidAddress(address)) {
        cout << "Некорректный адрес." << endl;
        return;
    }

    CellContent& cell = warehouse[address];

    if (cell.total + count > MAX_ITEMS) {
        cout << "Нельзя добавить товар. Ячейка переполнена." << endl;
        return;
    }

    cell.items[item] += count;
    cell.total += count;
    cout << "Товар '" << item << "' добавлен в ячейку " << address << "." << endl;
}

/*
  Функция удаления товара со склада
  address - адрес ячейки
  item - наименование товара
  count - количество товара для удаления
 */
void remove(string address, string item, int count) {
    if (count <= 0) {
        cout << "Количество должно быть положительным." << endl;
        return;
    }

    if (!isValidAddress(address)) {
        cout << "Некорректный адрес." << endl;
        return;
    }

    if (warehouse.find(address) == warehouse.end()) {
        cout << "Ячейка " << address << " пуста." << endl;
        return;
    }

    CellContent& cell = warehouse[address];

    if (cell.items.find(item) == cell.items.end() || cell.items[item] < count) {
        cout << "Недостаточно товара '" << item << "' для удаления." << endl;
        return;
    }

    cell.items[item] -= count;
    cell.total -= count;

    if (cell.items[item] == 0) {
        cell.items.erase(item);
    }

    cout << "Товар '" << item << "' удален из ячейки " << address << "." << endl;
}

/**
 * Функция вывода статистики по складу
 */
void info() {
    if (warehouse.empty()) {
        cout << "Склад полностью пуст." << endl;
        return;
    }

    int totalItems = 0;
    int zoneItems[3] = { 0 }; // 0 - зона A, 1 - зона B
    int totalCells = 3 * 6 * 4 * 4; // Всего ячеек на складе
    int usedCells = 0;

    // Подсчет статистики
    for (const auto& entry : warehouse) {
        totalItems += entry.second.total;
        usedCells++;

        if (entry.first[0] == 'A') {
            zoneItems[0] += entry.second.total;
        }
        else if(entry.first[1] == 'B') {
            zoneItems[1] += entry.second.total;
        }
        else {
            zoneItems[2] += entry.second.total;
        }
    }

    // Вывод общей статистики
    float totalPercent = (static_cast<float>(totalItems) / (totalCells * MAX_ITEMS)) * 100;
    cout << "Общая загруженность склада: " << fixed << setprecision(2) << totalPercent << "%" << endl;

    // Статистика по зонам
    float zoneAPercent = (static_cast<float>(zoneItems[0]) / (6 * 4 * 4 * MAX_ITEMS)) * 100;
    float zoneBPercent = (static_cast<float>(zoneItems[1]) / (6 * 4 * 4 * MAX_ITEMS)) * 100;
    float zoneCPercent = (static_cast<float>(zoneItems[2]) / (6 * 4 * 4 * MAX_ITEMS)) * 100;
    cout << "Зона A: " << zoneAPercent << "%" << endl;
    cout << "Зона B: " << zoneBPercent << "%" << endl;
    cout << "Зона C: " << zoneCPercent << "%" << endl;

    // Список занятых ячеек
    cout << "\nЗанятые ячейки:" << endl;
    for (const auto& entry : warehouse) {
        if (entry.second.total > 0) {
            cout << entry.first << ": " << entry.second.total << " единиц (";
            for (const auto& item : entry.second.items) {
                cout << item.first << " - " << item.second << ", ";
            }
            cout << "\b\b)" << endl;
        }
    }

    // Список пустых ячеек
    cout << "\nПустые ячейки:" << endl;
    vector<string> emptyCells;

    // Генерация всех возможных адресов без ostringstream

    for (char zone : {'A', 'B', 'C'}) {
        for (int rack = 1; rack <= 6; ++rack) {
            for (char section = '0'; section <= '3'; ++section) {
                for (int shelf = 0; shelf <= 20; ++shelf) {
                    // Формируем адрес с учетом двухзначных номеров полок
                    string address;
                    address += zone;
                    address += (rack < 10) ? "0" + to_string(rack) : to_string(rack);
                    address += section;
                    address += (shelf < 10) ? "0" + to_string(shelf) : to_string(shelf);

                    if (warehouse.find(address) == warehouse.end() || warehouse[address].total == 0) {
                        emptyCells.push_back(address);
                    }
                }
            }
        }
    }

    // Вывод пустых ячеек
    cout << "\nПустые ячейки:" << endl;
    const int itemsPerLine = 5; // Меньше, так как адреса теперь длиннее
    for (size_t i = 0; i < emptyCells.size(); ++i) {
        cout << emptyCells[i];
        if (i != emptyCells.size() - 1) {
            if ((i + 1) % itemsPerLine == 0) cout << endl;
            else cout << ", ";
        }
    }
    cout << endl;
}

int main() {
    string command, address, item;
    int count;
    setlocale(LC_ALL, "Russian");
    cout << "Система управления складом (версия с map)" << endl;
    cout << "Формат адреса: ZRSSH, где:" << endl;
    cout << "  Z - зона (A или B)" << endl;
    cout << "  RR - номер стеллажа (01-06)" << endl;
    cout << "  S - секция (0-3)" << endl;
    cout << "  H - полка (0-3)" << endl;
    cout << "Пример адреса: A0101 (Зона A, стеллаж 1, секция 0, полка 1)" << endl << endl;

    while (true) {
        cout << "\nВведите команду (ADD, REMOVE, INFO, EXIT): ";
        cin >> command;

        if (command == "ADD") {
            cout << "Введите адрес ячейки: ";
            cin >> address;
            cout << "Введите наименование товара: ";
            cin >> item;
            cout << "Введите количество товара: ";
            cin >> count;
            add(address, item, count);
        }
        else if (command == "REMOVE") {
            cout << "Введите адрес ячейки: ";
            cin >> address;
            cout << "Введите наименование товара: ";
            cin >> item;
            cout << "Введите количество товара: ";
            cin >> count;
            remove(address, item, count);
        }
        else if (command == "INFO") {
            info();
        }
        else if (command == "EXIT") {
            break;
        }
        else {
            cout << "Неизвестная команда. Доступные команды: ADD, REMOVE, INFO, EXIT" << endl;
        }
    }

    return 0;
}
