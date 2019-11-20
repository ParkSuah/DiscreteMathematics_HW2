CC = gcc
CFLAGS =
TARGET1 = DNF
OBJ1 = ./DNF.c

all : $(TARGET1)

$(TARGET1) : $(OBJ1)
	$(CC) $(CFLAGS) $(OPTION) -o $@ $^
