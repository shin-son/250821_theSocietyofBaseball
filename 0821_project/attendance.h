#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class AttendanceSystem {
public:
	static AttendanceSystem& instance();
	void load();
	void compute();
	void updateAttendanceAndScore(std::string name, std::string day);
	void updateAttendance(std::string name);
	void updateScore(std::string name, std::string day);

private:
	AttendanceSystem();

	std::map<std::string, int> rollBook;
	int totalUniformNumber = 0;

	int bonusPoint[100][100];
	int totalPoint[100];
	int memberGrade[100];
	std::string memberNameArray[100];
	std::string gradeList[3] = { "NORMAL", "SILVER", "GOLD" };

	int trainingDayAttendanceCount[100];
	int weekendAttendanceCount[100];

	enum {
		wednesday = 0,
		weekend = 1,
	};
};