using System;
using System.Collections.Generic;
using System.Globalization;

class WarehouseSystem
{
    const int MAX_ITEMS_PER_CELL = 10;
    const int TOTAL_WAREHOUSE_CAPACITY = 50400;

    class CellContent
    {
        public Dictionary<string, int> items = new Dictionary<string, int>();
        public int total = 0;
    }

    static Dictionary<string, CellContent> warehouse = new Dictionary<string, CellContent>();
    static int totalWarehouseItems = 0;

    static bool IsValidAddress(string address)
    {
        if (address.Length != 6) return false;

        char zone = address[0];
        string rackStr = address.Substring(1, 2);
        char section = address[3];
        string shelf = address.Substring(4, 2);

        // Проверка зоны (A, B или C)
        if (zone != 'A' && zone != 'B' && zone != 'C') return false;

        // Проверка стеллажа (01-14)
        if (!int.TryParse(rackStr, out int rack) || rack < 1 || rack > 14)
            return false;

        // Проверка секции (0-6)
        if (section < '0' || section > '6') return false;

        // Проверка полки (01-20)
        if (!int.TryParse(shelf, out int shelfs) || shelfs < 1 || shelfs > 20)
            return false;

        return true;
    }

    static void AddItem(string address, string item, int count)
    {
        if (count <= 0)
        {
            Console.WriteLine("Количество должно быть положительным.");
            return;
        }

        if (!IsValidAddress(address))
        {
            Console.WriteLine("Некорректный адрес.");
            return;
        }

        if (totalWarehouseItems + count > TOTAL_WAREHOUSE_CAPACITY)
        {
            Console.WriteLine($"Нельзя добавить товар. Склад переполнен (максимум {TOTAL_WAREHOUSE_CAPACITY} единиц).");
            return;
        }

        if (!warehouse.TryGetValue(address, out CellContent cell))
        {
            cell = new CellContent();
            warehouse[address] = cell;
        }

        if (cell.total + count > MAX_ITEMS_PER_CELL)
        {
            Console.WriteLine($"Нельзя добавить товар. Ячейка переполнена (максимум {MAX_ITEMS_PER_CELL} единиц).");
            return;
        }

        if (cell.items.ContainsKey(item))
            cell.items[item] += count;
        else
            cell.items[item] = count;

        cell.total += count;
        totalWarehouseItems += count;
        Console.WriteLine($"Товар '{item}' добавлен в ячейку {address}. Всего на складе: {totalWarehouseItems}/{TOTAL_WAREHOUSE_CAPACITY}");
    }

    static void RemoveItem(string address, string item, int count)
    {
        if (count <= 0)
        {
            Console.WriteLine("Количество должно быть положительным.");
            return;
        }

        if (!IsValidAddress(address))
        {
            Console.WriteLine("Некорректный адрес.");
            return;
        }

        if (!warehouse.TryGetValue(address, out CellContent cell))
        {
            Console.WriteLine($"Ячейка {address} пуста.");
            return;
        }

        if (!cell.items.TryGetValue(item, out int currentCount) || currentCount < count)
        {
            Console.WriteLine($"Недостаточно товара '{item}' для удаления.");
            return;
        }

        cell.items[item] -= count;
        cell.total -= count;
        totalWarehouseItems -= count;

        if (cell.items[item] == 0)
            cell.items.Remove(item);

        Console.WriteLine($"Товар '{item}' удален из ячейки {address}. Всего на складе: {totalWarehouseItems}/{TOTAL_WAREHOUSE_CAPACITY}");
    }

    static void ShowInfo()
    {
        Console.WriteLine("=== СТАТИСТИКА СКЛАДА ===");
        Console.WriteLine($"Общая вместимость склада: {TOTAL_WAREHOUSE_CAPACITY} единиц");
        Console.WriteLine($"Текущее количество товаров: {totalWarehouseItems} единиц");
        Console.WriteLine($"Свободная емкость: {TOTAL_WAREHOUSE_CAPACITY - totalWarehouseItems} единиц");

        if (warehouse.Count == 0)
        {
            Console.WriteLine("\nСклад полностью пуст.");
            return;
        }

        int[] zoneItems = new int[3]; // 0 - зона A, 1 - зона B, 2 - зона C
        int totalCells = 3 * 14 * 7 * 20;
        int usedCells = 0;

        foreach (var entry in warehouse)
        {
            usedCells++;
            char zone = entry.Key[0];
            if (zone == 'A') zoneItems[0] += entry.Value.total;
            else if (zone == 'B') zoneItems[1] += entry.Value.total;
            else zoneItems[2] += entry.Value.total;
        }

        float totalPercent = (float)totalWarehouseItems / TOTAL_WAREHOUSE_CAPACITY * 100;
        Console.WriteLine($"Общая загруженность склада: {totalPercent.ToString("F2", CultureInfo.InvariantCulture)}%");

        int zoneCapacity = TOTAL_WAREHOUSE_CAPACITY / 3;
        Console.WriteLine($"Зона A: {(float)zoneItems[0] / zoneCapacity * 100:F2}% ({zoneItems[0]}/{zoneCapacity})");
        Console.WriteLine($"Зона B: {(float)zoneItems[1] / zoneCapacity * 100:F2}% ({zoneItems[1]}/{zoneCapacity})");
        Console.WriteLine($"Зона C: {(float)zoneItems[2] / zoneCapacity * 100:F2}% ({zoneItems[2]}/{zoneCapacity})");

        Console.WriteLine($"Занятые ячейки ({usedCells} из {totalCells}):");
        foreach (var entry in warehouse)
        {
            if (entry.Value.total > 0)
            {
                Console.Write($"{entry.Key}: {entry.Value.total} ед. (");
                foreach (var item in entry.Value.items)
                    Console.Write($"{item.Key} - {item.Value}, ");
                Console.WriteLine("\b\b)");
            }
        }

        Console.WriteLine($"Пустых ячеек: {totalCells - usedCells}");
    }

    static void Main()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;
        Console.WriteLine("Система управления складом (C# версия)");
        Console.WriteLine($"Максимальная вместимость склада: {TOTAL_WAREHOUSE_CAPACITY} единиц");
        Console.WriteLine("Формат адреса: ZRRSSH, где:");
        Console.WriteLine("  Z - зона (A, B или C)");
        Console.WriteLine("  RR - номер стеллажа (01-14)");
        Console.WriteLine("  S - секция (0-6)");
        Console.WriteLine("  H - полка (01-20)");
        Console.WriteLine("Пример адреса: A010101 (Зона A, стеллаж 01, секция 0, полка 01)\n");

        while (true)
        {
            Console.Write("\nВведите команду (ADD, REMOVE, INFO, EXIT): ");
            string command = Console.ReadLine().ToUpper();

            if (command == "ADD")
            {
                Console.Write("Введите адрес ячейки: ");
                string address = Console.ReadLine();
                Console.Write("Введите наименование товара: ");
                string item = Console.ReadLine();
                Console.Write("Введите количество товара: ");
                if (int.TryParse(Console.ReadLine(), out int count))
                    AddItem(address, item, count);
                else
                    Console.WriteLine("Некорректное количество.");
            }
            else if (command == "REMOVE")
            {
                Console.Write("Введите адрес ячейки: ");
                string address = Console.ReadLine();
                Console.Write("Введите наименование товара: ");
                string item = Console.ReadLine();
                Console.Write("Введите количество товара: ");
                if (int.TryParse(Console.ReadLine(), out int count))
                    RemoveItem(address, item, count);
                else
                    Console.WriteLine("Некорректное количество.");
            }
            else if (command == "INFO")
            {
                ShowInfo();
            }
            else if (command == "EXIT")
            {
                break;
            }
            else
            {
                Console.WriteLine("Неизвестная команда. Доступные команды: ADD, REMOVE, INFO, EXIT");
            }
        }
    }
}
