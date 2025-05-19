#include <iostream>
#include <set>

using namespace std;

void addCleverStudents(set<int>& studentsList, int student) {
	int number = 0;
	for (int i = 0; i < student; i++) {
		studentsList.insert(student);
		number++;
	}
	cout << "Welcome " << number << " clever students!" << endl;
}

void top_List(set<int>& studentsList, set<int>& topList, int number_students) {
	if (number_students > *studentsList.rbegin()) {
		cout << "такого студента нет, повторите попытку.";
		return;
	}
	else {
		studentsList.erase(number_students);
		topList.insert(number_students);
		cout << "The suspected student " << number_students << endl;
	}
}

void immortal(set<int>& immortalStudents, set<int>& topList, int number_students) {
	if (number_students > *topList.rbegin()) {
		cout << "такого студента нет, повторите попытку.";
		return;
	}
	else {
		topList.erase(number_students);
		immortalStudents.insert(number_students);
		cout << "Student " << number_students << " is immortal!";
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	cout << "NEW_STUDENTS - добавить студентов в определенном количестве" << endl;
	cout << "SUSPICIOUS - добавить студента по номеру в топ-лист на отчисление" << endl;
	cout << "IMMORTAL - добавить студента по номеру в список неприкасаемых" << endl;
	cout << "TOP-LIST - вывести список студентов в топ-листе" << endl;
	cout << "SCOUNT - вывести количество студентов в стоп листе" << endl;
	cout << "EXIT - завершить программу" << endl;

	set<int> immortalStudents;
	set<int> studentsList;
	set<int> topList;
	string command;
	while (command != "EXIT") {
		cin >> command;
		if (command == "NEW_STUDENTS") {
			cout << "введите количество новых студентов: " << endl;
			int number;
			cin >> number;
			addCleverStudents(studentsList, number);
		}
		else if (command == "SUSPICIOUS") {
			int number;
			cout << "введите номер подозрительного студента: " << endl;
			cin >> number;
			top_List(studentsList, topList, number);
		}
		else if (command == "IMMORTAL") {
			cout << "введите номер неприкасаемого студента: " << endl;
			int number;
			cin >> number;
			immortal(immortalStudents, topList, number);
		}
		else if (command == "TOP-LIST") {
			cout << "список студентов на отчисление: ";
			for (auto n : topList) {
				cout << n << " ";
			}
			cout << endl;
		}
		else if (command == "SCOUNT") {
			cout << "количество студентов в списке на отчисление: " << topList.size() << endl;
		}
	}
	return 0;
}
