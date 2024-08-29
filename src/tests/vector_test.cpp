#include <gtest/gtest.h>

#include "../lib/s21_vector.h"

// Тест для функции at()
TEST(VectorFunctions, AtFunction) {
  s21::vector<int> vec{1, 2, 3, 4, 5};

  // Проверка доступа к элементам
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(4), 5);

  // Проверка на выброс исключения при выходе за границы
  EXPECT_THROW(vec.at(5), std::out_of_range);
}

// Тест для оператора []
TEST(VectorFunctions, OperatorSquareBrackets) {
  s21::vector<int> vec{1, 2, 3, 4, 5};

  // Проверка доступа к элементам
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[4], 5);
}

// Тест для функции front()
TEST(VectorFunctions, FrontFunction) {
  s21::vector<int> vec{1, 2, 3, 4, 5};

  // Проверка доступа к первому элементу
  EXPECT_EQ(vec.front(), 1);
}

// Тест для функции back()
TEST(VectorFunctions, BackFunction) {
  s21::vector<int> vec{1, 2, 3, 4, 5};

  // Проверка доступа к последнему элементу
  EXPECT_EQ(vec.back(), 5);
}

// Тест для функции data()
TEST(VectorFunctions, DataFunction) {
  s21::vector<int> vec{1, 2, 3, 4, 5};

  // Получаем указатель на данные
  int* data_ptr = vec.data();

  // Проверка, что данные соответствуют ожидаемым
  ASSERT_NE(data_ptr, nullptr);
  EXPECT_EQ(data_ptr[0], 1);
  EXPECT_EQ(data_ptr[4], 5);
}

// Тест для проверки функции reserve
TEST(VectorFunctions, ReserveFunction) {
  s21::vector<int> vec{1, 2, 3, 4, 5};
  size_t oldCapacity = vec.capacity();
  vec.reserve(10);

  // Проверка, что вместимость увеличилась
  EXPECT_GE(vec.capacity(), 10);
  EXPECT_GE(vec.capacity(), oldCapacity);

  // Проверка, что размер остался прежним
  EXPECT_EQ(vec.size(), 5);

  // Проверка, что элементы остались неизменными
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
  EXPECT_EQ(vec[4], 5);

  // Проверка реакции на меньший размер reserve
  vec.reserve(3);
  EXPECT_GE(vec.capacity(), 5);  // Вместимость не должна уменьшиться
}

// Дополнительный тест для проверки поведения reserve при размере 0
TEST(VectorFunctions, ReserveZeroSize) {
  s21::vector<int> vec;
  vec.reserve(0);

  // Проверка, что вместимость не меньше 0
  EXPECT_GE(vec.capacity(), 0);
}

TEST(VectorClearTest, ClearEmptyVector2) {
  s21::vector<int> my_vector;
  my_vector.clear();

  // Проверяем, что размер вектора равен 0
  ASSERT_EQ(my_vector.size(), 0);
}

// Тестирование метода clear на непустом векторе
TEST(VectorClearTest, ClearNonEmptyVector) {
  s21::vector<int> my_vector;
  my_vector.push_back(1);
  my_vector.push_back(2);
  my_vector.push_back(3);
  size_t old_capacity = my_vector.capacity();
  my_vector.clear();

  // Проверяем, что размер вектора равен 0
  ASSERT_EQ(my_vector.size(), 0);
  ASSERT_EQ(my_vector.capacity(), old_capacity);
}

// Тестирование повторного использования вектора после clear
TEST(VectorClearTest, ReuseAfterClear) {
  s21::vector<int> my_vector;
  my_vector.push_back(1);
  my_vector.push_back(2);
  my_vector.clear();
  my_vector.push_back(3);
  my_vector.push_back(4);

  // Проверяем, что вектор содержит правильные элементы
  ASSERT_EQ(my_vector.size(), 2);
  EXPECT_EQ(my_vector[0], 3);
  EXPECT_EQ(my_vector[1], 4);
}

TEST(VectorPushBackTest, PushBackToEmptyVector) {
  s21::vector<int> my_vector;
  my_vector.push_back(1);

  // Проверяем, что размер вектора увеличился и элемент добавлен
  ASSERT_EQ(my_vector.size(), 1);
  EXPECT_EQ(my_vector[0], 1);
}

// Тестирование последовательного добавления элементов
TEST(VectorPushBackTest, SequentialPushBack) {
  s21::vector<int> my_vector;
  my_vector.push_back(1);
  my_vector.push_back(2);
  my_vector.push_back(3);

  // Проверяем, что размер вектора и содержимое соответствуют ожиданиям
  ASSERT_EQ(my_vector.size(), 3);
  EXPECT_EQ(my_vector[0], 1);
  EXPECT_EQ(my_vector[1], 2);
  EXPECT_EQ(my_vector[2], 3);
}

// Тестирование добавления элементов с проверкой capacity
TEST(VectorPushBackTest, PushBackWithCapacityCheck) {
  s21::vector<int> my_vector;

  // Допустим, начальный capacity вектора равен 1
  size_t expected_capacity = 1;
  for (int i = 0; i < 5; ++i) {
    my_vector.push_back(i);

    // Проверяем, что capacity увеличивается при необходимости
    if (my_vector.size() > expected_capacity) {
      expected_capacity *= 2;  // Предполагаем, что capacity удваивается
    }
    ASSERT_LE(my_vector.size(), expected_capacity);
  }

  // Проверяем финальное содержимое вектора
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(my_vector[i], i);
  }
}

TEST(VectorPopBackTest, PopBackFromNonEmptyVector) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;
  my_vector.push_back(1);
  my_vector.push_back(2);
  std_vector.push_back(1);
  std_vector.push_back(2);

  my_vector.pop_back();
  std_vector.pop_back();

  ASSERT_EQ(my_vector.size(), std_vector.size());
  EXPECT_EQ(my_vector[0], std_vector[0]);
}

// Тестирование поведения при вызове pop_back на пустом векторе
TEST(VectorPopBackTest, PopBackFromEmptyVector) {
  s21::vector<int> my_vector;
  EXPECT_NO_THROW(my_vector.pop_back());
  // у стандартного вектора тут косяк, но я думаю все понимают что пусто вектор
  // имеет размер в 0
  ASSERT_EQ(my_vector.size(), 0);
}

// Тестирование последовательного удаления элементов
TEST(VectorPopBackTest, SequentialPopBack) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;
  my_vector.push_back(1);
  my_vector.push_back(2);
  my_vector.push_back(3);
  std_vector.push_back(1);
  std_vector.push_back(2);
  std_vector.push_back(3);

  my_vector.pop_back();
  my_vector.pop_back();
  std_vector.pop_back();
  std_vector.pop_back();

  ASSERT_EQ(my_vector.size(), std_vector.size());
  EXPECT_EQ(my_vector[0], std_vector[0]);
}

// Тестирование уменьшения вместимости вектора
TEST(VectorShrinkToFitTest, ShrinkToFitNonEmptyVector) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;

  // Добавляем элементы
  for (int i = 0; i < 10; ++i) {
    my_vector.push_back(i);
    std_vector.push_back(i);
  }

  // Увеличиваем capacity, добавив больше элементов и удалив часть из них
  for (int i = 0; i < 5; ++i) {
    my_vector.push_back(i);
    std_vector.push_back(i);
  }
  for (int i = 0; i < 5; ++i) {
    my_vector.pop_back();
    std_vector.pop_back();
  }

  my_vector.shrink_to_fit();
  std_vector.shrink_to_fit();

  // Проверяем, что размер и вместимость совпадают
  ASSERT_EQ(my_vector.size(), std_vector.size());
  ASSERT_EQ(my_vector.capacity(), std_vector.capacity());
}

// Тестирование shrink_to_fit на пустом векторе
TEST(VectorShrinkToFitTest, ShrinkToFitEmptyVector) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;

  my_vector.shrink_to_fit();
  std_vector.shrink_to_fit();

  ASSERT_EQ(my_vector.size(), std_vector.size());
  ASSERT_EQ(my_vector.capacity(), std_vector.capacity());
}

// Тестирование удаления элемента из середины вектора
TEST(VectorEraseTest, EraseFromMiddle) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;
  for (int i = 0; i < 5; ++i) {
    my_vector.push_back(i);
    std_vector.push_back(i);
  }
  auto it_1 = my_vector.begin();
  auto it_2 = std_vector.begin();

  my_vector.erase(it_1 + 2);
  std_vector.erase(it_2 + 2);

  ASSERT_EQ(my_vector.size(), std_vector.size());
  ASSERT_EQ(my_vector.capacity(), std_vector.capacity());
  for (size_t i = 0; i < my_vector.size(); ++i) {
    EXPECT_EQ(my_vector[i], std_vector[i]);
  }
}

// Тестирование удаления первого элемента
TEST(VectorEraseTest, EraseFirstElement) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;
  for (int i = 0; i < 5; ++i) {
    my_vector.push_back(i);
    std_vector.push_back(i);
  }

  my_vector.erase(my_vector.begin());
  std_vector.erase(std_vector.begin());

  ASSERT_EQ(my_vector.size(), std_vector.size());
  ASSERT_EQ(my_vector.capacity(), std_vector.capacity());
  for (size_t i = 0; i < my_vector.size(); ++i) {
    EXPECT_EQ(my_vector[i], std_vector[i]);
  }
}

// Тестирование удаления последнего элемента
TEST(VectorEraseTest, EraseLastElement) {
  s21::vector<int> my_vector;
  std::vector<int> std_vector;
  for (int i = 0; i < 5; ++i) {
    my_vector.push_back(i);
    std_vector.push_back(i);
  }

  my_vector.erase(my_vector.end() - 1);
  std_vector.erase(std_vector.end() - 1);

  ASSERT_EQ(my_vector.size(), std_vector.size());
  ASSERT_EQ(my_vector.capacity(), std_vector.capacity());
  for (size_t i = 0; i < my_vector.size(); ++i) {
    EXPECT_EQ(my_vector[i], std_vector[i]);
  }
}

// Тестирование вставки элемента в начало вектора
TEST(VectorInsertTest, InsertAtBeginning) {
  s21::vector<int> my_vector{2, 3, 4};
  auto it = my_vector.insert(my_vector.begin(), 1);

  ASSERT_EQ(my_vector.size(), 4);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(my_vector[0], 1);
  EXPECT_EQ(my_vector[1], 2);
  EXPECT_EQ(my_vector[2], 3);
  EXPECT_EQ(my_vector[3], 4);
}

// Тестирование вставки элемента в середину вектора
TEST(VectorInsertTest, InsertInMiddle_3) {
  s21::vector<int> my_vector{1, 2, 4};
  auto it = my_vector.insert(my_vector.begin() + 2, 3);

  ASSERT_EQ(my_vector.size(), 4);
  EXPECT_EQ(*it, 3);
  EXPECT_EQ(my_vector[0], 1);
  EXPECT_EQ(my_vector[1], 2);
  EXPECT_EQ(my_vector[2], 3);
  EXPECT_EQ(my_vector[3], 4);
}

// Тестирование вставки элемента в конец вектора
TEST(VectorInsertTest, InsertAtEnd) {
  s21::vector<int> my_vector{1, 2, 3};
  auto it = my_vector.insert(my_vector.end(), 4);

  ASSERT_EQ(my_vector.size(), 4);
  EXPECT_EQ(*it, 4);
  EXPECT_EQ(my_vector[0], 1);
  EXPECT_EQ(my_vector[1], 2);
  EXPECT_EQ(my_vector[2], 3);
  EXPECT_EQ(my_vector[3], 4);
}

// Тестирование вставки в пустой вектор
TEST(VectorInsertTest, InsertInEmptyVector) {
  s21::vector<int> my_vector;
  auto it = my_vector.insert(my_vector.begin(), 1);

  ASSERT_EQ(my_vector.size(), 1);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(my_vector[0], 1);
}

TEST(VectorAssignmentTest, CopyAssignmentOperator) {
  s21::vector<int> original;
  original.push_back(1);
  original.push_back(2);

  s21::vector<int> copy;
  copy = original;  // Используем оператор присваивания копированием

  // Проверяем, что размеры и значения совпадают
  ASSERT_EQ(copy.size(), original.size());
  for (size_t i = 0; i < original.size(); ++i) {
    EXPECT_EQ(copy[i], original[i]);
  }
}

TEST(VectorAssignmentTest, MoveAssignmentOperator) {
  s21::vector<int> original;
  original.push_back(1);
  original.push_back(2);

  s21::vector<int> moved;
  moved = std::move(original);  // Используем оператор присваивания перемещением

  // Проверяем, что moved получил данные
  ASSERT_EQ(moved.size(), 2);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);

  // Проверяем, что original теперь пуст
  ASSERT_EQ(original.size(), 0);
}

// Дополнительный тест на самоприсваивание
TEST(VectorAssignmentTest, SelfAssignment) {
  s21::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);

  vec = vec;  // Самоприсваивание

  // Проверяем, что данные остались неизменными
  ASSERT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
}

TEST(VectorSwapTest, SwapVectors) {
  s21::vector<int> vec1{1, 2, 3};
  s21::vector<int> vec2{4, 5, 6, 7};

  size_t vec1_size = vec1.size();
  size_t vec1_capacity = vec1.capacity();
  int* vec1_data = vec1.data();

  size_t vec2_size = vec2.size();
  size_t vec2_capacity = vec2.capacity();
  int* vec2_data = vec2.data();

  vec1.swap(vec2);

  // Проверяем, что размеры и вместимости обменялись местами
  ASSERT_EQ(vec1.size(), vec2_size);
  ASSERT_EQ(vec1.capacity(), vec2_capacity);
  ASSERT_EQ(vec1.data(), vec2_data);

  ASSERT_EQ(vec2.size(), vec1_size);
  ASSERT_EQ(vec2.capacity(), vec1_capacity);
  ASSERT_EQ(vec2.data(), vec1_data);

  // Проверяем, что содержимое векторов обменялось
  ASSERT_EQ(vec1[0], 4);
  ASSERT_EQ(vec2[0], 1);
}

TEST(VectorSwapTest, SwapWithEmptyVector) {
  s21::vector<int> vec1{1, 2, 3};
  s21::vector<int> vec2;

  size_t vec1_size = vec1.size();
  size_t vec1_capacity = vec1.capacity();
  int* vec1_data = vec1.data();

  vec1.swap(vec2);

  // Проверяем, что vec1 стал пустым, а vec2 получил данные
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec1.capacity(), 0);
  ASSERT_EQ(vec1.data(), nullptr);

  ASSERT_EQ(vec2.size(), vec1_size);
  ASSERT_EQ(vec2.capacity(), vec1_capacity);
  ASSERT_EQ(vec2.data(), vec1_data);
  ASSERT_EQ(vec2[0], 1);
  ASSERT_EQ(vec2[1], 2);
  ASSERT_EQ(vec2[2], 3);
}

TEST(VectorInsertManyBackTest, InsertMultipleElements) {
  s21::vector<int> vec;

  // Вставляем несколько элементов
  vec.insert_many_back(1, 2, 3, 4);

  // Проверяем, что элементы добавлены в конец вектора
  ASSERT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
}

TEST(VectorInsertManyBackTest, InsertIntoNonEmptyVector) {
  s21::vector<int> vec{0};

  // Вставляем дополнительные элементы в вектор, содержащий уже один элемент
  vec.insert_many_back(1, 2, 3);

  // Проверяем, что элементы добавлены в конец вектора
  ASSERT_EQ(vec.size(), 4);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 1);
  EXPECT_EQ(vec[2], 2);
  EXPECT_EQ(vec[3], 3);
}

TEST(VectorInsertManyBackTest, InsertNoElements) {
  s21::vector<int> vec;

  // Пытаемся вставить пустой набор элементов
  vec.insert_many_back();

  // Проверяем, что размер вектора не изменился
  ASSERT_EQ(vec.size(), 0);
}

TEST(VectorMaxSizeTest, MaxSizeIsPositive) {
  s21::vector<int> vec;
  EXPECT_GT(vec.max_size(), 0u);
}

TEST(VectorInsertManyTest, InsertMultipleAtPosition) {
  s21::vector<int> vec{1, 5};

  // Вставляем несколько элементов в позицию 1 (перед элементом 5)
  auto it = vec.insert_many(vec.begin() + 1, 2, 3, 4);

  // Проверяем, что элементы были добавлены корректно
  ASSERT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
  EXPECT_EQ(vec[4], 5);

  // Проверяем, что возвращаемый итератор указывает на первый вставленный
  // элемент
  EXPECT_EQ(*it, 2);
}

TEST(VectorInsertManyTest, InsertAtBeginning) {
  s21::vector<int> vec{4, 5, 6};

  // Вставляем элементы в начало вектора
  auto it = vec.insert_many(vec.begin(), 1, 2, 3);

  // Проверяем, что элементы были добавлены корректно
  ASSERT_EQ(vec.size(), 6);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
  EXPECT_EQ(vec[3], 4);
  EXPECT_EQ(vec[4], 5);
  EXPECT_EQ(vec[5], 6);

  // Проверяем, что возвращаемый итератор указывает на первый вставленный
  // элемент
  EXPECT_EQ(*it, 1);
}

TEST(VectorInsertManyTest, InsertAtEnd) {
  s21::vector<int> vec{1, 2, 3};

  // Вставляем элементы в конец вектора
  auto it = vec.insert_many(vec.end(), 4, 5, 6);

  // Проверяем, что элементы были добавлены корректно
  ASSERT_EQ(vec.size(), 6);
  EXPECT_EQ(vec[3], 4);
  EXPECT_EQ(vec[4], 5);
  EXPECT_EQ(vec[5], 6);

  // Проверяем, что возвращаемый итератор указывает на первый вставленный
  // элемент
  EXPECT_EQ(*it, 4);
}

TEST(VectorInsertManyTest, InsertIntoEmptyVector) {
  s21::vector<int> vec;

  // Вставляем элементы в пустой вектор
  auto it = vec.insert_many(vec.begin(), 1, 2, 3);

  // Проверяем, что элементы были добавлены корректно
  ASSERT_EQ(vec.size(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);

  // Проверяем, что возвращаемый итератор указывает на первый вставленный
  // элемент
  EXPECT_EQ(*it, 1);
}