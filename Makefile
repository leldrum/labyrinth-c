CC = gcc
CPPFLAGS = -Iinclude -DMAZE_DIR=\"$(FILE_DIR)\"
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g
TARGET = Maze
BUILD_DIR = build
FILE_DIR = file

SRCS = src/main.c \
	src/creation.c \
	src/display.c \
	src/launcher.c \
	src/savelaby.c \
	src/game.c

OBJS = $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all run test-creation clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	mkdir -p $(FILE_DIR)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)