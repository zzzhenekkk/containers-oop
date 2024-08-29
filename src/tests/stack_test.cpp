#include <gtest/gtest.h>

#include <iostream>

#include "../lib/s21_stack.h"

TEST(Stack, Constructor_Default) {
  s21::stack<int> s21_stack;

  EXPECT_EQ(s21_stack.size(), 0);
  EXPECT_EQ(s21_stack.empty(), true);
  EXPECT_THROW(s21_stack.top(), std::range_error);
}

TEST(Stack, InitializerStackConstructorBaseTest) {
  s21::stack<int> s21_stack = {1, 2, 3, 4, 5};

  EXPECT_EQ(s21_stack.size(), 5);

  size_t index = 0;
  for (int value : {1, 2, 3, 4, 5}) {
    EXPECT_EQ(s21_stack[index], value);
    ++index;
  }
}

TEST(Stack, InitializerStackConstructorEmpty) {
  s21::stack<int> s21_stack = {};

  EXPECT_EQ(s21_stack.empty(), true);
  EXPECT_EQ(s21_stack.size(), 0);
}

TEST(Stack, CopyConstructorCreatesExactCopy) {
  s21::stack<int> originalStack = {1, 2, 3, 4, 5};

  s21::stack<int> copiedStack(originalStack);

  EXPECT_EQ(originalStack.size(), copiedStack.size());
  for (size_t index = 0; index < copiedStack.size(); ++index) {
    EXPECT_EQ(originalStack[index], copiedStack[index]);
  }
}

TEST(Stack, ModificationsToCopyDoNotAffectOriginal) {
  s21::stack<int> originalStack = {1, 2, 3};
  s21::stack<int> copiedStack(originalStack);

  copiedStack[0] = 0;
  EXPECT_EQ(originalStack[0], 1);
  EXPECT_EQ(copiedStack[0], 0);
}

TEST(Stack, MoveConstructorCanMoveStack) {
  s21::stack<int> temp = {1, 2, 3, 4, 5};

  s21::stack<int> movedStack = std::move(temp);

  EXPECT_EQ(movedStack.size(), 5);

  int expectedValue = 1;
  for (size_t index = 0; index < movedStack.size(); ++index) {
    EXPECT_EQ(movedStack[index], expectedValue++);
  }

  EXPECT_TRUE(temp.empty());
}

TEST(Stack, CanMoveAssignStack) {
  s21::stack<int> temp = {1, 2, 3, 4, 5};

  s21::stack<int> targetStack;
  targetStack = std::move(temp);

  EXPECT_EQ(targetStack.size(), 5);

  int expectedValue = 1;
  for (size_t index = 0; index < targetStack.size(); ++index) {
    EXPECT_EQ(targetStack[index], expectedValue++);
  }

  EXPECT_TRUE(temp.empty());
}

TEST(Stack, CorrectlySwapsContents) {
  s21::stack<int> Stack1 = {1, 2, 3};
  s21::stack<int> Stack2 = {4, 5, 6, 7};

  auto size1_before = Stack1.size();
  auto size2_before = Stack2.size();

  Stack1.swap(Stack2);

  EXPECT_EQ(Stack1.size(), size2_before);
  EXPECT_EQ(Stack2.size(), size1_before);

  EXPECT_EQ(Stack1[0], 4);
  EXPECT_EQ(Stack1[3], 7);
  EXPECT_EQ(Stack2[0], 1);
  EXPECT_EQ(Stack2[2], 3);
}

TEST(Stack, Top) {
  s21::stack<int> s21_stack = {1, 2, 3, 4, 5};

  EXPECT_EQ(s21_stack.top(), 5);
}

TEST(Stack, Push) {
  s21::stack<int> s21_stack = {1, 2, 3, 4, 5};

  s21_stack.push(0);
  s21_stack.push(10);
  EXPECT_EQ(s21_stack.size(), 7);
  EXPECT_EQ(s21_stack[5], 0);
  EXPECT_EQ(s21_stack[6], 10);
}

TEST(Stack, PushInEmpty) {
  s21::stack<int> s21_stack;

  s21_stack.push(0);
  EXPECT_EQ(s21_stack.size(), 1);
  EXPECT_EQ(s21_stack.top(), 0);
}

TEST(Stack, Pop) {
  s21::stack<int> s21_stack = {1, 2, 3, 4, 5};

  s21_stack.pop();
  EXPECT_EQ(s21_stack.size(), 4);
  EXPECT_EQ(s21_stack.top(), 4);
}

TEST(Stack, PopEmpty) {
  s21::stack<int> s21_stack;

  s21_stack.pop();
  EXPECT_EQ(s21_stack.size(), 0);
  EXPECT_EQ(s21_stack.empty(), true);
}

TEST(Stack, Insert_Many_front) {
  s21::stack<int> s21_stack = {1, 2, 3};
  s21_stack.insert_many_front(7, 8, 9);
  EXPECT_EQ(s21_stack.size(), 6);
  EXPECT_EQ(s21_stack[3], 7);
  EXPECT_EQ(s21_stack[4], 8);
  EXPECT_EQ(s21_stack.top(), 9);
}

TEST(Stack, Insert_Empty_front) {
  s21::stack<int> s21_stack;
  s21_stack.insert_many_front(0, 10);
  EXPECT_EQ(s21_stack.size(), 2);
  EXPECT_EQ(s21_stack.top(), 10);
}