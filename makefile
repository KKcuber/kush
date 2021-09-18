CC = gcc

TARGET = kush

$(TARGET):
	$(CC) -o $(TARGET) *.c

kush:
	$(CC) -o $(TARGET) *.c

clean:
	rm $(TARGET)