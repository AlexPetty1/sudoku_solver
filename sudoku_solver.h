#ifndef _SUDOKU_SOLVER_H
#define _SUDOKU_SOLVER_H



//Constants
const int SIZE_OF_BOXES = 3;
const int NUMBER_OF_BOXES = SIZE_OF_BOXES * SIZE_OF_BOXES;
const int ROW_LENGTH = SIZE_OF_BOXES * SIZE_OF_BOXES;
const int NUMBER_OF_TILES = ROW_LENGTH * ROW_LENGTH;


struct selector{
    int x;
    int y;
};

struct tile{
    int potentialNums[10];       // acts as a map
                                 // 0 can not be a potential tile
                                 // 1 is valid potential tile
    int numberOfPotentialNums;
    int num;                    // number in box
                                // -1 if no number decided yet
    int xCor;
    int yCor;
    int testedForPair;

    int setByNotBruteForce;     // used so brute force will skip over tiles, 
                                // set by the normal solving algoritm

};

struct row{
    int potentialNumsInRow[10];       // acts as a map
                                    // 0 number is in row
                                    // 1 - 9 number of tiles that can have that number
    int rowNum;                 
    int isColumn;
    int hiddenPairChecked[10]; 
};

struct box{
    int xCor;
    int yCor;
    int potentialNumsInBox[10];           // acts as a map
                                // 0 number is in box
                                // 1 - 9 number of tiles that can have that number
    int doublePointerChecked[10]; 
    int triplePointerChecked[10]; 
    int hiddenPairChecked[10]; 
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
struct board* makeBoard();

int setTile(int x, int y, int number, struct board* board);

struct box* getBoxFromCords(struct box*** boxArray, int x, int y);

void freeTileArray(struct tile*** tileArray, int rowLength);
void freeBoxArray(struct box*** boxArray, int boxLength);
void freeRowArray(struct row** rowArray, int rowLength);
void freeBoard(struct board* board);

void updateRow(struct board* board, struct row* rowArray, int numberAddedToRow);
void updateBox(struct board* board, struct box* boxArray, int numberAddedToBox);
void updateBoxIgnoretiles(struct board* board, struct box* box, int numberAddedToBox, int tilesIgnored[10]);
void updateRowIgnoreTiles(struct board* board, struct row* row, int numberAddedToRow, int tilesIgnored[9]);

int checkRow(struct board* board, struct row* row);
int checkBox(struct board* board, struct box* box);

int checkRowsForHiddenSingles(struct board* board);
int checkRowForHiddenSingle(struct board* board, struct row* row);
int checkBoxesForHiddenSingles(struct board* board);
int checkBoxForHiddenSingle(struct board* board, struct box* box);

int checkTilesForPairs(struct board* board);
int checkBoxForPairs(struct board* board, struct box* box, struct tile* knownDouble);
int checkBoxesForPointers(struct board* board);
int checkBoxForDoublePointers(struct board* board, struct box* box, int numberDecreased);
int checkBoxForTriplePointers(struct board* board, struct box* box, int numberDecreased);


int checkRowForHiddenPair(struct board* board, struct row* row);
int checkRowsForHiddenPairs(struct board* board);
int checkBoxesForHiddenPairs(struct board* board);
int checkBoxForHiddenPair(struct board* board, struct box* box);
int updateTileHiddenPair(struct board* board, struct tile* tile, int num1, int num2);


void interactiveMode(struct board* board);


void printBoard(struct board* board);

#endif