#ifndef _SUDOKU_SOLVER_H
#define _SUDOKU_SOLVER_H


struct selector{
    int x;
    int y;
};

struct tile{
    int potentialNums[10];       // acts as a map
                                 // 0 can not be a potentia tile
                                 // 1 is valid potential tile
    int numberOfPotentialNums;
    int num;                    // number in box
                                // -1 if no number decided yet
    int xCor;
    int yCor;
};

struct row{
    int numsInRow[10];       // acts as a map
                            // 0 number is not in box
                            // 1 number is in box
    int rowNum;                 
    int isColumn;
};

struct box{
    int xCor;
    int yCor;
    int numsInBox[10];           // acts as a map, 0 number is not in box, 1 number is in box
};


struct board{
    struct tile*** tileArray;
    struct box*** boxArray;
    struct row** rowArray;
    struct row** colArray;

};

struct tile* makeTile(int x, int y);
struct row* makeRow(int rowNum, int isColumn);
struct box* makeBox(int xCor, int yCor);

int setTile(int x, int y, int number, struct board* board);

struct box* getBoxFromCords(struct box*** boxArray, int x, int y);

void freeTileArray(struct tile*** tileArray, int rowLength);
void freeBoxArray(struct box*** boxArray, int boxLength);
void freeRowArray(struct row** rowArray, int rowLength);

void updateRow(struct tile*** tileArray, struct row* rowArray, int numberAddedToRow);
void updateBox(struct tile*** tileArray, struct box* boxArray, int numberAddedToBox);

void checkRow(struct board* board, struct row* row);
void checkBox(struct board* board, struct box* box);

void printBoard(struct tile*** tileArray, struct selector* boardSelector);

#endif