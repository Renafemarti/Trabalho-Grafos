CC     = gcc
CFLAGS = -Wall -O2
TARGET = grafo

$(TARGET): grafo.c main.c grafo.h
	$(CC) $(CFLAGS) -o $(TARGET) grafo.c main.c

clean:
	rm -f $(TARGET)
