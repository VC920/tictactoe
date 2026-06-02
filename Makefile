all:
	gcc -Wall -Wextra -o tictactoe tictactoe.c -l raylib

run:
	./tictactoe
