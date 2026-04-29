CC := gcc
CFLAGS := -Wall -Wextra -pedantic -g #-Werror
LINKER := gcc

TARGET := cash

HEADER_DIR := headers
SOURCE_DIR := src
OBJECT_DIR := obj

SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

$(TARGET): $(OBJECTS)
	$(LINKER) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(OBJECTS)
	rm $(TARGET)