.PHONY: all clean check tests gcov_report dvi install uninstall dist
XX=g++
APP=$(BD)3D_Viewer2_1.app

FILES= Model/s21_matrix_oop.cpp \
	   Model/affineTransformations.cpp \

DIRS= View \
	  Model \
	  Controller \
	  Icon \
	  giflib \
	  doc \

LDFLAGS= -Wall -Wextra -Werror -std=c++17 -g
OS := $(shell uname -s)
TEST_FLAGS= -lgtest -lgtest_main -lpthread
CFLAGS= -fprofile-arcs -ftest-coverage
BD= build/

ifeq ($(OS), Darwin)
	CC += -D OS_MAC
else
	CC += -D OS_LINUX
endif

all: build install tests

build: clean copy
	cd $(BD); qmake 3D_Viewer2_1.pro
	make -C $(BD)

copy:
	@mkdir build
	cp *.pro *.cpp *.qrc $(BD)
	rsync -rupE $(DIRS) $(BD)

install: uninstall build
	@mkdir $(HOME)/Applications/3D_Viewer2_1/ 2>/dev/null
	cp -rf $(APP) $(HOME)/Applications/3D_Viewer2_1/
	make clean

uninstall: 
	rm -rf $(HOME)/Applications/3D_Viewer2_1

dist:
	rm -rf 3D_Viewer2_1
	mkdir 3D_Viewer2_1
	cp Makefile *.pro *.cpp 3D_Viewer2_1/
	rsync -rupE $(DIRS)  3D_Viewer2_1/
	tar -cvzf $(HOME)/Desktop/3D_Viewer2_1.tgz 3D_Viewer2_1/
	rm -rf 3D_Viewer2_1/

dvi:
	open doc/index.html

tests: clean
	$(XX) $(LDFLAGS) $(FLAGS) Tests/3DViwerTests.cpp $(FILES) -lstdc++ $(TEST_FLAGS) -o test
	./test

clean:
	rm -rf build test *.a *.o *.cfg *.gcda *.gcno *.html *.info *.dSYM report

check: 
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	cp ../materials/linters/suppressions.cfg suppressions.cfg
	python3 ../materials/linters/cpplint.py --extension=cpp Model/*.cpp Controller/*.cpp View/*.cpp Model/*.h Controller/*.h View/*.h
	cppcheck -q --std=c++17 --enable=all --language=c++ --suppressions-list=suppressions.cfg Model/*.cpp Controller/*.cpp View/*.cpp Model/*.h Controller/*.h View/*.h

gcov_report: clean
	$(XX) $(FLAGS) $(CFLAGS) $(FILES) Tests/3DViwerTests.cpp -lstdc++ $(TEST_FLAGS) -o test
	./test
	lcov --no-external -c -d . -o tests.info
	genhtml tests.info -o report
ifeq ($(OS), Darwin)
	open -a "Google Chrome" report/index.html
else
	xdg-open report/index.html
endif
