using System;
using System.Collections.Generic;
using System.Linq;

// Структура для хранения информации о посетителе
public struct Visitor
{
    public string Ticket;      // Номер талона
    public int Duration;      // Продолжительность посещения
}

public class Program
{
    private static Random random = new Random();
    
    // Функция для генерации случайного номера талона
    public static string GenerateTicket()
    {
        const string charset = "0123456789";
        string ticket = "T";
        for (int i = 0; i < 3; ++i)
        {
            ticket += charset[random.Next(charset.Length)];
        }
        return ticket;
    }

    // Функция для распределения очереди по окнам
    public static void DistributeQueue(List<Visitor> visitors, List<List<Visitor>> windows)
    {
        // Создаем список для хранения суммарного времени каждого окна
        List<int> windowTimes = new List<int>(new int[windows.Count]);
        int currentVisitor = 0; // Индекс текущего посетителя

        // Пока есть посетители в очереди
        while (currentVisitor < visitors.Count)
        {
            Visitor current = visitors[currentVisitor];
            currentVisitor++;

            // Находим окно с минимальным временем
            int minTime = windowTimes.Min();
            int windowIndex = windowTimes.IndexOf(minTime);

            // Добавляем посетителя в это окно
            windows[windowIndex].Add(current);
            windowTimes[windowIndex] += current.Duration;
        }

        // Выводим распределение
        for (int i = 0; i < windows.Count; ++i)
        {
            if (windows[i].Count > 0)
            {
                Console.Write($">>> Окно {i + 1} ({windowTimes[i]} минут): ");

                for (int j = 0; j < windows[i].Count; ++j)
                {
                    Console.Write(windows[i][j].Ticket);
                    if (j != windows[i].Count - 1)
                    {
                        Console.Write(", ");
                    }
                }
                Console.WriteLine();
            }
        }
    }

    public static void CheckBags(int mbBags)
    {
        if (mbBags <= 0)
        {
            Console.WriteLine("введена невозможная продолжительность посещения или количество окон.");
            Environment.Exit(0);
        }
    }

    public static void Main()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;
        Console.WriteLine("Команда ENQUEUE - добавить продолжительность приема человека, чтобы получить талон.");
        Console.WriteLine("Команда DISTRIBUTE - узнать информацию об очереди.");
        
        int windowsCount;   // Количество окон
        List<Visitor> visitors = new List<Visitor>();  // Список посетителей вместо очереди
        List<List<Visitor>> windows = new List<List<Visitor>>();  // Список окон с распределенными посетителями

        Console.WriteLine(">>> Введите кол-во окон");
        Console.Write("<<< ");
        windowsCount = int.Parse(Console.ReadLine());
        CheckBags(windowsCount);

        for (int i = 0; i < windowsCount; i++)
        {
            windows.Add(new List<Visitor>());
        }

        while (true)
        {
            Console.Write("<<< ");
            string input = Console.ReadLine();
            string[] parts = input.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            
            if (parts.Length == 0) continue;
            
            string command = parts[0];

            if (command == "ENQUEUE")
            {
                if (parts.Length < 2)
                {
                    Console.WriteLine(">>> Необходимо указать продолжительность");
                    continue;
                }
                
                int duration;
                if (!int.TryParse(parts[1], out duration))
                {
                    Console.WriteLine(">>> Некорректная продолжительность");
                    continue;
                }
                
                CheckBags(duration);
              
// Создаем нового посетителя
                Visitor visitor = new Visitor
                {
                    Ticket = GenerateTicket(),
                    Duration = duration
                };

                // Добавляем в конец списка
                visitors.Add(visitor);

                Console.WriteLine($">>> {visitor.Ticket}");
            }
            else if (command == "DISTRIBUTE")
            {
                // Распределяем очередь по окнам
                DistributeQueue(visitors, windows);
                break;
            }
            else
            {
                Console.WriteLine(">>> Неизвестная команда");
            }
        }
    }
}
