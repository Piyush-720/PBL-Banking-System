CC = gcc
SRC = src/main.c src/piyush.c src/deepanshu.c src/gayatri.c src/aakriti.c
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CC) -Iinclude $(SRC) -o $(OUT)

clean:
	del $(OUT).exe