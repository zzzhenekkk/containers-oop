.PHONY: all clean
FLAGS :=-std=c++17 -Wextra -Wall -Werror
SOURCES_TEST := $(wildcard tests/*.cpp) main.cpp
LGFLAGS := -lgtest -lgtest_main
# COVFLAGS = -fprofile-arcs  -lcheck -ftest-coverage

all: gcov_report

gcov_report: test
	gcov -f *.gcda
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info 
	open report/index.html

test: clean
	g++  $(SOURCES_TEST) --coverage $(FLAGS) $(LGFLAGS) -o test -L.
	./test

clean:
	rm -rf *.a lib/*.o  main test *.gcda *.gcno *.gcov *.info *.html report

valgrind: test
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test

style:
	clang-format -style=google -i tests/*.cpp lib/*.h