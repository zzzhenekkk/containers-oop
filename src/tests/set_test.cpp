#include <gtest/gtest.h>

#include <iterator>

#include "../lib/s21_set.h"
using namespace s21;

TEST(setTest, DefaultConstructor) {
  s21::set<int> set;
  EXPECT_TRUE(set.empty());
}

TEST(setTest, InitializerListConstructor) {
  s21::set<int> set({1, 2, 3});
  EXPECT_EQ(set.size(), 3);
  EXPECT_TRUE(set.contains(1));
  EXPECT_TRUE(set.contains(2));
  EXPECT_TRUE(set.contains(3));
}

TEST(setTest, CopyConstructor) {
  s21::set<int> original({1, 2, 3});
  s21::set<int> copy(original);
  EXPECT_EQ(copy.size(), 3);
  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));
  EXPECT_TRUE(copy.contains(3));
}

TEST(setTest, MoveConstructor) {
  s21::set<int> original({1, 2, 3});
  s21::set<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), 3);
  EXPECT_TRUE(moved.contains(1));
  EXPECT_TRUE(moved.contains(2));
  EXPECT_TRUE(moved.contains(3));

  EXPECT_TRUE(original.empty());
}

TEST(setTest, MoveAssignmentOperator) {
  s21::set<int> original({1, 2, 3});
  s21::set<int> moved;
  moved = std::move(original);
  EXPECT_EQ(moved.size(), 3);
  EXPECT_TRUE(moved.contains(1));
  EXPECT_TRUE(moved.contains(2));
  EXPECT_TRUE(moved.contains(3));

  EXPECT_TRUE(original.empty());
}

TEST(S21setTest, BeginReturnsIteratorToBeginning) {
  s21::set<int> set;
  set.insert(10);
  set.insert(5);
  set.insert(15);

  auto it = set.begin();

  EXPECT_EQ(*it, 5);
}

TEST(S21setTest, EndReturnsIteratorToEnd) {
  s21::set<int> set;

  set.insert(10);
  set.insert(5);
  set.insert(15);

  int count = 0;
  for (auto it = set.begin(); it != set.end(); ++it) {
    ++count;
  }

  EXPECT_EQ(count, 3);
}

TEST(S21setIteratorTest, IteratorIncrement) {
  s21::set<int> set;
  set.insert(1);
  set.insert(2);
  set.insert(3);

  auto it = set.begin();

  EXPECT_EQ(*it, 1);

  ++it;
  EXPECT_EQ(*it, 2);

  ++it;
  EXPECT_EQ(*it, 3);

  ++it;
  EXPECT_EQ(it, set.end());

  ++it;
  EXPECT_EQ(it, set.begin());
}

TEST(S21setIteratorTest, IteratorDecrement) {
  s21::set<int> set;
  set.insert(2);
  set.insert(1);
  set.insert(3);

  auto it = set.end();

  --it;
  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);

  --it;
  EXPECT_EQ(it, set.end());

  --it;
  EXPECT_EQ(*it, 3);

  it--;
  EXPECT_EQ(*it, 2);

  it--;
  EXPECT_EQ(*it, 1);

  it--;
  EXPECT_EQ(it, set.end());

  it++;
  EXPECT_EQ(*it, 1);
  it++;
  it++;
  it++;
  it++;
}
TEST(S21setTest, ClearFunction) {
  s21::set<int> set;
  set.insert(1);
  set.insert(2);
  EXPECT_EQ(set.size(), 2);

  set.clear();
  EXPECT_TRUE(set.empty());
}

TEST(S21setTest, InsertFunction) {
  s21::set<int> set;
  auto [it, inserted] = set.insert(1);
  EXPECT_TRUE(inserted);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(set.size(), 1);

  auto result = set.insert(1);
  EXPECT_FALSE(result.second);
  EXPECT_EQ(set.size(), 1);
}

TEST(S21setTest, EraseFunction) {
  s21::set<int> set;
  set.insert(1);
  set.insert(2);
  set.insert(3);

  auto it = set.find(2);
  set.erase(it);
  EXPECT_EQ(set.size(), 2);
  EXPECT_EQ(set.find(2), set.end());
}

TEST(S21setTest, SwapFunction) {
  s21::set<int> set1;
  set1.insert(1);
  set1.insert(2);

  s21::set<int> set2;
  set2.insert(3);

  set1.swap(set2);

  EXPECT_EQ(set1.size(), 1);
  EXPECT_TRUE(set1.find(3) != set1.end());
  EXPECT_EQ(set2.size(), 2);
  EXPECT_TRUE(set2.find(1) != set2.end());
  EXPECT_TRUE(set2.find(2) != set2.end());
}

TEST(S21setTest, SwapFunction2) {
  s21::set<int> set1;
  set1.insert(1);
  set1.insert(2);

  s21::set<int> set2;
  set2.insert(3);

  set1.swap(set2);

  EXPECT_EQ(set1.size(), 1);
  EXPECT_TRUE(set1.find(3) != set1.end());
  EXPECT_EQ(set2.size(), 2);
  EXPECT_TRUE(set2.find(1) != set2.end());
  EXPECT_TRUE(set2.find(2) != set2.end());
}

// Тест на слияние двух непересекающихся множеств
TEST(SetMerge, NonOverlappingSets) {
  s21::set<int> set1;
  set1.insert(1);
  set1.insert(3);
  set1.insert(5);

  s21::set<int> set2;
  set2.insert(2);
  set2.insert(4);
  set2.insert(6);

  set1.merge(set2);

  for (int i = 1; i <= 6; i++) {
    EXPECT_TRUE(set1.contains(i));
  }

  EXPECT_TRUE(set2.empty());
}

// Тест на слияние двух множеств с пересекающимися элементами
TEST(SetMerge, OverlappingSets) {
  s21::set<int> set1;
  set1.insert(1);
  set1.insert(2);
  set1.insert(3);

  s21::set<int> set2;
  set2.insert(3);
  set2.insert(4);
  set2.insert(5);

  set1.merge(set2);

  for (int i = 1; i <= 5; i++) {
    EXPECT_TRUE(set1.contains(i));
  }

  EXPECT_EQ(set2.size(), 1);
  EXPECT_TRUE(set2.contains(3));
}

// Тест на слияние множества с самим собой
TEST(SetMerge, MergeWithItself) {
  s21::set<int> set1;
  set1.insert(1);
  set1.insert(2);

  set1.merge(set1);

  EXPECT_TRUE(set1.contains(1) && set1.contains(2));
  EXPECT_EQ(set1.size(), 2);
}

TEST(S21setTest, ContainsFunction) {
  s21::set<int> set;
  set.insert(10);
  set.insert(20);
  set.insert(30);

  EXPECT_TRUE(set.contains(20));

  EXPECT_FALSE(set.contains(40));
}

TEST(S21setTest, InsertManyFunction) {
  s21::set<int> set;

  auto results = set.insert_many(10, 20, 30, 40);

  EXPECT_EQ(results.size(), 4);

  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
  }

  EXPECT_TRUE(set.contains(10));
  EXPECT_TRUE(set.contains(20));
  EXPECT_TRUE(set.contains(30));
  EXPECT_TRUE(set.contains(40));

  auto moreResults = set.insert_many(20, 50);

  EXPECT_EQ(moreResults.size(), 2);

  EXPECT_FALSE(moreResults[0].second);

  EXPECT_TRUE(moreResults[1].second);

  EXPECT_TRUE(set.contains(50));
}
