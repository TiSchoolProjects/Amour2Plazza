TARGET = plazza

CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20

INC = -I include

SRC = $(wildcard src/*.cpp) \
			$(wildcard src/Mutex/*.cpp)

OBJ = $(SRC:.cpp=.o)

TEST_TARGET = unit_tests
SRC_TESTS = $(wildcard tests/*.cpp)
SRC_NO_MAIN = $(filter-out main.cpp, $(SRC))
OBJ_TESTS = $(SRC_NO_MAIN:.cpp=.o) $(SRC_TESTS:.cpp=.o)
LIBS_TESTS = -lcriterion

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

tests: $(OBJ_NO_MAIN) $(OBJ_TESTS)
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ_NO_MAIN) $(OBJ_TESTS) -o $(TEST_TARGET) -lcriterion
	@ $(LOG_TIME) "$(C_GREEN) Tests Builed: $(C_PURPLE)$(TEST_TARGET)$(C_RESET)"

tests_run: tests
	./$(TEST_TARGET)

format:
	@ find src/ -name "*.cpp" -type f -exec clang-format -i {} ";"
	@ find include/ -name "*.hpp" -type f -exec clang-format -i {} ";"

clean:
	@ $(RM) $(OBJ)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(OBJ) $(C_RESET)"

fclean: clean
	@ $(RM) $(TARGET)
	@ $(RM) $(TEST_TARGET)
	@ $(LOG_TIME) "$(C_YELLOW) RM $(C_PURPLE) $(TARGET) & $(TEST_TARGET) $(C_RESET)"
	rm -f *.gcno *.gcda
	rm -f tests/*.o
	rm -f unit_tests


.NOTPARALLEL: re
re:	fclean all

.PHONY: all clean fclean re



PREFIX ?=
BINDIR ?= $(PREFIX)/bin


V ?= 0
ifneq ($(V),0)
  Q :=
else
  Q := @
endif

ifneq ($(shell command -v tput),)
  ifneq ($(shell tput colors),0)

mk-color = \e[$(strip $1)m

C_BEGIN := \033[A
C_RESET := $(call mk-color, 00)

C_RED := $(call mk-color, 31)
C_GREEN := $(call mk-color, 32)
C_YELLOW := $(call mk-color, 33)
C_BLUE := $(call mk-color, 34)
C_PURPLE := $(call mk-color, 35)
C_CYAN := $(call mk-color, 36)

  endif
endif

NOW = $(shell date +%s%3N)

ifndef STIME
STIME := $(call NOW)
endif

TIME_NS = $(shell expr $(call NOW) - $(STIME))
TIME_MS = $(shell expr $(call TIME_NS))

BOXIFY = "[$(C_BLUE)$(1)$(C_RESET)] $(2)"

ifneq ($(shell command -v printf),)
  LOG_TIME = printf $(call BOXIFY, %6s ,$(strip %b\n)) "$(call TIME_MS)"
else
  LOG_TIME = echo -e $(call BOXIFY, $(call TIME_MS) ,)
endif
