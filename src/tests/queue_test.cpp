#include <gtest/gtest.h>

#include <iostream>

#include "../lib/s21_queue.h"

TEST(Queue, Constructor_Default) {
  s21::queue<int> s21_queue;

  EXPECT_EQ(s21_queue.size(), 0);
  EXPECT_EQ(s21_queue.empty(), true);
  EXPECT_THROW(s21_queue.front(), std::range_error);
}

TEST(Queue, InitializerQueueConstructorBaseTest) {
  s21::queue<int> s21_queue = {1, 2, 3, 4, 5};

  EXPECT_EQ(s21_queue.size(), 5);

  size_t index = 0;
  for (int value : {1, 2, 3, 4, 5}) {
    EXPECT_EQ(s21_queue[index], value);
    ++index;
  }
}

TEST(Queue, InitializerQueueConstructorEmpty) {
  s21::queue<int> s21_queue = {};

  EXPECT_EQ(s21_queue.empty(), true);
  EXPECT_EQ(s21_queue.size(), 0);
}

TEST(Queue, CopyConstructorCreatesExactCopy) {
  s21::queue<int> originalQueue = {1, 2, 3, 4, 5};

  s21::queue<int> copiedQueue(originalQueue);

  EXPECT_EQ(originalQueue.size(), copiedQueue.size());
  for (size_t index = 0; index < copiedQueue.size(); ++index) {
    EXPECT_EQ(originalQueue[index], copiedQueue[index]);
  }
}

TEST(Queue, ModificationsToCopyDoNotAffectOriginal) {
  s21::queue<int> originalQueue = {1, 2, 3};
  s21::queue<int> copiedQueue(originalQueue);

  copiedQueue[0] = 0;
  EXPECT_EQ(originalQueue[0], 1);
  EXPECT_EQ(copiedQueue[0], 0);
}

TEST(Queue, MoveConstructorCanMoveQueue) {
  s21::queue<int> temp = {1, 2, 3, 4, 5};

  s21::queue<int> movedQueue = std::move(temp);

  EXPECT_EQ(movedQueue.size(), 5);

  int expectedValue = 1;
  for (size_t index = 0; index < movedQueue.size(); ++index) {
    EXPECT_EQ(movedQueue[index], expectedValue++);
  }

  EXPECT_TRUE(temp.empty());
}

TEST(Queue, CanMoveAssignQueue) {
  s21::queue<int> temp = {1, 2, 3, 4, 5};

  s21::queue<int> targetQueue;
  targetQueue = std::move(temp);

  EXPECT_EQ(targetQueue.size(), 5);

  int expectedValue = 1;
  for (size_t index = 0; index < targetQueue.size(); ++index) {
    EXPECT_EQ(targetQueue[index], expectedValue++);
  }

  EXPECT_TRUE(temp.empty());
}

TEST(Queue, CorrectlySwapsContents) {
  s21::queue<int> Queue1 = {1, 2, 3};
  s21::queue<int> Queue2 = {4, 5, 6, 7};

  auto size1_before = Queue1.size();
  auto size2_before = Queue2.size();

  Queue1.swap(Queue2);

  EXPECT_EQ(Queue1.size(), size2_before);
  EXPECT_EQ(Queue2.size(), size1_before);

  EXPECT_EQ(Queue1[0], 4);
  EXPECT_EQ(Queue1[3], 7);
  EXPECT_EQ(Queue2[0], 1);
  EXPECT_EQ(Queue2[2], 3);
}

TEST(Queue, Front) {
  s21::queue<int> s21_queue = {1, 2, 3, 4, 5};

  EXPECT_EQ(s21_queue.front(), 1);
}

TEST(Queue, Push) {
  s21::queue<int> s21_queue = {1, 2, 3, 4, 5};

  s21_queue.push(0);
  s21_queue.push(10);
  EXPECT_EQ(s21_queue.size(), 7);
  EXPECT_EQ(s21_queue[5], 0);
  EXPECT_EQ(s21_queue[6], 10);
}

TEST(Queue, PushInEmpty) {
  s21::queue<int> s21_queue;

  s21_queue.push(0);
  EXPECT_EQ(s21_queue.size(), 1);
  EXPECT_EQ(s21_queue.front(), 0);
}

TEST(Queue, Pop) {
  s21::queue<int> s21_queue = {1, 2, 3, 4, 5};

  s21_queue.pop();
  EXPECT_EQ(s21_queue.size(), 4);
  EXPECT_EQ(s21_queue.front(), 2);
  EXPECT_EQ(s21_queue.back(), 5);
}

TEST(Queue, PopEmpty) {
  s21::queue<int> s21_queue;

  s21_queue.pop();
  EXPECT_EQ(s21_queue.size(), 0);
  EXPECT_EQ(s21_queue.empty(), true);
}

TEST(Queue, Insert_Many_front) {
  s21::queue<int> s21_queue = {1, 2, 3};
  s21_queue.insert_many_back(7, 8, 11);
  EXPECT_EQ(s21_queue.size(), 6);
  EXPECT_EQ(s21_queue[3], 7);
  EXPECT_EQ(s21_queue[4], 8);
  EXPECT_EQ(s21_queue.back(), 11);
}

TEST(Queue, Insert_Empty_front) {
  s21::queue<int> s21_queue;
  s21_queue.insert_many_back(0, 1);
  EXPECT_EQ(s21_queue.size(), 2);
  EXPECT_EQ(s21_queue.back(), 1);
}