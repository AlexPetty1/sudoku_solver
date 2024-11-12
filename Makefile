CC=gcc -g

sudoku_solver: sudoku_solver.o
	$(CC) sudoku_solver.o -o sudoku_solver

sudoku_solver.o: sudoku_solver.c sudoku_solver.h
	$(CC) -c sudoku_solver.c