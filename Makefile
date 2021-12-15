CC = gcc

C_SRC = \
main.c\
basic_core.c\
instruction_set.c

default:
	$(CC) $(C_SRC) -o basiccore 

debug:
	$(CC) -DDEBUG $(C_SRC) -o basiccore 

clean:
	rm -rf build/*
	rm -rf bc_build/*

