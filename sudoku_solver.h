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
    int testedForPair;

};

struct row{
    int potentialNumsInRow[10];       // acts as a map
                                    // 0 number is in row
                                    // 1 - 9 number of tiles that can have that number
    int rowNum;                 
    int isColumn;
};

struct box{
    int xCor;
    int yCor;
    int potentialNumsInBox[10];           // acts as a map
                                // 0 number is in box
                                // 1 - 9 number of tiles that can have that number
    int pointerChecked[10]; 
};


struct board{
    struct tile*** tileArray;
    struct box*** boxArray;
    struct row** rowArray;
    struct row** colArray;
    struct selector selector;
};

struct tile* makeTile(int x, int y);
struct row* makeRow(int rowNum, int isColumn);
struct box* makeBox(int xCor, int yCor);

int setTile(int x, int y, int number, struct board* board);

struct box* getBoxFromCords(struct box*** boxArray, int x, int y);

void freeTileArray(struct tile*** tileArray, int rowLength);
void freeBoxArray(struct box*** boxArray, int boxLength);
void freeRowArray(struct row** rowArray, int rowLength);

void updateRow(struct board* board, struct row* rowArray, int numberAddedToRow);
void updateBox(struct board* board, struct box* boxArray, int numberAddedToBox);
void updateBoxIgnoretiles(struct board* board, struct box* box, int numberAddedToBox, int tilesIgnored[10]);
void updateRowIgnoreTiles(struct board* board, struct row* row, int numberAddedToRow, int tilesIgnored[9]);

int checkRow(struct board* board, struct row* row);
int checkBox(struct board* board, struct box* box);

int checkRowsForOnePotentialNumber(struct board* board, int number);
int checkBoxesForOnePotentialNumber(struct board* board, int number);
int checkAllPotentialNumberInBox(struct board* board, struct box* box);
int checkAllPotentialNumbersInRow(struct board* board, struct row* row);

int checkBoxForPairs(struct board* board, struct box* box, struct tile* knownDouble);
int checkBoxForPointers(struct board* board, struct box* box, int numberDecreased);

void printBoard(struct tile*** tileArray, struct selector* boardSelector);

#endif