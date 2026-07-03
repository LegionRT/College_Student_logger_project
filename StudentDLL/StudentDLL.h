#pragma once

#ifdef STUDENTDLL_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

struct Student {
    int id;
    char fullName[100];
    int scores[5];
    double averageScore;
};
extern "C" {
    CORE_API Student* createGroup(int size);
    CORE_API void initDemoData(Student* group, int size);
    CORE_API void calculateAllAverages(Student* group, int size);
    CORE_API int findBestStudent(const Student* group, int size);
    CORE_API int countDebtors(const Student* group, int size);
    CORE_API Student* filterByAverage(const Student* group, int size, double threshold, int* outSize);
    CORE_API void sortByAverage(Student* group, int size);
    CORE_API void freeGroup(Student* group);
}