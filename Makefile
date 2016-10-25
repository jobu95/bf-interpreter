CC=g++
TARGET=bf

.PHONY: compile
compile: bf.cpp
	$(CC) $(LFLAGS) $^ -o $(TARGET)

clean:
	rm -f $(TARGET)
