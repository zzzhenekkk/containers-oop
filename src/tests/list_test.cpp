#include <gtest/gtest.h>

#include <iostream>
#include <list>

#include "../lib/s21_list.h"

// проблема с max_size, надо подкрутить
// TEST(List, MaxSize) {
//     s21::list<int> s21_list;
//     std::list<int> std_list;
//     EXPECT_EQ(s21_list.max_size(),  4611686018427387903); // у нас
//     EXPECT_EQ(s21_list.max_size(),  std_list.max_size()); // должно быть
// }

TEST(list, Constructor_Default) {
  s21::list<int> s21_list;
  std::list<int> std_list;

  EXPECT_EQ(s21_list.size(), std_list.size());
  EXPECT_EQ(s21_list.empty(), std_list.empty());
}

TEST(ListConstructor, ConstructWithSize) {
  s21::list<int> intList(5);
  EXPECT_EQ(intList.size(), 5);

  for (size_t i = 0; i < intList.size(); ++i) {
    EXPECT_EQ(intList[i], 0);
  }

  s21::list<std::string> stringList(3);
  EXPECT_EQ(stringList.size(), 3);

  for (size_t i = 0; i < stringList.size(); ++i) {
    EXPECT_TRUE(stringList[i].empty());
  }
}

TEST(ListInitializerListConstructor, CanInitializeWithInitializerList) {
  s21::list<int> intList = {1, 2, 3, 4, 5};

  EXPECT_EQ(intList.size(), 5);

  size_t index = 0;
  for (int value : {1, 2, 3, 4, 5}) {
    EXPECT_EQ(intList[index], value);
    ++index;
  }
}

TEST(ListInitializerListConstructor, InitializeWithEmptyList) {
  s21::list<int> emptyList = {};

  EXPECT_TRUE(emptyList.empty());
}

TEST(ListCopyConstructor, CopyConstructorCreatesExactCopy) {
  s21::list<int> originalList = {1, 2, 3, 4, 5};

  s21::list<int> copiedList(originalList);

  EXPECT_EQ(originalList.size(), copiedList.size());

  size_t index = 0;
  for (auto it = copiedList.begin(); it != copiedList.end(); ++it) {
    EXPECT_EQ(*it, originalList[index]);
    ++index;
  }
}

TEST(ListCopyConstructor, ModificationsToCopyDoNotAffectOriginal) {
  s21::list<int> originalList = {1, 2, 3};
  s21::list<int> copiedList(originalList);

  *(copiedList.begin()) = 10;

  // Проверяем, что первый элемент оригинального списка не изменился
  EXPECT_EQ(*(originalList.begin()), 1);
}

TEST(ListMoveConstructor, CanMoveList) {
  // Создаем и заполняем временный список
  s21::list<int> temp = {1, 2, 3, 4, 5};

  // Используем конструктор перемещения для создания нового списка
  s21::list<int> movedList = std::move(temp);

  // Проверяем, что новый список содержит правильные данные
  EXPECT_EQ(movedList.size(), 5);
  int expectedValue = 1;
  for (const auto& item : movedList) {
    EXPECT_EQ(item, expectedValue++);
  }

  // Проверяем, что исходный список теперь пуст
  EXPECT_TRUE(temp.empty());
}

TEST(ListMoveConstructor, SourceListIsValidAfterMove) {
  s21::list<int> temp = {1, 2, 3};
  s21::list<int> movedList = std::move(temp);

  EXPECT_EQ(temp.size(), 0);
}

TEST(ListMoveAssignment, CanMoveAssignList) {
  s21::list<int> temp = {1, 2, 3, 4, 5};

  s21::list<int> targetList;
  targetList = std::move(temp);

  EXPECT_EQ(targetList.size(), 5);
  int expectedValue = 1;
  for (const auto& item : targetList) {
    EXPECT_EQ(item, expectedValue++);
  }

  EXPECT_TRUE(temp.empty());
}

TEST(ListMoveAssignment, ReleasesResourcesBeforeMoveAssign) {
  s21::list<int> targetList = {10, 20, 30};
  s21::list<int> temp = {1, 2, 3, 4, 5};

  targetList = std::move(temp);

  EXPECT_EQ(targetList.size(), 5);
}

TEST(ListIteratorDecrement, DecrementMiddle) {
  s21::list<int> myList = {1, 2, 3, 4, 5};
  auto it = myList.begin();
  ++it;  // Перемещаемся к 2
  ++it;  // Перемещаемся к 3

  --it;  // Декрементируем итератор, должны вернуться к 2
  EXPECT_EQ(*it, 2);
}

// Тест на декремент итератора, указывающего на последний элемент
TEST(ListIteratorDecrement, DecrementFromEnd) {
  s21::list<int> myList = {1, 2, 3, 4, 5};
  auto it = myList.end();
  --it;  // Декремент до последнего элемента (5)
  EXPECT_EQ(*it, 5);

  --it;  // Декремент до предпоследнего элемента (4)
  EXPECT_EQ(*it, 4);
}

// Тест на декремент итератора, указывающего на начало списка
TEST(ListIteratorDecrement, DecrementFromBegin) {
  std::list<int> myList = {1, 2, 3, 4, 5};
  auto it = myList.begin();

  s21::list<int> myList_s21 = {1, 2, 3, 4, 5};
  auto it_s21 = myList_s21.begin();

  --it;
  --it_s21;

  EXPECT_EQ(*it, *it_s21);
}

TEST(Insert, InsertStart) {
  s21::list<int> list = {1, 2, 3};
  s21::list<int>::iterator it = list.begin();

  // вставляем в начало
  s21::list<int>::iterator it2 = list.insert(it, 0);

  // Проверяем, что первый элемент теперь равен 0
  EXPECT_EQ(list[0], 0);
  // Проверяем, что итератор ссылается на вставленный элемент
  EXPECT_EQ(*it2, 0);
  EXPECT_EQ(list.size(), 4);
}

TEST(Insert, InsertMiddle1) {
  s21::list<int> list = {1, 2, 3};
  s21::list<int>::iterator it = list.begin();

  ++it;
  // вставляем меду первым и вторым
  s21::list<int>::iterator it2 = list.insert(it, 5);

  // Проверяем, что второй элемент теперь равен 5
  EXPECT_EQ(list[1], 5);
  // Проверяем, что итератор ссылается на вставленный элемент
  EXPECT_EQ(*it2, 5);
  EXPECT_EQ(list.size(), 4);
}

TEST(Insert, InsertMiddle2) {
  s21::list<int> list = {1, 2, 3};
  s21::list<int>::iterator it = list.begin();

  ++it;
  ++it;
  // вставляем меду вторым и третьим (между предпоследним и последним)
  s21::list<int>::iterator it2 = list.insert(it, 7);

  // Проверяем, что третий элемент теперь равен 7
  EXPECT_EQ(list[2], 7);
  // Проверяем, что итератор ссылается на вставленный элемент
  EXPECT_EQ(*it2, 7);
  EXPECT_EQ(list.size(), 4);
}

TEST(Insert, InsertEnd) {
  s21::list<int> list = {1, 2, 3};
  s21::list<int>::iterator it = list.end();

  // вставляем в конец
  s21::list<int>::iterator it2 = list.insert(it, 9);

  EXPECT_EQ(list[3], 9);
  // Проверяем, что итератор ссылается на вставленный элемент
  EXPECT_EQ(*it2, 9);
  EXPECT_EQ(list.size(), 4);
}

TEST(Erase, Start) {
  s21::list<int> list = {1, 2, 3};
  s21::list<int>::iterator it = list.begin();

  list.erase(it);

  // Проверяем, что первый элемент теперь равен 2
  EXPECT_EQ(list[0], 2);
}

TEST(Erase, Second) {
  s21::list<int> list = {1, 2, 3};
  s21::list<int>::iterator it = list.begin();
  ++it;

  list.erase(it);

  // Проверяем, что первый элемент теперь равен 2
  EXPECT_EQ(list[1], 3);
}

TEST(Erase, Last) {
  s21::list<int> list = {1, 2, 3, 4};
  s21::list<int>::iterator it = list.begin();
  ++it;
  ++it;
  ++it;

  list.erase(it);

  // Проверяем, что первый элемент теперь равен 2
  EXPECT_EQ(list[2], 3);
  EXPECT_EQ(list.size(), 3);
}

// Тестирование доступа к первому элементу непустого списка
TEST(ListFrontBack, AccessFirstElement) {
  s21::list<int> myList{1, 2, 3, 4, 5};

  const int expectedFirstElement = 1;
  EXPECT_EQ(myList.front(), expectedFirstElement);
}

// Тестирование доступа к последнему элементу непустого списка
TEST(ListFrontBack, AccessLastElement) {
  s21::list<int> myList{1, 2, 3, 4, 5};

  const int expectedLastElement = 5;
  EXPECT_EQ(myList.back(), expectedLastElement);
}

TEST(ListPop, PopBack) {
  s21::list<int> myList{1, 2, 3, 4, 5};
  myList.pop_back();
  EXPECT_EQ(myList.back(), 4);
  EXPECT_EQ(myList.size(), 4);
}

TEST(ListPop, PopBackEmpty) {
  s21::list<int> myList;
  EXPECT_THROW(myList.pop_back(), std::out_of_range);
}

TEST(ListPop, PopFront) {
  s21::list<int> myList{0, 1, 2, 3, 4, 5};
  myList.pop_front();
  EXPECT_EQ(myList.front(), 1);
  EXPECT_EQ(myList.size(), 5);
}

TEST(ListPop, PopFrontEmpty) {
  s21::list<int> myList;
  EXPECT_THROW(myList.pop_front(), std::out_of_range);
}

// Тест проверяет, что swap корректно обменивает содержимое двух списков
TEST(ListSwap, CorrectlySwapsContents) {
  s21::list<int> list1 = {1, 2, 3};
  s21::list<int> list2 = {4, 5, 6, 7};

  // Запоминаем размеры списков до swap
  auto size1_before = list1.size();
  auto size2_before = list2.size();

  list1.swap(list2);

  // Проверяем, что размеры списков поменялись
  EXPECT_EQ(list1.size(), size2_before);
  EXPECT_EQ(list2.size(), size1_before);

  // Проверяем, что содержимое списков поменялось
  auto it = list1.begin();
  EXPECT_EQ(*it++, 4);
  EXPECT_EQ(*it++, 5);
  EXPECT_EQ(*it++, 6);
  EXPECT_EQ(*it, 7);

  auto it2 = list2.begin();
  EXPECT_EQ(*it2++, 1);
  EXPECT_EQ(*it2++, 2);
  EXPECT_EQ(*it2, 3);
}

// Тест проверяет, что swap корректно работает с пустыми списками
TEST(ListSwap, WorksCorrectlyWithEmptyLists) {
  s21::list<int> list1 = {1, 2, 3};
  s21::list<int> emptyList;

  list1.swap(emptyList);

  // Проверяем, что list1 теперь пустой, а emptyList содержит элементы
  EXPECT_TRUE(list1.empty());
  EXPECT_FALSE(emptyList.empty());

  // Проверяем содержимое теперь непустого списка
  auto it = emptyList.begin();
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it, 3);
}

TEST(ListSpliceTest, MoveAllElementsInEmptyList) {
  s21::list<int> s21List1, s21List2;
  std::list<int> stdList1, stdList2;

  // Заполняем списки элементами
  for (int i = 0; i < 5; ++i) {
    s21List2.push_back(i);
    stdList2.push_back(i);
  }

  // Выполняем splice
  s21List1.splice(s21List1.end(), s21List2);
  stdList1.splice(stdList1.end(), stdList2);

  // Проверяем размеры списков после splice
  EXPECT_EQ(s21List1.size(), stdList1.size());
  EXPECT_EQ(s21List2.size(), stdList2.size());

  // Проверяем, что элементы корректно переместились
  auto s21It = s21List1.begin();
  auto stdIt = stdList1.begin();
  for (; s21It != s21List1.end() && stdIt != stdList1.end(); ++s21It, ++stdIt) {
    EXPECT_EQ(*s21It, *stdIt);
  }
}

TEST(ListSpliceTest, MoveAllElements) {
  s21::list<int> s21List1 = {0}, s21List2;
  std::list<int> stdList1 = {0}, stdList2;

  // Заполняем списки элементами
  for (int i = 0; i < 5; ++i) {
    s21List2.push_back(i);
    stdList2.push_back(i);
  }

  // Выполняем splice
  s21List1.splice(s21List1.end(), s21List2);
  stdList1.splice(stdList1.end(), stdList2);

  // Проверяем размеры списков после splice
  EXPECT_EQ(s21List1.size(), stdList1.size());
  EXPECT_EQ(s21List2.size(), stdList2.size());

  // Проверяем, что элементы корректно переместились
  auto s21It = s21List1.begin();
  auto stdIt = stdList1.begin();
  for (; s21It != s21List1.end() && stdIt != stdList1.end(); ++s21It, ++stdIt) {
    EXPECT_EQ(*s21It, *stdIt);
  }
}

// Тест на перемещение элементов в начало списка
TEST(ListSplice, MoveElementsToFront) {
  s21::list<int> list1 = {4, 5, 6};
  s21::list<int> list2 = {1, 2, 3};

  list1.splice(list1.begin(), list2);

  EXPECT_EQ(list1.size(), 6);  // Проверяем размер результирующего списка
  EXPECT_TRUE(
      list2.empty());  // Второй список должен быть пустым после операции

  // Проверяем порядок элементов
  auto it = list1.begin();
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it++, 3);
  EXPECT_EQ(*it++, 4);
  EXPECT_EQ(*it++, 5);
  EXPECT_EQ(*it, 6);
}

TEST(ListSplice, MoveElementsToMiddle) {
  s21::list<int> list1 = {4, 5, 6};
  s21::list<int> list2 = {1, 2, 3};

  list1.splice(++list1.begin(), list2);

  EXPECT_EQ(list1.size(), 6);  // Проверяем размер результирующего списка
  EXPECT_TRUE(
      list2.empty());  // Второй список должен быть пустым после операции

  // Проверяем порядок элементов
  auto it = list1.begin();
  EXPECT_EQ(*it++, 4);
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it++, 3);
  EXPECT_EQ(*it++, 5);
  EXPECT_EQ(*it, 6);
}

TEST(ListSplice, MoveBetweenEmptyLists) {
  s21::list<int> list1;
  s21::list<int> list2;

  list1.splice(list1.begin(), list2);

  EXPECT_TRUE(list1.empty());
  EXPECT_TRUE(list2.empty());
}

TEST(ListSplice, MoveInEmptyLists) {
  s21::list<int> list1;
  s21::list<int> list2 = {2, 20};

  list1.splice(list1.begin(), list2);

  EXPECT_EQ(list1.size(), 2);
  EXPECT_TRUE(list2.empty());

  // Проверяем порядок элементов
  auto it = list1.begin();
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it++, 20);
}

TEST(ListSplice, SpliceItself) {
  s21::list<int> list = {1, 2, 3};
  list.splice(list.begin(), list);

  EXPECT_EQ(list.size(), 3);
  auto it = list.begin();
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it, 3);
}

TEST(ListReverse, HandleEmptyList) {
  s21::list<int> emptyList;
  emptyList.reverse();
  EXPECT_EQ(emptyList.size(), 0);
}

TEST(ListReverse, HandleSingleElementList) {
  s21::list<int> singleElementList = {1};
  singleElementList.reverse();
  EXPECT_EQ(singleElementList.front(), 1);
  EXPECT_EQ(singleElementList.back(), 1);
}

TEST(ListReverse, HandleMultipleElementsList) {
  s21::list<int> multipleElementsList = {1, 2, 3, 4, 5};
  multipleElementsList.reverse();

  std::vector<int> expectedReversedElements = {5, 4, 3, 2, 1};
  std::vector<int> actualReversedElements;
  for (auto it = multipleElementsList.begin(); it != multipleElementsList.end();
       ++it) {
    actualReversedElements.push_back(*it);
  }

  EXPECT_EQ(actualReversedElements, expectedReversedElements);
}

// Тест на проверку сохранения размера списка после reverse
TEST(ListReverse, PreserveSizeAfterReverse) {
  s21::list<int> list = {1, 2, 3, 4, 5};
  size_t originalSize = list.size();
  list.reverse();
  EXPECT_EQ(list.size(), originalSize);
}

TEST(ListUnique, RemovesConsecutiveDuplicates) {
  s21::list<int> testList = {1, 2, 2, 3, 3, 3, 4, 5, 5};
  testList.unique();
  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  for (auto it = testList.begin(); it != testList.end(); ++it, ++i) {
    EXPECT_EQ(*it, expected[i]);
  }
  EXPECT_EQ(i, 5);
}

TEST(ListUnique, WorksOnEmptyList) {
  s21::list<int> emptyList;
  emptyList.unique();
  EXPECT_TRUE(emptyList.empty());
}

TEST(ListUnique, WorksOnSingleElementList) {
  s21::list<int> singleElementList = {42};
  singleElementList.unique();
  EXPECT_EQ(singleElementList.size(), 1);
  EXPECT_EQ(*(singleElementList.begin()), 42);
}

TEST(ListUnique, NoDuplicates) {
  s21::list<int> noDuplicateList = {1, 2, 3, 4, 5};
  noDuplicateList.unique();
  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  for (auto it = noDuplicateList.begin(); it != noDuplicateList.end();
       ++it, ++i) {
    EXPECT_EQ(*it, expected[i]);
  }
  EXPECT_EQ(i, 5);
}

TEST(ListSort, SortEmptyList) {
  s21::list<int> list;
  list.sort();
  EXPECT_TRUE(list.empty());
}

// Тест на сортировку списка из одного элемента
TEST(ListSort, SortSingleElementList) {
  s21::list<int> list = {1};
  list.sort();
  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 1);
}

// Тест на сортировку списка из нескольких элементов
TEST(ListSort, SortMultipleElementsList) {
  s21::list<int> list = {3, 1, 4, 1, 5, 9, 2, 6};
  list.sort();

  std::vector<int> expected = {1, 1, 2, 3, 4, 5, 6, 9};
  std::vector<int> actual;
  for (auto it = list.begin(); it != list.end(); ++it) {
    actual.push_back(*it);
  }
  EXPECT_EQ(actual, expected);
}

// Тест на сортировку уже отсортированного списка
TEST(ListSort, SortAlreadySorted) {
  s21::list<int> list = {1, 2, 3, 4, 5};
  list.sort();

  std::vector<int> expected = {1, 2, 3, 4, 5};
  std::vector<int> actual;
  for (auto it = list.begin(); it != list.end(); ++it) {
    actual.push_back(*it);
  }
  EXPECT_EQ(actual, expected);
}

// Тест на сортировку списка с обратным порядком элементов
TEST(ListSort, SortReverseOrder) {
  s21::list<int> list = {5, 4, 3, 2, 1};
  list.sort();

  std::vector<int> expected = {1, 2, 3, 4, 5};
  std::vector<int> actual;
  for (auto it = list.begin(); it != list.end(); ++it) {
    actual.push_back(*it);
  }
  EXPECT_EQ(actual, expected);
}

TEST(List, Merge) {
  s21::list<int> our_list_first = {1};
  s21::list<int> our_list_second = {2, 3, 4, 5};
  std::list<int> std_list_first = {1};
  std::list<int> std_list_second = {2, 3, 4, 5};
  our_list_first.merge(our_list_second);
  std_list_first.merge(std_list_second);
  EXPECT_EQ(our_list_first.front(), std_list_first.front());
  EXPECT_EQ(our_list_first.back(), std_list_first.back());
  EXPECT_EQ(our_list_second.empty(), std_list_second.empty());
}

TEST(List, Merge2) {
  s21::list<int> our_list_first = {10, 6, 128};
  s21::list<int> our_list_second = {256};
  std::list<int> std_list_first = {10, 6, 128};
  std::list<int> std_list_second = {256};
  our_list_first.merge(our_list_second);
  std_list_first.merge(std_list_second);
  EXPECT_EQ(our_list_first.front(), std_list_first.front());
  EXPECT_EQ(our_list_first.back(), std_list_first.back());
  EXPECT_EQ(our_list_second.empty(), std_list_second.empty());
}

TEST(List, Merge3) {
  s21::list<int> our_list_first = {10, 6, 64};
  s21::list<int> our_list_second = {256, 128};
  std::list<int> std_list_first = {10, 6, 64};
  std::list<int> std_list_second = {256, 128};
  our_list_first.merge(our_list_second);
  std_list_first.merge(std_list_second);
  EXPECT_EQ(our_list_first.front(), std_list_first.front());
  EXPECT_EQ(our_list_first.back(), std_list_first.back());
  EXPECT_EQ(our_list_second.empty(), std_list_second.empty());
}

TEST(List, Insert_Many_first) {
  s21::list<int> our_list = {1, 2, 3, 4, 5};
  s21::list<int>::iterator our_it = our_list.begin();
  our_list.insert_many(our_it, 7, 8, 9);
  auto new_it = our_list.begin();
  EXPECT_EQ(*new_it, 7);
  ++new_it;
  EXPECT_EQ(*new_it, 8);
  ++new_it;
  EXPECT_EQ(*new_it, 9);
  ++new_it;
  EXPECT_EQ(*new_it, 1);
  ++new_it;
  EXPECT_EQ(*new_it, 2);
}

TEST(List, Insert_Many_second) {
  s21::list<int> our_list = {1, 2, 3, 4, 5};
  s21::list<int>::iterator our_it = our_list.begin();
  ++our_it;
  our_list.insert_many(our_it, 7, 8, 9);
  auto new_it = our_list.begin();
  EXPECT_EQ(*new_it, 1);
  ++new_it;
  EXPECT_EQ(*new_it, 7);
  ++new_it;
  EXPECT_EQ(*new_it, 8);
  ++new_it;
  EXPECT_EQ(*new_it, 9);
  ++new_it;
  EXPECT_EQ(*new_it, 2);
}

TEST(List, Insert_Many_back) {
  s21::list<int> our_list = {1, 2, 3, 4, 5};
  our_list.insert_many_back(7, 8);
  auto new_it = our_list.end();
  --new_it;
  --new_it;
  --new_it;
  EXPECT_EQ(*new_it, 5);
  ++new_it;
  EXPECT_EQ(*new_it, 7);
  ++new_it;
  EXPECT_EQ(*new_it, 8);
  ++new_it;
}

TEST(List, Insert_Many_front) {
  s21::list<int> our_list = {1, 2, 3};
  our_list.insert_many_front(7, 8);
  auto new_it = our_list.begin();
  EXPECT_EQ(*new_it, 7);
  ++new_it;
  EXPECT_EQ(*new_it, 8);
  ++new_it;
  EXPECT_EQ(*new_it, 1);
  ++new_it;
}

TEST(List, Insert_Many_empty) {
  s21::list<int> our_list;
  s21::list<int>::iterator our_it = our_list.begin();
  our_list.insert_many(our_it, 7, 8, 9);
  auto new_it = our_list.begin();
  EXPECT_EQ(*new_it, 7);
  ++new_it;
  EXPECT_EQ(*new_it, 8);
  ++new_it;
  EXPECT_EQ(*new_it, 9);
}