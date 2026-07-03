#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
    
struct Student {
    int id;
    char fullName[100];
    int scores[5];
    double averageScore;
};

typedef Student* (*CreateGroupFunc)(int);
typedef void (*InitDemoDataFunc)(Student*, int);
typedef void (*CalculateAllAveragesFunc)(Student*, int);
typedef int (*FindBestStudentFunc)(const Student*, int);
typedef int (*CountDebtorsFunc)(const Student*, int);
typedef Student* (*FilterByAverageFunc)(const Student*, int, double, int*);
typedef void (*SortByAverageFunc)(Student*, int);
typedef void (*FreeGroupFunc)(Student*);

HMODULE hLib = nullptr;
Student* group = nullptr;
int groupSize = 0;
bool dataInitialized = false;
bool groupInitialized = false;

CreateGroupFunc pCreateGroup = nullptr;
InitDemoDataFunc pInitDemoData = nullptr;
CalculateAllAveragesFunc pCalcAvg = nullptr;
FindBestStudentFunc pFindBest = nullptr;
CountDebtorsFunc pCountDebtors = nullptr;
FilterByAverageFunc pFilter = nullptr;
SortByAverageFunc pSort = nullptr;
FreeGroupFunc pFreeGroup = nullptr;

bool loadDLL() {
    hLib = LoadLibraryA("StudentDLL.dll");
    if (!hLib) {
        std::cout << "[ОШИБКА] DLL не найдена!\n";
        return false;
    }

        pCreateGroup = reinterpret_cast<CreateGroupFunc>(GetProcAddress(hLib, "createGroup"));
    pInitDemoData = reinterpret_cast<InitDemoDataFunc>(GetProcAddress(hLib, "initDemoData"));
    pCalcAvg = reinterpret_cast<CalculateAllAveragesFunc>(GetProcAddress(hLib, "calculateAllAverages"));
    pFindBest = reinterpret_cast<FindBestStudentFunc>(GetProcAddress(hLib, "findBestStudent"));
    pCountDebtors = reinterpret_cast<CountDebtorsFunc>(GetProcAddress(hLib, "countDebtors"));
    pFilter = reinterpret_cast<FilterByAverageFunc>(GetProcAddress(hLib, "filterByAverage"));
    pSort = reinterpret_cast<SortByAverageFunc>(GetProcAddress(hLib, "sortByAverage"));
    pFreeGroup = reinterpret_cast<FreeGroupFunc>(GetProcAddress(hLib, "freeGroup"));

    if (!pCreateGroup || !pInitDemoData || !pCalcAvg || !pFindBest ||
        !pCountDebtors || !pFilter || !pSort || !pFreeGroup) {
        std::cout << "[ОШИБКА] не все функции найдены!\n";
        FreeLibrary(hLib);
        hLib = nullptr;
        return false;
    }

    std::cout << "[УСПЕХ] DLL загружена!\n";
    return true;
}

void unloadDLL() {
    if (group && pFreeGroup) {
        pFreeGroup(group);
        group = nullptr;
    }
    if (hLib) {
        FreeLibrary(hLib);
        hLib = nullptr, pCreateGroup = nullptr,pInitDemoData = nullptr,pCalcAvg = nullptr,pFindBest = nullptr,pCountDebtors = nullptr,pFilter = nullptr,pSort = nullptr,pFreeGroup = nullptr;
        std::cout << "[УСПЕХ] DLL выгружена.\n";
    }
}

void showAllStudents() {
    if (!group) {
        std::cout << "группы не существует\n";
        return;
    }
    std::cout << "\nстуденты\n";
    for (int i = 0; i < groupSize; i++) {
        std::cout << "ID: " << group[i].id
            << " | ФИО: " << group[i].fullName
            << " | Оценки: ";
        for (int j = 0; j < 5; j++) {
            std::cout << group[i].scores[j] << " ";
        }
        std::cout << " | Средний: " << group[i].averageScore << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    int choice;

    do {
        std::cout << "\n=== МЕНЮ ===\n";
        std::cout << "1. Загрузить DLL\n";
        std::cout << "2. Создать группу\n";
        std::cout << "3. Заполнить данными\n";
        std::cout << "4. Показать всех\n";
        std::cout << "5. Рассчитать средние\n";
        std::cout << "6. Лучший студент\n";
        std::cout << "7. Должники\n";
        std::cout << "8. Фильтр (>4.0)\n";
        std::cout << "9. Сортировка\n";
        std::cout << "10. Выгрузить DLL\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            if (hLib) {
                std::cout << "DLL уже загружена.\n";
            }
            else {
                loadDLL();
            }
            break;

        case 2:
            if (!hLib) {
                std::cout << "Сначала загрузите DLL!\n";
                break;
            }
            if (groupInitialized) {
                std::cout << "Группа уже создана.\n";
                break;
            }
            std::cout << "Размер группы: ";
            std::cin >> groupSize;
            if (group) pFreeGroup(group);
            group = pCreateGroup(groupSize);
            groupInitialized = true;
            dataInitialized = false;
            std::cout << "Группа создана.\n";
            break;

        case 3:
            if (!hLib || !group) {
                std::cout << "Ошибка! Загрузите DLL и создайте группу.\n";
                break;
            }
            if (!groupInitialized) {
                std::cout << "Сначала создайте группу (пункт 2)!\n";
                break;
            }
            if (dataInitialized) {
                std::cout << "Демо-данные уже был заполнены.\n";
                break;
            }
            pInitDemoData(group, groupSize);
            dataInitialized = true;
            std::cout << "Демо-данные заполнены.\n";
            break;

        case 4:
            if (!hLib || !group) {
                std::cout << "Ошибка!\n";
                break;
            }
            showAllStudents();
            break;

        case 5:
            if (!hLib || !group) {
                std::cout << "Ошибка!\n";
                break;
            }
            pCalcAvg(group, groupSize);
            std::cout << "Средние баллы рассчитаны.\n";
            showAllStudents();
            break;

        case 6:
            if (!hLib || !group) {
                std::cout << "Ошибка!\n";
                break;
            }
            {
                int bestId = pFindBest(group, groupSize);
                std::cout << "Лучший студент ID: " << bestId << "\n";
            }
            break;

        case 7:
            if (!hLib || !group) {
                std::cout << "Ошибка!\n";
                break;
            }
            {
                int debtors = pCountDebtors(group, groupSize);
                std::cout << "Должников: " << debtors << "\n";
            }
            break;

        case 8:
            if (!hLib || !group) {
                std::cout << "Ошибка!\n";
                break;
            }
            {
                int filteredSize = 0;
                Student* filtered = pFilter(group, groupSize, 4.0, &filteredSize);
                std::cout << "Студентов с баллом > 4.0: " << filteredSize << "\n";
                if (filtered) {
                    for (int i = 0; i < filteredSize; i++) {
                        std::cout << filtered[i].fullName
                            << " | Средний: " << filtered[i].averageScore << "\n";
                    }
                    pFreeGroup(filtered);
                }
            }
            break;

        case 9:
            if (!hLib || !group) {
                std::cout << "Ошибка!\n";
                break;
            }
            pSort(group, groupSize);
            std::cout << "Отсортировано.\n";
            showAllStudents();
            break;

        case 10:
            unloadDLL();
            groupInitialized = false;
            dataInitialized = false;
            break;

        case 0:
            std::cout << "Выход...\n";
            break;

        default:
            std::cout << "Неверный выбор!\n";
        }
    } while (choice != 0);

    unloadDLL();
    return 0;
}