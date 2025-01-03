Sudoku Solver Project
Is able to solve puzzles from the game Sudoku.

Uses a combination of a normal sudoku solver that works by elimination potential options and a bruteforce 
backtracing algorithm that uses the partially solved sudoku as input if the normal did not solve it.

Normal Sudoku Solver:
Can ushally solve medium nyt sudokus and sometimes hard.
Works by eliminating potential options, implements the tests:
  1. Obvious single - checks all rows and boxes for tiles with only 1 potential option
  2. Hidden single - row/box only has one place a number can be
  3. Obvious pairs - if a box/row has 2 tiles with the only 2 potential tiles and both have same potential tiles. 
           any other tiles in box/col/row can not have those.
  4. Hidden pairs - if a pair of tiles in a box/row has 2 of the same potential tiles and those are not found anywhere else in the same box/row. 
            Eliminate all other potential tiles from the 2 tiles.
  5. Pointers - if a box has 2 or 3 potential tiles a number can be, and those tiles are all in the same row or column. 
            No other tiles in that row or column can have those potential tiles. 


Brute Force:
Uses the normal solving algorithm's result to skip over tiles and numbers on a tile.
Works by:
  1. Get to a tile.
  2. If tile is valid move to next tile
  3. If tile is invalid, increase by 1 and repeat until valid.
  4. If loops over from increase: set to empty,  move to previous tile and increase it by 1. 
