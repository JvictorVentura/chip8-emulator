CFLAGS=-lSDL2 -Wall -Werror -Wextra
EXE_NAME=chip8

make:
		gcc *.c $(CFLAGS) -o $(EXE_NAME)

run:
		gcc *.c $(CFLAGS) -o  $(EXE_NAME) &	./$(EXE_NAME) $(rom)
