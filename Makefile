CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g
TARGET = Maze
BUILD_DIR = build

SRCS = src/main.c \
	src/creation.c \
	src/display.c \
	src/launcher.c \
	src/managemaze.c \
	src/game.c \
	src/monster.c

OBJS = $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

TEST_SRCS = tests/test_main.c

TEST_OBJS = $(TEST_SRCS:tests/%.c=$(BUILD_DIR)/test_%.o)

$(BUILD_DIR)/test_%.o: tests/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -Itests -c $< -o $@

$(BUILD_DIR)/test_test_main.o: tests/test_creation.c tests/test_display.c \
	tests/test_game.c tests/test_monster.c tests/test_managemaze.c \
	tests/test_launcher.c

$(BUILD_DIR)/unit_tests: $(TEST_OBJS) \
	$(BUILD_DIR)/creation.o \
	$(BUILD_DIR)/display.o \
	$(BUILD_DIR)/managemaze.o \
	$(BUILD_DIR)/monster.o \
	$(BUILD_DIR)/game.o \
	$(BUILD_DIR)/launcher.o
	$(CC) $(CFLAGS) $^ -o $@

test: $(BUILD_DIR)/unit_tests
	./$(BUILD_DIR)/unit_tests

clean:
	rm -rf $(BUILD_DIR) $(TARGET)