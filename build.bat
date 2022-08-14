@echo off

gcc Sudoku_Platform.c -lgdi32 -luser32 -o sudoku_solver
"sudoku_solver.exe"