#include <gtest/gtest.h>

#include <iostream>

#include "../lib/s21_ordered_list.h"

TEST(orderedList, Constructor_Default) {
  s21::orderedList<int> s21_list;

  EXPECT_EQ(s21_list.size(), 0);
  EXPECT_EQ(s21_list.empty(), true);
}

TEST(orderedListConstructor, ConstructWithSize) {
  s21::orderedList<int> intList(5);
  EXPECT_EQ(intList.size(), 5);

  for (size_t i = 0; i < intList.size(); ++i) {
    EXPECT_EQ(intList[i], 0);
  }

  s21::orderedList<std::string> stringList(3);
  EXPECT_EQ(stringList.size(), 3);

  EXPECT_TRUE(stringList[0].empty());

  for (size_t i = 0; i < stringList.size(); ++i) {
    EXPECT_TRUE(stringList[i].empty());
  }
}

TEST(ListInitializerOrderedListConstructor, BaseTest) {
  s21::orderedList<int> intList = {1, 2, 3, 4, 5};

  EXPECT_EQ(intList.size(), 5);

  size_t index = 0;
  for (int value : {1, 2, 3, 4, 5}) {
    EXPECT_EQ(intList[index], value);
    ++index;
  }
}

TEST(ListInitializerOrderedListConstructor, EmptyTest) {
  s21::orderedList<int> emptyList = {};

  EXPECT_TRUE(emptyList.empty());
}

TEST(OrderedListCopyConstructor, CopyConstructorCreatesExactCopy) {
  s21::orderedList<int> originalList = {1, 2, 3, 4, 5};

  s21::orderedList<int> copiedList(originalList);

  EXPECT_EQ(originalList.size(), copiedList.size());
  for (size_t index = 0; index < copiedList.size(); ++index) {
    EXPECT_EQ(originalList[index], originalList[index]);
  }
}

TEST(OrderedListCopyConstructor, ModificationsToCopyDoNotAffectOriginal) {
  s21::orderedList<int> originalList = {1, 2, 3};
  s21::orderedList<int> copiedList(originalList);

  copiedList[0] = 0;
  EXPECT_EQ(originalList[0], 1);
  EXPECT_EQ(copiedList[0], 0);
}

TEST(OrderedListMoveConstructor, CanMoveList) {
  // Создаем и заполняем временный список
  s21::orderedList<int> temp = {1, 2, 3, 4, 5};

  // Используем конструктор перемещения для создания нового списка
  s21::orderedList<int> movedList = std::move(temp);

  // Проверяем, что новый список содержит правильные данные
  EXPECT_EQ(movedList.size(), 5);

  int expectedValue = 1;
  for (size_t index = 0; index < movedList.size(); ++index) {
    EXPECT_EQ(movedList[index], expectedValue++);
  }

  // Проверяем, что исходный список теперь пуст
  EXPECT_TRUE(temp.empty());
}

TEST(OrderedListMoveAssignment, CanMoveAssignList) {
  s21::orderedList<int> temp = {1, 2, 3, 4, 5};

  s21::orderedList<int> targetList;
  targetList = std::move(temp);

  EXPECT_EQ(targetList.size(), 5);

  int expectedValue = 1;
  for (size_t index = 0; index < targetList.size(); ++index) {
    EXPECT_EQ(targetList[index], expectedValue++);
  }

  EXPECT_TRUE(temp.empty());
}

TEST(OrderedListSwap, CorrectlySwapsContents) {
  s21::orderedList<int> list1 = {1, 2, 3};
  s21::orderedList<int> list2 = {4, 5, 6, 7};

  // Запоминаем размеры списков до swap
  auto size1_before = list1.size();
  auto size2_before = list2.size();

  list1.swap(list2);

  // Проверяем, что размеры списков поменялись
  EXPECT_EQ(list1.size(), size2_before);
  EXPECT_EQ(list2.size(), size1_before);

  // Проверяем, что содержимое списков поменялось
  EXPECT_EQ(list1[0], 4);
  EXPECT_EQ(list1[3], 7);
  EXPECT_EQ(list2[0], 1);
  EXPECT_EQ(list2[2], 3);
}
