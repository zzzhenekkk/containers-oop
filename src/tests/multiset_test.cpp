#include <gtest/gtest.h>

#include <iterator>
#include <set>

#include "../lib/s21_multiset.h"
using namespace s21;

TEST(multisetTest, DefaultConstructor) {
  s21::multiset<int> multiset;
  EXPECT_TRUE(multiset.empty());
}

TEST(multisetTest, InitializerListConstructor) {
  s21::multiset<int> multiset({1, 2, 3, 2});
  EXPECT_EQ(multiset.size(), 4);
}

TEST(multisetTest, CopyConstructor) {
  s21::multiset<int> original({1, 2, 3, 2});
  s21::multiset<int> copy(original);
  EXPECT_EQ(copy.size(), 4);
}

TEST(multisetTest, MoveConstructor) {
  s21::multiset<int> original({1, 2, 3, 2});
  s21::multiset<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), 4);
  EXPECT_TRUE(original.empty());
}

TEST(multisetTest, MoveAssignmentOperator) {
  s21::multiset<int> original({1, 2, 3, 2});
  s21::multiset<int> moved;
  moved = std::move(original);
  EXPECT_EQ(moved.size(), 4);
  EXPECT_TRUE(original.empty());
}

TEST(multisetTest, InsertFunction) {
  s21::multiset<int> multiset;
  auto it = multiset.insert(1);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(multiset.size(), 1);

  // Вставляем дубликат
  auto it2 = multiset.insert(1);
  EXPECT_EQ(*it2, 1);
  EXPECT_EQ(multiset.size(), 2);
}

TEST(multisetTest, EraseFunction) {
  s21::multiset<int> multiset({1, 2, 2, 3});
  auto it = multiset.find(2);
  multiset.erase(it);
  EXPECT_EQ(multiset.size(), 3);
}

TEST(multisetTest, ClearFunction) {
  s21::multiset<int> multiset({1, 2, 2, 3});
  multiset.clear();
  EXPECT_TRUE(multiset.empty());
}

TEST(S21multisetTest, MergeFunction) {
  s21::multiset<int> multiset1;
  multiset1.insert(1);
  multiset1.insert(2);
  multiset1.insert(2);

  s21::multiset<int> multiset2;
  multiset2.insert(3);
  multiset2.insert(4);
  multiset2.insert(1);

  size_t sizeBeforeMerge = multiset1.size() + multiset2.size();

  multiset1.merge(multiset2);

  EXPECT_EQ(multiset1.size(), sizeBeforeMerge);
  EXPECT_TRUE(multiset1.contains(3));
  EXPECT_TRUE(multiset1.contains(4));
  EXPECT_TRUE(multiset2.empty());
}

// Тест на отсутствие элемента
TEST(multisetCountTest, ElementNotPresent) {
  s21::multiset<int> multiset;
  multiset.insert(1);
  multiset.insert(2);
  ASSERT_EQ(multiset.count(3), 0);
}

// Тест на наличие элемента ровно один раз
TEST(multisetCountTest, ElementPresentOnce) {
  s21::multiset<int> multiset;
  multiset.insert(1);
  multiset.insert(2);
  ASSERT_EQ(multiset.count(1), 1);
}

// Тест на наличие элемента несколько раз
TEST(multisetCountTest, ElementPresentMultipleTimes) {
  s21::multiset<int> multiset;
  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(2);
  ASSERT_EQ(multiset.count(1), 2);
}

// Тест на поиск существующего элемента
TEST(multisetFindTest, FindExistingElement) {
  s21::multiset<int> mset;
  mset.insert(10);
  mset.insert(20);
  mset.insert(20);
  mset.insert(30);

  auto it = mset.find(20);
  ASSERT_NE(it, mset.end());
  EXPECT_EQ(*it, 20);
}

// Тест на поиск несуществующего элемента
TEST(multisetFindTest, FindNonExistingElement) {
  s21::multiset<int> mset;
  mset.insert(10);
  mset.insert(20);
  mset.insert(30);

  auto it = mset.find(40);
  EXPECT_EQ(it, mset.end());
}

// Тест на поиск элемента в пустом multiset
TEST(multisetFindTest, FindInEmptymultiset) {
  s21::multiset<int> mset;

  auto it = mset.find(10);
  EXPECT_EQ(it, mset.end());
}

// Тест на поиск первого вхождения элемента, если их несколько
TEST(multisetFindTest, FindFirstOccurrence) {
  s21::multiset<int> mset;
  mset.insert(20);
  mset.insert(20);
  mset.insert(20);

  auto it = mset.find(20);
  ASSERT_NE(it, mset.end());
  EXPECT_EQ(*it, 20);

  ++it;
  EXPECT_EQ(*it, 20);
}

// Тест на проверку наличия элементов с определенным ключом
TEST(multisetTest, EqualRangePresentMultipleTimes) {
  s21::multiset<int> mset = {1, 2, 2, 3, 4, 4, 4, 5};

  auto range = mset.equal_range(4);

  ASSERT_NE(range.first, range.second);

  int count = 0;
  for (auto it = range.first; it != range.second; ++it) {
    EXPECT_EQ(*it, 4);
    ++count;
  }

  EXPECT_EQ(count, 3);
}

// Тест на проверку наличия элемента с уникальным ключом
TEST(multisetTest, EqualRangePresentOnce) {
  s21::multiset<int> mset = {1, 2, 3, 4, 5};

  auto range = mset.equal_range(3);

  ASSERT_NE(range.first, range.second);
  EXPECT_EQ(*range.first, 3);
}

// Тест на проверку отсутствия элемента
TEST(multisetTest, EqualRangeAbsent) {
  s21::multiset<int> mset = {1, 2, 3, 4, 5};

  auto range = mset.equal_range(6);

  EXPECT_EQ(range.first, range.second);
}

TEST(S21multiset, LowerBound) {
  s21::multiset<int> mset = {1, 2, 2, 4, 5};

  auto it = mset.lower_bound(2);
  EXPECT_EQ(*it, 2);

  it = mset.lower_bound(3);
  EXPECT_EQ(*it, 4);

  it = mset.lower_bound(6);
  EXPECT_TRUE(it == mset.end());
}

TEST(S21multiset, UpperBound) {
  s21::multiset<int> mset = {1, 2, 2, 4, 5};

  auto it = mset.upper_bound(2);
  EXPECT_EQ(*it, 4);

  it = mset.upper_bound(3);
  EXPECT_EQ(*it, 4);

  it = mset.upper_bound(5);
  EXPECT_TRUE(it == mset.end());
}

// Тест на успешную вставку уникальных элементов
TEST(MultisetInsertMany, UniqueElements) {
  s21::multiset<int> mset;
  auto results = mset.insert_many(1, 2, 3);

  ASSERT_EQ(results.size(), 3);
  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
    EXPECT_TRUE(mset.find(*result.first) != mset.end());
  }
}

// Тест на вставку с дубликатами
TEST(MultisetInsertMany, DuplicateElements) {
  s21::multiset<int> mset;
  auto results = mset.insert_many(4, 4, 5);

  ASSERT_EQ(results.size(), 3);
  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);

  EXPECT_EQ(mset.count(4), 2);
}

// Тест на вставку смешанных элементов
TEST(MultisetInsertMany, MixedElements) {
  s21::multiset<int> mset;
  mset.insert(6);
  auto results = mset.insert_many(6, 7, 6, 8);

  ASSERT_EQ(results.size(), 4);
  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_TRUE(results[3].second);

  EXPECT_EQ(mset.count(6), 3);
}

// Тест на слияние двух непересекающихся мультимножеств
TEST(MultisetMerge, NonOverlappingSets) {
  s21::multiset<int> mset1;
  mset1.insert(1);
  mset1.insert(3);
  mset1.insert(5);

  s21::multiset<int> mset2;
  mset2.insert(2);
  mset2.insert(4);
  mset2.insert(6);

  mset1.merge(mset2);

  for (int i = 1; i <= 6; i++) {
    EXPECT_GT(mset1.count(i), 0);
  }

  EXPECT_TRUE(mset2.empty());
}

// Тест на слияние двух мультимножеств с пересекающимися элементами
TEST(MultisetMerge, OverlappingSets) {
  s21::multiset<int> mset1;
  mset1.insert(1);
  mset1.insert(2);
  mset1.insert(3);

  s21::multiset<int> mset2;
  mset2.insert(3);
  mset2.insert(3);
  mset2.insert(4);
  mset2.insert(5);

  mset1.merge(mset2);

  for (int i = 1; i <= 5; i++) {
    EXPECT_GT(mset1.count(i), 0);
  }
  EXPECT_EQ(mset1.count(3), 3);

  EXPECT_TRUE(mset2.empty());
}
