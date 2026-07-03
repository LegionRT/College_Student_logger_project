#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "StudentCore.h"
#include <random>
Student* createGroup(int size) {
	if (size <= 0) return nullptr;
	return (Student*)malloc(size * sizeof(Student));
}


void initDemoData(Student* group, int size) {
	if (group == nullptr) return;
	const char* names[] = { "John Doe", "Jane Smith", "Alice Johnson", "Bob Brown", "Charlie Davis" };
	const int namesCount = 5;
	std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dist(2, 5);
	for (int i = 0; i < size; i++) {
		group[i].id = i + 1;
		strcpy(group[i].fullName, names[i % namesCount]);
		for (int j = 0; j < 5; j++) {
			group[i].scores[j] = dist(gen);
		}
		group[i].averageScore = 0.0;
	}
}

void calculateAllAverages(Student* group, int size) {
	if (group == nullptr) return;
	for (int i = 0; i < size; i++) {
		int sum = 0;
		for (int j = 0; j < 5; j++) {
			sum += group[i].scores[j];
		}
		group[i].averageScore = sum / 5.0;

	}

}

int findBestStudent(const Student* group, int size) {
	if (group == nullptr) return -1;
	int bestId = -1;
	double maxGrade = -1.0;
	for (int i = 0; i < size; i++) {
		if (maxGrade < group[i].averageScore) {
			maxGrade = group[i].averageScore;
			bestId = group[i].id;
		}
	}
	return bestId;
}


int countDebtors(const Student* group, int size) {
	if (group == nullptr) return 0;
	int debtorCount = 0;
	for (int i = 0; i < size; i++) {
		bool hasDebt = false;
		for (int j = 0; j < 5; j++) {
			if (group[i].scores[j] < 3) {
				hasDebt = true;
				break;
			}
		}if (hasDebt) {
			debtorCount++;
		}
	}
	return debtorCount;
}


Student* filterByAverage(const Student* group, int size, double threshold, int* outSize) {
	if (group == nullptr || outSize == nullptr) {
		if (outSize != nullptr) *outSize = 0;
		return nullptr;
	}
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (group[i].averageScore >= threshold) {
			count++;
		}
	}
	*outSize = count;
	if (count == 0) {
		return nullptr;
	}
	Student* filteredGroup = (Student*)malloc(count * sizeof(Student));
	if (filteredGroup == nullptr) {
		*outSize = 0;
		return nullptr;
	}
	int id = 0;
	for (int i = 0; i < size; i++) {
		if (group[i].averageScore >= threshold) {
			filteredGroup[id] = group[i];
			id++;
		}
	}
	return filteredGroup;
}


void sortByAverage(Student* group, int size) {
	if (group == nullptr) return;
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (group[j].averageScore < group[j + 1].averageScore) {
				Student temp = group[j];
				group[j] = group[j + 1];
				group[j + 1] = temp;
			}
		}
	}

}


void freeGroup(Student* group) {
	if (group == nullptr) return;
	free(group);
}