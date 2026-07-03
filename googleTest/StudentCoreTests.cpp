#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include "StudentCore.h"
#include <cstring>

TEST(StudentCoreTest, CreateAndFreeGroup) {
    Student* group = createGroup(5);
    ASSERT_NE(group, nullptr) << "Группа не создана";

        for (int i = 0; i < 5; i++) {
        group[i].id = i + 1;
    }

    
    freeGroup(group);
    SUCCEED() << "Память успешно освобождена";
}

TEST(StudentCoreTest, AverageCalculation) {
    Student* group = createGroup(1);
    (group, nullptr);

        group[0].id = 1;
    strcpy(group[0].fullName, "Test Student");
    group[0].scores[0] = 4;
    group[0].scores[1] = 5;
    group[0].scores[2] = 3;
    group[0].scores[3] = 4;
    group[0].scores[4] = 5;

    calculateAllAverages(group, 1);

        EXPECT_DOUBLE_EQ(group[0].averageScore, 4.2)
        << "Неверный средний балл";

    freeGroup(group);
}

TEST(StudentCoreTest, FindBestStudent) {
    const int size = 3;
    Student* group = createGroup(size);
    ASSERT_NE(group, nullptr);

        group[0].id = 1;
    strcpy(group[0].fullName, "Ivanov");
    group[0].averageScore = 3.5;

    group[1].id = 2;
    strcpy(group[1].fullName, "Petrov");
    group[1].averageScore = 4.8;

    group[2].id = 3;
    strcpy(group[2].fullName, "Sidorov");
    group[2].averageScore = 4.2;

    int bestId = findBestStudent(group, size);

    EXPECT_EQ(bestId, 2) << "Лучший студент должен быть Petrov (ID=2)";

    freeGroup(group);
}

TEST(StudentCoreTest, DebtorsCount) {
    const int size = 4;
    Student* group = createGroup(size);
    ASSERT_NE(group, nullptr);

        group[0].id = 1;
    for (int i = 0; i < 5; i++) group[0].scores[i] = 4;

        group[1].id = 2;
    group[1].scores[0] = 2;
    for (int i = 1; i < 5; i++) group[1].scores[i] = 5;

        group[2].id = 3;
    for (int i = 0; i < 5; i++) group[2].scores[i] = 5;

        group[3].id = 4;
    group[3].scores[2] = 2;
    for (int i = 0; i < 5; i++) {
        if (i != 2) group[3].scores[i] = 4;
    }

    int debtors = countDebtors(group, size);

    EXPECT_EQ(debtors, 2) << "Должно быть 2 должника";

    freeGroup(group);
}

TEST(StudentCoreTest, FilterByAverage) {
    const int size = 4;
    Student* group = createGroup(size);
    ASSERT_NE(group, nullptr);

    group[0].id = 1;
    group[0].averageScore = 3.5;

    group[1].id = 2;
    group[1].averageScore = 4.5;

    group[2].id = 3;
    group[2].averageScore = 4.8;

    group[3].id = 4;
    group[3].averageScore = 3.9;

    int filteredSize = 0;
    Student* filtered = filterByAverage(group, size, 4.0, &filteredSize);

    EXPECT_EQ(filteredSize, 2) << "Должно быть 2 студента с баллом >= 4.0";
    EXPECT_NE(filtered, nullptr) << "Отфильтрованный массив не должен быть nullptr";

    if (filtered) {
                for (int i = 0; i < filteredSize; i++) {
            EXPECT_GE(filtered[i].averageScore, 4.0)
                << "Студент " << filtered[i].id << " имеет балл < 4.0";
        }
        freeGroup(filtered);
    }

    freeGroup(group);
}

TEST(StudentCoreTest, FilterEmptyResult) {
    const int size = 3;
    Student* group = createGroup(size);
    ASSERT_NE(group, nullptr);

    group[0].id = 1;
    group[0].averageScore = 3.0;

    group[1].id = 2;
    group[1].averageScore = 3.5;

    group[2].id = 3;
    group[2].averageScore = 3.8;

    int filteredSize = 0;
    Student* filtered = filterByAverage(group, size, 4.5, &filteredSize);

    EXPECT_EQ(filteredSize, 0) << "Не должно быть студентов с баллом >= 4.5";
    EXPECT_EQ(filtered, nullptr) << "Отфильтрованный массив должен быть nullptr";

    freeGroup(group);
}

TEST(StudentCoreTest, SortByAverage) {
    const int size = 3;
    Student* group = createGroup(size);
    ASSERT_NE(group, nullptr);

    group[0].id = 1;
    group[0].averageScore = 3.5;

    group[1].id = 2;
    group[1].averageScore = 4.8;

    group[2].id = 3;
    group[2].averageScore = 4.2;

    sortByAverage(group, size);

        EXPECT_GE(group[0].averageScore, group[1].averageScore)
        << "Первый студент должен иметь балл >= второго";
    EXPECT_GE(group[1].averageScore, group[2].averageScore)
        << "Второй студент должен иметь балл >= третьего";

    EXPECT_EQ(group[0].averageScore, 4.8) << "Первый должен быть Petrov (4.8)";
    EXPECT_EQ(group[1].averageScore, 4.2) << "Второй должен быть Sidorov (4.2)";
    EXPECT_EQ(group[2].averageScore, 3.5) << "Третий должен быть Ivanov (3.5)";

    freeGroup(group);
}

TEST(StudentCoreTest, NullPointerHandling) {
        EXPECT_EQ(createGroup(0), nullptr);
    EXPECT_EQ(createGroup(-1), nullptr);

        calculateAllAverages(nullptr, 5);      EXPECT_EQ(findBestStudent(nullptr, 5), -1);
    EXPECT_EQ(countDebtors(nullptr, 5), 0);
    sortByAverage(nullptr, 5);      freeGroup(nullptr);  
    SUCCEED() << "Все функции безопасно обработали nullptr";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}