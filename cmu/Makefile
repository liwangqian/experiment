CC = gcc

INC = -I.
INC += -I./priv

SRC = $(shell find ./ -name "*.c")
CFLAGS += $(INC)

OBJ = $(SRC:%.c=%.o)

TARGET = main.exe
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)
