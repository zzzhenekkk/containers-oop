#include <gtest/gtest.h>

#include <iterator>
#include <map>
#include <stdexcept>
#include <vector>

#include "../lib/s21_map.h"
using namespace s21;

// Тест на проверку конструктора по умолчанию
TEST(mapConstructor, DefaultConstructor) {
  s21::map<int, std::string> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

TEST(mapCopyConstructor, InsertAndEq) {
  s21::map<int, std::string> map;

  map.insert(std::make_pair(1, "one"));
  map.insert(std::make_pair(2, "two"));
  map.insert(std::make_pair(3, "three"));

  s21::map<int, std::string> map2(map);
  EXPECT_EQ(map.size(), 3);
  EXPECT_EQ(map2.size(), 3);

  for (auto it = map.begin(), it2 = map2.begin(); it != map.end();
       ++it, ++it2) {
    EXPECT_EQ(*it, *it2);
  }

  EXPECT_EQ((*map.find(1)).second, "one");
  EXPECT_EQ((*map.find(2)).second, "two");
  EXPECT_EQ((*map.find(3)).second, "three");
}

TEST(mapTest, AccessExistingElement) {
  map<int, std::string> testmap;
  testmap.insert(std::make_pair(1, "One"));
  testmap.insert(std::make_pair(2, "Two"));
  testmap.insert(std::make_pair(3, "Three"));

  EXPECT_EQ(testmap.at(1), "One");
  EXPECT_EQ(testmap.at(2), "Two");
  EXPECT_EQ(testmap.at(3), "Three");
}

TEST(mapTest, ThrowExceptionForNonExistingElement) {
  map<int, std::string> testmap;
  testmap.insert(std::make_pair(1, "One"));

  EXPECT_THROW(testmap.at(2), std::out_of_range);
}

TEST(mapTest, AccessModifyExistingElement) {
  map<int, std::string> testmap;
  testmap.insert(std::make_pair(1, "One"));
  testmap.insert(std::make_pair(2, "Two"));

  testmap[1] = "ONE";
  testmap[2] = "TWO";
  EXPECT_EQ(testmap[1], "ONE");
  EXPECT_EQ(testmap[2], "TWO");
}

TEST(mapTest, InsertNewElementWithDefaultValue) {
  map<int, std::string> testmap;
  testmap[1] = "One";

  ASSERT_NO_THROW({
    std::string value = testmap[1];
    EXPECT_EQ(value, "One");
  });

  testmap[2];
  EXPECT_EQ(testmap[2], "");
}

TEST(mapEraseTest, EraseByIterator) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  map.insert(std::make_pair(2, "Two"));
  map.insert(std::make_pair(3, "Three"));

  auto it = map.find(2);
  EXPECT_NE(it, map.end());
  map.erase(it);

  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.find(2), map.end());

  auto itOne = map.find(1);
  EXPECT_NE(itOne, map.end());
  EXPECT_EQ((*itOne).second, "One");

  auto itThree = map.find(3);
  EXPECT_NE(itThree, map.end());
  EXPECT_EQ((*itThree).second, "Three");
}

TEST(mapEraseTest, EraseNonExistentElement) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  s21::map<int, std::string>::Iterator it = map.end();

  EXPECT_NO_THROW(map.erase(it));
  EXPECT_EQ(map.size(), 1);  // Размер не изменился
}

TEST(mapEraseTest, IteratorValidityAfterErase) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  map.insert(std::make_pair(2, "Two"));
  map.insert(std::make_pair(3, "Three"));

  auto it = map.find(2);
  map.erase(it);

  std::vector<int> keys;
  for (auto it = map.begin(); it != map.end(); ++it) {
    keys.push_back((*it).first);
  }

  std::vector<int> expectedKeys = {1, 3};
  EXPECT_EQ(keys, expectedKeys);
}

TEST(TreeEraseTest, EraseNodeWithTwoChildren) {
  s21::map<int, std::string> map;
  map.insert({5, "Root"});
  map.insert({3, "Left"});
  map.insert({7, "Right"});
  map.insert({2, "Left-Left"});
  map.insert({4, "Left-Right"});
  map.insert({6, "Right-Left"});
  map.insert({8, "Right-Right"});

  EXPECT_EQ(map.size(), 7);

  auto it = map.find(5);
  ASSERT_NE(it, map.end());

  map.erase(it);

  EXPECT_EQ(map.size(), 6);
  EXPECT_EQ(map.find(5), map.end());

  auto newRootIt = map.find(6);
  ASSERT_NE(newRootIt, map.end());
  EXPECT_EQ((*newRootIt).second, "Right-Left");

  EXPECT_NE(map.find(2), map.end());
  EXPECT_NE(map.find(3), map.end());
  EXPECT_NE(map.find(4), map.end());
  EXPECT_NE(map.find(7), map.end());
  EXPECT_NE(map.find(8), map.end());
}

// Тест на проверку работы функции clear() в пустом map
TEST(mapClearTest, ClearEmptymap) {
  s21::map<int, std::string> map;
  map.clear();
  EXPECT_EQ(map.size(), 0);
}

// Тест на проверку работы функции clear() в не пустом map
TEST(mapClearTest, ClearNonEmptymap) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  map.insert(std::make_pair(2, "Two"));
  map.insert(std::make_pair(3, "Three"));

  EXPECT_EQ(map.size(), 3);
  map.clear();
  EXPECT_EQ(map.size(), 0);
}

// Тест на проверку состояния map после очистки
TEST(mapClearTest, StateAfterClear) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  map.clear();

  EXPECT_THROW(map.at(1), std::out_of_range);
}

// Тест проверяет, что содержимое двух map корректно меняется местами
TEST(mapSwapTest, CorrectSwapContent) {
  map<int, std::string> map1{{1, "one"}, {2, "two"}};
  map<int, std::string> map2{{3, "three"}, {4, "four"}};

  auto originalmap1 = map1;
  auto originalmap2 = map2;

  map1.swap(map2);

  for (const auto& pair : map1) {
    EXPECT_EQ(pair.second, originalmap2[pair.first]);
  }

  for (const auto& pair : map2) {
    EXPECT_EQ(pair.second, originalmap1[pair.first]);
  }
}

// Тест проверяет, что размеры map меняются корректно после swap
TEST(mapSwapTest, CorrectSwapSize) {
  map<int, std::string> map1{{1, "one"}, {2, "two"}};
  map<int, std::string> map2{{3, "three"}, {4, "four"}, {5, "five"}};

  // Сохраняем размеры до swap
  auto sizemap1BeforeSwap = map1.size();
  auto sizemap2BeforeSwap = map2.size();

  // Выполняем swap
  map1.swap(map2);

  // Проверяем, что размеры поменялись корректно
  EXPECT_EQ(map1.size(), sizemap2BeforeSwap);
  EXPECT_EQ(map2.size(), sizemap1BeforeSwap);
}

TEST(mapMergeTest, MergeWithDuplicateKeys) {
  std::map<int, std::string> stdmap1{
      {1, "A"}, {2, "B"}, {3, "C"}, {4, "E"}, {5, "F"}};
  std::map<int, std::string> stdmap2{{3, "D"}};

  s21::map<int, std::string> s21map1{{1, "A"}, {2, "B"}, {3, "C"}};
  s21::map<int, std::string> s21map2{{3, "D"}, {4, "E"}, {5, "F"}};

  s21map1.merge(s21map2);

  EXPECT_EQ(stdmap1.size(), s21map1.size());

  for (const auto& pair : stdmap1) {
    auto it = s21map1.find(pair.first);
    ASSERT_NE(it, s21map1.end());
    EXPECT_EQ(it->second, pair.second);
  }

  EXPECT_EQ(stdmap2.size(), s21map2.size());
  for (const auto& pair : stdmap2) {
    auto it = s21map2.find(pair.first);
    ASSERT_NE(it, s21map2.end());
    EXPECT_EQ(it->second, pair.second);
  }
}

// Тест на проверку наличия элемента
TEST(mapContainsTest, ContainsExistingKey) {
  s21::map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.insert({3, "three"});

  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_TRUE(map.contains(3));
}

// Тест на проверку отсутствия элемента
TEST(mapContainsTest, DoesNotContainKey) {
  s21::map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.insert({3, "three"});

  EXPECT_FALSE(map.contains(4));
}

// Тест на проверку работы с пустым map
TEST(mapContainsTest, EmptymapDoesNotContainKey) {
  s21::map<int, std::string> map;

  EXPECT_FALSE(map.contains(1));
}

// Дополнительный тест на проверку содержания после удаления элемента
TEST(mapContainsTest, DoesNotContainKeyAfterRemoval) {
  s21::map<int, std::string> map;
  map.insert({1, "one"});
  map.insert({2, "two"});
  map.erase(map.find(2));

  EXPECT_TRUE(map.contains(1));
  EXPECT_FALSE(map.contains(2));
}

// Тест на вставку нового элемента
TEST(mapInsert, InsertNewElement) {
  s21::map<int, std::string> map;
  auto result = map.insert({1, "One"});
  EXPECT_TRUE(result.second);
  EXPECT_EQ(result.first->first, 1);
  EXPECT_EQ(result.first->second, "One");
  EXPECT_EQ(map.size(), 1);
}

// Тест на попытку вставки элемента с ключом, который уже существует
TEST(mapInsert, InsertDuplicateKey) {
  s21::map<int, std::string> map;
  map.insert({2, "Two"});
  auto result = map.insert({2, "Another Two"});

  EXPECT_FALSE(result.second);

  EXPECT_EQ(result.first->first, 2);
  EXPECT_EQ(result.first->second, "Two");
  EXPECT_EQ(map.size(), 1);
}

// Тест на вставку нескольких элементов
TEST(mapInsert, InsertMultipleElements) {
  s21::map<int, std::string> map;
  map.insert({3, "Three"});
  map.insert({1, "One"});
  auto result = map.insert({2, "Two"});

  EXPECT_TRUE(result.second);
  EXPECT_EQ(map.size(), 3);

  EXPECT_EQ(map.find(1)->second, "One");
  EXPECT_EQ(map.find(2)->second, "Two");
  EXPECT_EQ(map.find(3)->second, "Three");
}

// Тест на добавление нового элемента
TEST(mapInsertOrAssign, InsertNewElement) {
  s21::map<int, std::string> map;
  auto result = map.insert_or_assign(1, "One");
  EXPECT_TRUE(result.second);
  EXPECT_EQ(result.first->first, 1);
  EXPECT_EQ(result.first->second, "One");
  EXPECT_EQ(map.size(), 1);
}

// Тест на обновление значения существующего элемента
TEST(mapInsertOrAssign, AssignExistingElement) {
  s21::map<int, std::string> map;
  map.insert({1, "One"});
  auto result = map.insert_or_assign(1, "New One");

  EXPECT_FALSE(result.second);

  EXPECT_EQ(result.first->first, 1);
  EXPECT_EQ(result.first->second, "New One");
  EXPECT_EQ(map.size(), 1);  // Размер не должен измениться
}

// Тест на добавление нового элемента и обновление существующего
TEST(mapInsertOrAssign, InsertAndAssign) {
  s21::map<int, std::string> map;
  map.insert_or_assign(2, "Two");
  EXPECT_EQ(map.size(), 1);

  map.insert_or_assign(2, "New Two");
  EXPECT_EQ(map.size(), 1);
  EXPECT_EQ(map.find(2)->second, "New Two");

  map.insert_or_assign(3, "Three");
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map.find(3)->second, "Three");
}

TEST(mapMoveConstructor, Insert) {
  s21::map<int, std::string> map;

  map.insert(std::make_pair(1, "one"));
  map.insert(std::make_pair(2, "two"));
  map.insert(std::make_pair(3, "three"));

  s21::map<int, std::string> map2(std::move(map));
  EXPECT_EQ(map2.size(), 3);
  EXPECT_EQ(map.size(), 0);

  EXPECT_EQ(map2.find(1)->second, "one");
  EXPECT_EQ(map2.find(2)->second, "two");
  EXPECT_EQ(map2.find(3)->second, "three");
}

// Тест на обновление значения по существующему ключу
TEST(mapTest, UpdateValue) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  map[1] = "Uno";

  EXPECT_EQ(map.find(1)->second, "Uno");
}

// Тест на успешную вставку уникальных элементов
TEST(mapTest, InsertUniqueElements) {
  s21::map<int, std::string> container;
  auto results =
      container.insert_many(std::make_pair(1, "One"), std::make_pair(2, "Two"));

  ASSERT_EQ(results.size(), 2);
  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_EQ(container[1], "One");
  EXPECT_EQ(container[2], "Two");
}

// Тест на попытку вставить элементы с дублирующимися ключами
TEST(mapTest, InsertDuplicateKeys) {
  s21::map<int, std::string> container;
  container.insert(std::make_pair(1, "One"));

  auto results = container.insert_many(std::make_pair(1, "One Again"),
                                       std::make_pair(2, "Two"));

  ASSERT_EQ(results.size(), 2);
  EXPECT_FALSE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_EQ(container.size(), 2);
}

// Тест на размер карты после вставки
TEST(mapTest, SizeAfterInsert) {
  s21::map<int, std::string> map;
  map.insert(std::make_pair(1, "One"));
  map.insert(std::make_pair(2, "Two"));

  EXPECT_EQ(map.size(), 2);
}

TEST(MapInsertOrAssign, InsertNewElement) {
  s21::map<int, std::string> map;
  auto result = map.insert_or_assign(1, "one");

  ASSERT_TRUE(result.second);

  EXPECT_EQ(result.first->first, 1);
  EXPECT_EQ(result.first->second, "one");

  EXPECT_EQ(map.size(), 1);
}

TEST(MapInsertOrAssign, AssignExistingElement) {
  s21::map<int, std::string> map;
  map.insert({1, "one"});
  auto result = map.insert_or_assign(1, "uno");

  ASSERT_FALSE(result.second);

  EXPECT_EQ(result.first->first, 1);
  EXPECT_EQ(result.first->second, "uno");

  EXPECT_EQ(map.size(), 1);
}

TEST(MapInsertOrAssign, MultipleOperations) {
  s21::map<int, std::string> map;
  map.insert_or_assign(1, "one");
  map.insert_or_assign(2, "two");

  map.insert_or_assign(3, "three");
  auto result = map.insert_or_assign(2, "dos");

  ASSERT_FALSE(result.second);
  EXPECT_EQ(result.first->second, "dos");

  EXPECT_EQ(map.size(), 3);
  EXPECT_EQ(map[1], "one");
  EXPECT_EQ(map[2], "dos");
  EXPECT_EQ(map[3], "three");
}