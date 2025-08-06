CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
TARGET=badapple
SOURCE=src/badapple.c

INSTALL_DIR=/usr/local/bin
DATA_DIR=/usr/local/share/badapple

.PHONY: all clean data

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

data: assets/badapple.dat

assets/badapple.dat: src/process_frames.py
	python3 src/process_frames.py

clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo mkdir -p $(INSTALL_DIR)
	sudo mkdir -p $(DATA_DIR)
	sudo cp $(TARGET) $(INSTALL_DIR)/
	sudo cp assets/badapple.dat $(DATA_DIR)/
	sudo cp assets/badapple.mp3 $(DATA_DIR)/

uninstall:
	sudo rm -f $(INSTALL_DIR)/badapple
	sudo rm -rf $(DATA_DIR)

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all     - Build the main program"
	@echo "  data    - Generate badapple.dat from frames"
	@echo "  clean   - Remove built files"
	@echo "  install - Install to /usr/local/bin"