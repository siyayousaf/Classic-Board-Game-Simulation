CC = gcc
CFLAGS = -Wall -pedantic-errors -std=c99
TARGET = p2_qyousaf_206

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET)

