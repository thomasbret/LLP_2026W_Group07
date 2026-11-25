CC     = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -fno-strict-aliasing -Iinclude -Isrc
 
SRC = src/main.c src/heap.c
OUT = build/demo
 
all: $(OUT)
 
$(OUT): $(SRC) include/my_alloc.h src/internal.h
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
 
run: $(OUT)
	./$(OUT)
 
clean:
	rm -rf build
 
.PHONY: all run clean
 