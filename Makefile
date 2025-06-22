CC = gcc
SRC = src/main.c src/backup_switch.c src/create.c src/operation.c src/reports.c
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CC) -Iinclude $(SRC) -o $(OUT)

clean:
	del $(OUT).exe