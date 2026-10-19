CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g
TARGET = labyrinth
BUILD_DIR = build

SRCS = src/main.c \
	src/creation.c \
	src/display.c \
	src/launcher.c \
	src/savelaby.c

OBJS = $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)