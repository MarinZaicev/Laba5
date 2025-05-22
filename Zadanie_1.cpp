#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_ITEMS_PER_CELL = 10; // Максимальное количество товара в ячейке
const int TOTAL_WAREHOUSE_CAPACITY = 50400; // Общая вместимость склада

// Структура для хранения содержимого ячейки
struct CellContent {
    map<string, int> items; // Наименование товара - количество
    int total = 0;          // Общее количество товаров в ячейке
};

// структура хранения данных склада
map<string, CellContent> warehouse; // Ключ - адрес ячейки

// Глобальная переменная для отслеживания общего количества товаров на складе
int totalWarehouseItems = 0;

/*
  Проверяет корректность адреса ячейки
  address - адрес в формате "ZRSSHH" (Зона, Стеллаж, Секция, Полка)
  true если адрес корректен
*/
bool isValidAddress(const string& address) {
    if (address.length() != 6) return false;

    char zone = address[0];
    string rackStr = address.substr(1, 2);
    char section = address[3];
    string shelf = address.substr(4, 5);

    // Проверка зоны (A, B или C)
    if (zone != 'A' && zone != 'B' && zone != 'C') return false;

    // Проверка стеллажа (01-14)
    try {
        int rack = stoi(rackStr);
        if (rack < 1 || rack > 14) return false;
    }
    catch (...) {
        return false;
    }

    // Проверка секции (0-6)
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

    // Проверка общей вместимости склада
    if (totalWarehouseItems + count > TOTAL_WAREHOUSE_CAPACITY) {
        cout << "Нельзя добавить товар. Склад переполнен (максимум " << TOTAL_WAREHOUSE_CAPACITY << " единиц)." << endl;
        return;
    }

    CellContent& cell = warehouse[address];

    if (cell.total + count > MAX_ITEMS_PER_CELL) {
        cout << "Нельзя добавить товар. Ячейка переполнена (максимум " << MAX_ITEMS_PER_CELL << " единиц)." << endl;
        return;
    }

    cell.items[item] += count;
    cell.total += count;
    totalWarehouseItems += count;
    cout << "Товар '" << item << "' добавлен в ячейку " << address << ". Всего на складе: " << totalWarehouseItems << "/" << TOTAL_WAREHOUSE_CAPACITY << endl;
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
    totalWarehouseItems -= count;

    if (cell.items[item] == 0) {
        cell.items.erase(item);
    }

    cout << "Товар '" << item << "' удален из ячейки " << address << ". Всего на складе: " << totalWarehouseItems << "/" << TOTAL_WAREHOUSE_CAPACITY << endl;
}

/*
  Функция вывода статистики по складу
 */
void info() {
    cout << "=== СТАТИСТИКА СКЛАДА ===" << endl;
    cout << "Общая вместимость склада: " << TOTAL_WAREHOUSE_CAPACITY << " единиц" << endl;
    cout << "Текущее количество товаров: " << totalWarehouseItems << " единиц" << endl;
    cout << "Свободная емкость: " << (TOTAL_WAREHOUSE_CAPACITY - totalWarehouseItems) << " единиц" << endl;

    if (warehouse.empty()) {
        cout << "\nСклад полностью пуст." << endl;
        return;
    }

    int zoneItems[3] = { 0 }; // 0 - зона A, 1 - зона B, 2 - зона C
    int totalCells = 3 * 14 * 7 * 20; // Всего ячеек на складе (3 зоны × 14 стеллажей × 7 секций × 20 полок)
    int usedCells = 0;

    // Подсчет статистики
    for (const auto& entry : warehouse) {
        usedCells++;
        if (entry.first[0] == 'A') {
            zoneItems[0] += entry.second.total;
        }
        else if (entry.first[0] == 'B') {
            zoneItems[1] += entry.second.total;
        }
        else {
            zoneItems[2] += entry.second.total;
        }
    }

    // Вывод общей статистики
    float totalPercent = (static_cast<float>(totalWarehouseItems) / TOTAL_WAREHOUSE_CAPACITY) * 100;
    cout << "Общая загруженность склада: " << fixed << setprecision(2) << totalPercent << "%" << endl;

    // Статистика по зонам
    int zoneCapacity = TOTAL_WAREHOUSE_CAPACITY / 3; // Предполагаем равное распределение
    float zoneAPercent = (static_cast<float>(zoneItems[0]) / zoneCapacity) * 100;
    float zoneBPercent = (static_cast<float>(zoneItems[1]) / zoneCapacity) * 100;
    float zoneCPercent = (static_cast<float>(zoneItems[2]) / zoneCapacity) * 100;
    cout << "Зона A: " << zoneAPercent << "% (" << zoneItems[0] << "/" << zoneCapacity << ")" << endl;
    cout << "Зона B: " << zoneBPercent << "% (" << zoneItems[1] << "/" << zoneCapacity << ")" << endl;
    cout << "Зона C: " << zoneCPercent << "% (" << zoneItems[2] << "/" << zoneCapacity << ")" << endl;

    // Список занятых ячеек
    cout << "Занятые ячейки (" << usedCells << " из " << totalCells << "):" << endl;
    for (const auto& entry : warehouse) {
        if (entry.second.total > 0) {
            cout << entry.first << ": " << entry.second.total << " ед. (";
            for (const auto& item : entry.second.items) {
                cout << item.first << " - " << item.second << ", ";
            }
            cout << "\b\b)" << endl;
        }
    }

    // Список пустых ячеек
    cout << "Пустых ячеек: " << (totalCells - usedCells) << endl;
}

int main() {
    string command, address, item;
    int count;
    setlocale(LC_ALL, "Russian");
    cout << "Система управления складом (версия с map)" << endl;
    cout << "Максимальная вместимость склада: " << TOTAL_WAREHOUSE_CAPACITY << " единиц" << endl;
    cout << "Формат адреса: ZRRSSH, где:" << endl;
    cout << "  Z - зона (A, B или C)" << endl;
    cout << "  RR - номер стеллажа (01-14)" << endl;
    cout << "  S - секция (0-6)" << endl;
    cout << "  H - полка (01-20)" << endl;
    cout << "Пример адреса: A010101 (Зона A, стеллаж 01, секция 0, полка 01)" << endl << endl;

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
