#include "attendance.h"

using namespace std;

AttendanceSystem& AttendanceSystem::instance() {
	static AttendanceSystem inst;
	return inst;
}

void AttendanceSystem::load() {
	ifstream fin{ "attendance_weekday_500.txt" }; //500�� ������ �Է�
	for (int rollBookIndex = 0; rollBookIndex < 500; rollBookIndex++) {
		string name, day;
		fin >> name >> day;
		updateAttendanceAndScore(name, day);
	}
}

void AttendanceSystem::compute() {
	for (int uniformNumber = 1; uniformNumber <= totalUniformNumber; uniformNumber++) {
		if (bonusPoint[uniformNumber][wednesday] > 9) {
			totalPoint[uniformNumber] += 10;
		}

		if (bonusPoint[uniformNumber][weekend] > 9) {
			totalPoint[uniformNumber] += 10;
		}

		if (totalPoint[uniformNumber] >= 50) {
			memberGrade[uniformNumber] = 2;
		}
		else if (totalPoint[uniformNumber] >= 30) {
			memberGrade[uniformNumber] = 1;
		}

		cout << "NAME : " << memberNameArray[uniformNumber] << ", ";
		cout << "POINT : " << totalPoint[uniformNumber] << ", ";
		cout << "GRADE : " << gradeList[memberGrade[uniformNumber]] << "\n";
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int uniformNumber = 1; uniformNumber <= totalUniformNumber; uniformNumber++) {
		if (memberGrade[uniformNumber] == 0 && trainingDayAttendanceCount[uniformNumber] == 0 && weekendAttendanceCount[uniformNumber] == 0) {
			std::cout << memberNameArray[uniformNumber] << "\n";
		}
	}
}

void AttendanceSystem::updateAttendanceAndScore(string name, string day) {
	updateAttendance(name);
	updateScore(name, day);
}

void AttendanceSystem::updateAttendance(string name) {
	if (rollBook.count(name) == 0) {
		rollBook.insert({ name, ++totalUniformNumber });
		memberNameArray[totalUniformNumber] = name;
	}
}

void AttendanceSystem::updateScore(string name, string day) {
	int uniformNumber = rollBook[name];

	if (day == "wednesday") {
		totalPoint[uniformNumber] += 3;
		bonusPoint[uniformNumber][wednesday] += 1;
		trainingDayAttendanceCount[uniformNumber]++;
	}
	else if (day == "saturday" || day == "sunday") {
		totalPoint[uniformNumber] += 2;
		bonusPoint[uniformNumber][weekend] += 1;
		weekendAttendanceCount[uniformNumber]++;
	}
	else {
		totalPoint[uniformNumber]++;
	}
}

AttendanceSystem::AttendanceSystem() = default;