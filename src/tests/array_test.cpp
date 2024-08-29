#include <gtest/gtest.h>

#include <array>
#include <iostream>

#include "../lib/s21_array.h"

TEST(ArrayTest, DefaultConstructor) {
  s21::array<int> myArray;  // Создаем экземпляр класса array из пространства
                            // имен s21 с пустым конструктором
  std::array<int, 0> origArray;

  EXPECT_EQ(myArray.size(), origArray.size());  // Проверяем, что размер равен 0
  EXPECT_EQ(myArray.max_size(),
            origArray.max_size());  // Проверяем, что вместимость равна 0
}

TEST(ArrayTest, SizeNConstructor) {
  s21::array<int, 10> myArray;  //
  std::array<int, 10> origArray;

  EXPECT_EQ(myArray.size(), origArray.size());
  EXPECT_EQ(myArray.max_size(), origArray.max_size());
}

TEST(ArrayTest, InitializerListConstructor) {
  s21::array<int> myArray{1, 2, 3, 4, 5};  // Использование initializer_list
  std::array<int, 5> originArray{1, 2, 3, 4, 5};
  // Проверка размера
  EXPECT_EQ(myArray.size(), originArray.size());

  // Проверка вместимости
  // В этом случае предполагаем, что вместимость не меньше размера
  EXPECT_EQ(myArray.max_size(), originArray.max_size());

  // Проверка значений элементов
  EXPECT_EQ(myArray[0], 1);
  EXPECT_EQ(myArray[1], 2);
  EXPECT_EQ(myArray[2], 3);
  EXPECT_EQ(myArray[3], 4);
  EXPECT_EQ(myArray[4], 5);
}

TEST(ArrayFunctions, AtFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  EXPECT_EQ(arr.at(0), 1);  // Доступ к первому элементу
  EXPECT_EQ(arr.at(4), 5);  // Доступ к последнему элементу
  // Ожидание исключения при выходе за границы
  EXPECT_THROW(arr.at(5), std::out_of_range);
}

// Тест для оператора []
TEST(ArrayFunctions, OperatorSquareBrackets) {
  s21::array<int> arr{1, 2, 3, 4, 5};

  EXPECT_EQ(arr[0], 1);  // Доступ к первому элементу
  EXPECT_EQ(arr[4], 5);  // Доступ к последнему элементу
  // Нет проверки на выход за границы, поведение неопределено
}

// Тест для функции front()
TEST(ArrayFunctions, FrontFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  EXPECT_EQ(arr.front(), 1);  // Доступ к первому элементу
}

// Тест для функции back()
TEST(ArrayFunctions, BackFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  EXPECT_EQ(arr.back(), 5);  // Доступ к последнему элементу
}

// Тест для функции data()
TEST(ArrayFunctions, DataFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  int *raw_ptr = arr.data();
  ASSERT_NE(raw_ptr, nullptr);  // Убедиться, что указатель не нулевой
  EXPECT_EQ(raw_ptr[0], 1);  // Доступ к первому элементу через указатель
  EXPECT_EQ(raw_ptr[4], 5);  // Доступ к последнему элементу через указатель
}

// Тест для проверки независимости копии от оригинала
TEST(ArrayCopyConstructor, CopyIsIndependentOfOriginal) {
  s21::array<int> original{1, 2, 3, 4, 5};
  s21::array<int> copy = original;  // Использование конструктора копирования

  // Изменение оригинального массива
  original[0] = 10;

  // Проверка, что изменения в оригинале не повлияли на копию
  EXPECT_NE(copy[0], original[0]);
  EXPECT_EQ(copy[0], 1);
}

TEST(ArrayMoveConstructor, MoveConstructorCreatesValidObject) {
  s21::array<int> original{1, 2, 3, 4, 5};
  s21::array<int> moved(
      std::move(original));  // Использование конструктора перемещения

  // Проверка, что moved содержит данные original
  EXPECT_EQ(moved.size(), 5);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[4], 5);

  // Проверка, что original находится в валидном, но неопределенном состоянии
  EXPECT_EQ(original.size(), 0);
  // Или original может иметь нулевой указатель данных
  // EXPECT_EQ(original.data(), nullptr);
}

// Тест для проверки оператора перемещения
TEST(ArrayMoveAssignment, MoveAssignmentCreatesValidObject) {
  s21::array<int> original{1, 2, 3, 4, 5};
  s21::array<int> moved;
  moved = std::move(original);  // Использование оператора перемещения

  // Проверка, что moved содержит данные original
  EXPECT_EQ(moved.size(), 5);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[4], 5);

  // Проверка, что original находится в валидном, но неопределенном состоянии
  EXPECT_EQ(original.size(), 0);
  // Или original может иметь нулевой указатель данных
  // EXPECT_EQ(original.data(), nullptr);
}

// Тест для метода swap
TEST(ArrayModifiers, SwapFunction) {
  s21::array<int> arr1{1, 2, 3};
  s21::array<int> arr2{4, 5, 6, 7};

  size_t size1_before = arr1.size();
  size_t size2_before = arr2.size();

  arr1.swap(arr2);

  // Проверка, что размеры массивов изменились
  EXPECT_EQ(arr1.size(), size2_before);
  EXPECT_EQ(arr2.size(), size1_before);

  // Проверка, что элементы массивов действительно поменялись местами
  EXPECT_EQ(arr1[0], 4);
  EXPECT_EQ(arr2[0], 1);
}

// Тест для метода fill
TEST(ArrayModifiers, FillFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  int fillValue = 9;

  arr.fill(fillValue);

  // Проверка, что все элементы массива теперь равны fillValue
  for (size_t i = 0; i < arr.size(); ++i) {
    EXPECT_EQ(arr[i], fillValue);
  }
}

// Тест для проверки метода begin()
TEST(ArrayIterators, BeginFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  // Получаем итератор на начало массива
  auto it = arr.begin();
  // Проверяем, что итератор указывает на первый элемент массива
  EXPECT_EQ(*it, 1);
}

// Тест для проверки метода end()
TEST(ArrayIterators, EndFunction) {
  s21::array<int> arr{1, 2, 3, 4, 5};

  // Получаем итератор на конец массива
  auto it = arr.end();
  // Перемещаем итератор назад на одну позицию, так как end() указывает на
  // элемент после последнего
  --it;
  // Проверяем, что итератор указывает на последний элемент массива
  EXPECT_EQ(*it, 5);
}

// Дополнительный тест для проверки обхода массива с помощью итераторов
TEST(ArrayIterators, IterationThroughArray) {
  s21::array<int> arr{1, 2, 3, 4, 5};
  int expected_value = 1;
  // Итерация по массиву и проверка значений
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    EXPECT_EQ(*it, expected_value++);
  }
}

// Тест для оператора присваивания копированием
TEST(ArrayAssignmentOperator, CopyAssignment) {
  s21::array<int> original{1, 2, 3};
  s21::array<int> copy;
  copy = original;  // Присваивание копированием

  // Проверка, что скопированный массив идентичен оригиналу
  EXPECT_EQ(copy.size(), original.size());
  for (size_t i = 0; i < copy.size(); ++i) {
    EXPECT_EQ(copy[i], original[i]);
  }

  // Проверка независимости копии от оригинала
  original[0] = 10;
  EXPECT_NE(copy[0], original[0]);
}

// Тест для оператора присваивания перемещением
TEST(ArrayAssignmentOperator, MoveAssignment) {
  s21::array<int> original{1, 2, 3};
  s21::array<int> moved;
  moved = std::move(original);  // Присваивание перемещением

  // Проверка, что перемещенный массив содержит данные оригинала
  EXPECT_EQ(moved.size(), 3);
  EXPECT_EQ(moved[0], 1);
  EXPECT_EQ(moved[1], 2);
  EXPECT_EQ(moved[2], 3);

  // Проверка, что оригинальный массив теперь пуст
  EXPECT_EQ(original.size(), 0);
}

// Тест для оператора присваивания копированием
TEST(ArrayAssignmentOperator, CopyAssignment2) {
  s21::array<int> original{1, 2, 3};
  s21::array<int> copy;
  copy = original;  // Присваивание копированием

  // Проверка, что скопированный массив идентичен оригиналу
  EXPECT_EQ(copy.size(), original.size());
  for (size_t i = 0; i < copy.size(); ++i) {
    EXPECT_EQ(copy[i], original[i]);
  }

  // Проверка независимости копии от оригинала
  original[0] = 10;
  EXPECT_NE(copy[0], original[0]);
}

// Тест для функции empty()
TEST(ArrayFunctions, EmptyFunction) {
  s21::array<int> arr1;  // Пустой массив
  EXPECT_TRUE(arr1.empty());

  s21::array<int> arr2{1, 2, 3};  // Непустой массив
  EXPECT_FALSE(arr2.empty());
}

// Тест для функции size()
TEST(ArrayFunctions, SizeFunction) {
  s21::array<int> arr1;  // Пустой массив
  EXPECT_EQ(arr1.size(), 0);

  s21::array<int> arr2{1, 2, 3};  // Непустой массив
  EXPECT_EQ(arr2.size(), 3);
}

// Тест для функции max_size()
TEST(ArrayFunctions, MaxSizeFunction) {
  s21::array<int> arr;
  // Проверяем, что max_size() возвращает значение, которое больше или равно
  // size()
  EXPECT_GE(arr.max_size(), arr.size());
}