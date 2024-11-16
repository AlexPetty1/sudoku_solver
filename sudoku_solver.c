#include <stdio.h>
#include <stdlib.h>

#include "sudoku_solver.h"


// Function: makeRow
// Description: mallocs a tile struct and returns it's pointer
// Parameters: x - x cordinate of the tile
//             y - y cordinate of the tile
struct tile* makeTile(int x, int y){
    struct tile* newTile = malloc(sizeof(struct tile));
    newTile->xCor = x;
    newTile->yCor = y;
    newTile->num = -1;
    newTile->numberOfPotentialNums = 9;
    newTile->testedForPair = 0;

    for(int i = 0; i < 10; i++){
        newTile->potentialNums[i] = 1;
    }

    return newTile;
}


// Function: makeRow
// Description: mallocs a row struct and returns it's pointer
// Parameters: rowNum - the number the row is
//             isColumn - 1 if row is a column, 0 if not
struct row* makeRow(int rowNum, int isColumn){
    struct row* newRow = malloc(sizeof(struct row));

    newRow->rowNum = rowNum;
    newRow->isColumn = isColumn;
    for(int i = 0; i < 10; i++){
        newRow->potentialNumsInRow[i] = 9;
    }

    return newRow;
}


// Function: makeBox
// Description: mallocs a box struct and returns it's pointer
// Parameters: x - x cordinate of the box
//             y - y cordinate of the box
struct box* makeBox(int x, int y){
    struct box* newBox = malloc(sizeof(struct box));

    newBox->xCor = x;
    newBox->yCor = y;
    for(int i = 0; i < 10; i++){
        newBox->potentialNumsInBox[i] = 9;
        newBox->pointerChecked[i] = 0;
    }
}

// Function: freeTileArray
// Description: frees a every tile in 2d array and set it to NULL
// Parameters: tileArray - array of tiles being freed
//              rowLength - length of the row
void freeTileArray(struct tile*** tileArray, int rowLength){

    for(int y = 0; y < rowLength; y++){
        for(int x  = 0; x < rowLength; x++){
            free(tileArray[y][x]);
            tileArray[y][x] = NULL;
        }

        free(tileArray[y]);
        tileArray[y] = NULL;
    }

    free(tileArray);
    tileArray = NULL;
}

// Function: freeRowArray
// Description: frees a array of boxs and sets each to NULL
// Parameters: boxArray - array of boxs being freed
//              boxLength - length of the box
void freeBoxArray(struct box*** boxArray, int boxLength){
    for(int y = 0; y < boxLength; y++){
        for(int x = 0; x < boxLength; x++){
            free(boxArray[y][x]);
            boxArray[y][x] = NULL;
        }

        free(boxArray[y]);
        boxArray[y] = NULL;
    }

    free(boxArray);
    boxArray = NULL;
}


// Function: freeRowArray
// Description: frees a array of rows and sets each to NULL
// Parameters: rowArray - array of rows being freed
//              rowLength - length of the row
void freeRowArray(struct row** rowArray, int rowLength){

    for(int i = 0; i < rowLength; i++){
        if(rowArray[i] != NULL){
            free(rowArray[i]);
            rowArray[i] = NULL;
        }
    }

    free(rowArray);
    rowArray = NULL;
}


// Function: updateBox
// Description: updates the box to have the numberAddedToBox, 
//              updates each tile in box to not have numberAddedToBox among potential numbers
// Parameters: tileArray - the array of all tiles
//              boxArray - pointer to the box being added to
//              numberAddedToBox - the number is added to the box
void updateRow(struct board* board, struct row* row, int numberAddedToRow){

    struct tile*** tileArray = board->tileArray;
    row->potentialNumsInRow[numberAddedToRow] = 0;

    for(int i = 0; i < 9; i++){
        struct tile* tileOn = NULL;
        struct row* otherRow = NULL;
        if(row->isColumn == 1){
            tileOn = tileArray[i][row->rowNum];
            otherRow = board->rowArray[tileOn->yCor];   //is a row
        } else {
            tileOn = tileArray[row->rowNum][i];
            otherRow = board->colArray[tileOn->xCor];   //is a col
        }
            
        if(tileOn->potentialNums[numberAddedToRow] == 1){
            tileOn->potentialNums[numberAddedToRow] = 0;
            tileOn->numberOfPotentialNums = tileOn->numberOfPotentialNums - 1;
            // if(tileOn->numberOfPotentialNums == 2){
            //     struct box* boxOn = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            //     checkBoxForPairs(board, boxOn, tileOn);
            // }

            //updates box and other row of tile
            struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            if(otherRow->potentialNumsInRow[numberAddedToRow] != 0){
                otherRow->potentialNumsInRow[numberAddedToRow]--;
            }

            if(box->potentialNumsInBox[numberAddedToRow] != 0){
                box->potentialNumsInBox[numberAddedToRow]--;
            }

            // struct box* boxTileIn = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor); 
            // // checks for pointers
            // if(box->potentialNumsInBox[numberAddedToRow] == 2){
            //     checkBoxForPointers(board, boxTileIn, numberAddedToRow);
            // }

        }
    }

}



void updateRowIgnoreTiles(struct board* board, struct row* row, int numberAddedToRow, int tilesIgnored[9]){
    struct tile*** tileArray = board->tileArray;

    for(int i = 0; i < 9; i++){
        //skips iteration
        if(tilesIgnored[i] == 1){
            continue;
        }

        struct tile* tileOn = NULL;
        struct row* otherRow = NULL;
        if(row->isColumn == 1){
            tileOn = tileArray[i][row->rowNum];
            otherRow = board->rowArray[tileOn->yCor];   //is a row
        } else {
            tileOn = tileArray[row->rowNum][i];
            otherRow = board->colArray[tileOn->xCor];   //is a col
        }
            
        if(tileOn->potentialNums[numberAddedToRow] == 1){
            tileOn->potentialNums[numberAddedToRow] = 0;
            tileOn->numberOfPotentialNums = tileOn->numberOfPotentialNums - 1;
            row->potentialNumsInRow[numberAddedToRow] = row->potentialNumsInRow[numberAddedToRow] - 1;

            // if(tileOn->numberOfPotentialNums == 2){
            //     struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            //     checkBoxForPairs(board, box, tileOn);
            // }

            //updates box and other row of tile
            struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            if(otherRow->potentialNumsInRow[numberAddedToRow] != 0){
                otherRow->potentialNumsInRow[numberAddedToRow]--;
            }

            if(box->potentialNumsInBox[numberAddedToRow] != 0){
                box->potentialNumsInBox[numberAddedToRow]--;
            }

            // checks for pointers
            // struct box* boxTileIn = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            // if(box->potentialNumsInBox[numberAddedToRow] == 2){
            //     checkBoxForPointers(board, boxTileIn, numberAddedToRow);
            // }

        }
    }

}


// Function: updateBox
// Description: updates the box to have the numberAddedToBox, 
//              updates each tile in box to not have numberAddedToBox among potential numbers
// Parameters: tileArray - the array of all tiles
//              boxArray - pointer to the box being added to
//              numberAddedToBox - the number is added to the box
void updateBox(struct board* board, struct box* box, int numberAddedToBox){
    struct tile*** tileArray = board->tileArray;
    box->potentialNumsInBox[numberAddedToBox] = 0;
    
    // checks every tile in box
    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            struct tile* tileOn = tileArray[y][x];

            //removes number from being a potential option in tile
            if(tileOn->potentialNums[numberAddedToBox] == 1){
                struct row* tileRow = board->rowArray[y];
                struct row* tileCol = board->colArray[x];

                tileOn->potentialNums[numberAddedToBox] = 0;
                tileOn->numberOfPotentialNums = tileOn->numberOfPotentialNums - 1;

                if(tileRow->potentialNumsInRow[numberAddedToBox] != 0){
                    tileRow->potentialNumsInRow[numberAddedToBox]--;
                }

                if(tileCol->potentialNumsInRow[numberAddedToBox] != 0){
                    tileCol->potentialNumsInRow[numberAddedToBox]--;
                }
            }
        }
    }
}



// Function: updateBoxIgnoreTiles
// Description: updates the box to have the numberAddedToBox, 
//              updates each tile in box to not have numberAddedToBox among potential numbers
// Parameters: tileArray - the array of all tiles
//              boxArray - pointer to the box being added to
//              numberAddedToBox - the number is added to the box
void updateBoxIgnoretiles(struct board* board, struct box* box, int numberAddedToBox, int tilesIgnored[10]){
    struct tile*** tileArray = board->tileArray;
    
    // checks every tile in box
    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    int index = -1;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            index = index + 1;

            if(tilesIgnored[index] == 1){
                continue;
            }

            struct tile* tileOn = tileArray[y][x];
            //removes number from being a potential option in tile
            if(tileOn->potentialNums[numberAddedToBox] == 1){
                struct row* tileRow = board->rowArray[y];
                struct row* tileCol = board->colArray[x];

                tileOn->potentialNums[numberAddedToBox] = 0;
                tileOn->numberOfPotentialNums = tileOn->numberOfPotentialNums - 1;

                box->potentialNumsInBox[numberAddedToBox] = box->potentialNumsInBox[numberAddedToBox] - 1;

                // if(tileOn->numberOfPotentialNums == 2){
                //     struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
                //     checkBoxForPairs(board, box, tileOn);
                // }

                if(tileRow->potentialNumsInRow[numberAddedToBox] != 0){
                    tileRow->potentialNumsInRow[numberAddedToBox]--;
                }

                if(tileCol->potentialNumsInRow[numberAddedToBox] != 0){
                    tileCol->potentialNumsInRow[numberAddedToBox]--;
                }
            }
        }
    }
}


// Function: chcekBoxForPairs
// Description: tests if a box has 2 tiles with 2 of the same potential numbers and only 2 potential numbers
//                  if true removes those 2 potential numbers as potential from all tiles in box
//                  run when eliminating a option from a tile
// Parameters: 
int checkBoxForPairs(struct board* board, struct box* box, struct tile* knownDouble){
    struct tile*** tileArray = board->tileArray;

    //makes sure known double is actually a double
    if(knownDouble->numberOfPotentialNums != 2){
        return 0;
    }


    //gets values known double
    int value1 = -1;
    int value2 = -1;
    for(int num = 1; num < 10; num++){
        if(knownDouble->potentialNums[num] == 0){
            continue;
        }

        if(value1 == -1){
            value1 = num;
        } else {
            value2 = num;
        }
    }


    //checks box for other double's values
    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            //skips if on known tile
            if(x == knownDouble->xCor && y == knownDouble->yCor){
                continue;
            }

            struct tile* tileOn = tileArray[y][x];
            if(tileOn->numberOfPotentialNums != 2){
                continue;
            }


            if(tileOn->potentialNums[value1] == 0){
                continue;
            }

            if(tileOn->potentialNums[value2] == 0){
                continue;
            }

            int ignore[10] = {0};
            int ignoreTile1Index = (knownDouble->yCor - startTileY) * 3 + (knownDouble->xCor - startTileX);
            int ignoreTile2Index = (tileOn->yCor - startTileY) * 3 + (tileOn->xCor - startTileX);
            ignore[ignoreTile1Index] = 1;
            ignore[ignoreTile2Index] = 1;

            updateBoxIgnoretiles(board, box, value1, ignore);
            updateBoxIgnoretiles(board, box, value2, ignore);
            checkBox(board, box);
            return 1;
        }
    }

    return 0;
}

// Function: checkBoxForPointers
// Description: 
// Parameters: 
int checkBoxForPointers(struct board* board, struct box* box, int numberDecreased){
    struct tile*** tileArray = board->tileArray;

    //needs to have 2 potential numbers
    if(box->potentialNumsInBox[numberDecreased] != 2){
        return 0;
    }

    struct tile* tile1 = NULL;
    struct tile* tile2 = NULL;

    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            struct tile* tileOn = tileArray[y][x];
            if(tileOn->potentialNums[numberDecreased] == 1){
                
                if(tile1 == NULL){
                    tile1 = tileOn;
                } else {
                    tile2 = tileOn;
                }
            }
        }
    }

    //could not find both tiles
    if(tile2 == NULL){
        return 0;
    }


    // tests if tile1 and tile2 is in column
    if(tile1->xCor == tile2->xCor){
        int ignore[9] = {0};
        ignore[tile1->yCor] = 1;
        ignore[tile2->yCor] = 1;

        struct row* col = board->colArray[tile1->xCor];
        updateRowIgnoreTiles(board, col, numberDecreased, ignore);
        checkRow(board, col);
        return 1;
    
      // tests if tile1 or tile2 is in a row
    } else if(tile1->yCor == tile2->yCor){
        int ignore[9] = {0};
        ignore[tile1->xCor] = 1;
        ignore[tile2->xCor] = 1;


        struct row* row = board->rowArray[tile1->yCor];
        updateRowIgnoreTiles(board, row, numberDecreased, ignore);
        checkRow(board, row);
        return 1;
    }

    return 0;
}

int checkBoxesForPointers(struct board* board){
    struct box*** boxArray = board->boxArray;
    int hasUpdates = 0;
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            struct box* boxOn = boxArray[y][x];
            for(int number = 1; number < 10; number++){
                
                if(boxOn->potentialNumsInBox[number] != 2){
                    continue;
                }

                if(boxOn->pointerChecked[number] == 1){
                    continue;
                }

                boxOn->pointerChecked[number] = 1;
                hasUpdates += checkBoxForPointers(board, boxOn, number);
            }
        }
    }
    return hasUpdates;
}


int checkTilesForPairs(struct board* board){
    int hasUpdates = 0;
    struct tile*** tileArray = board->tileArray;
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            struct tile* tileOn = tileArray[y][x];
            if(tileOn->numberOfPotentialNums!= 2){
                continue;
            }

            if(tileOn->testedForPair == 1){
                continue;
            }

            tileOn->testedForPair = 1;
            struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            hasUpdates += checkBoxForPairs(board, box, tileOn);
        }
    }
    return hasUpdates;
}

// Function: checkRow
// Description: checks all tiles in a row if they only have 1 potential number
//              if so, sets the tile to the only potential number
// Parameters: board - stores the board state used to pass to setTile function call
//              row - pointer to the row you are testing
int checkRow(struct board* board, struct row* row){
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;

    for(int i = 0; i < 9; i++){
        struct tile* tileOn = NULL;
        if(row->isColumn == 1){
            tileOn = tileArray[i][row->rowNum];
        } else {
            tileOn = tileArray[row->rowNum][i];
        }
        
        //only 1 number is available
        if(tileOn->numberOfPotentialNums == 1 && tileOn->num == -1){

            //looks for available tile and sets it to that
            for(int i = 1; i < 10; i++){
                if(tileOn->potentialNums[i] == 1){
                    setTile( tileOn->xCor, tileOn->yCor, i, board);
                    hasUpdates = 1;
                }
            }

        }
    }

    return hasUpdates;
}


// Function: checkBox
// Description: checks all tiles in a box if they only have 1 potential number
//              if so, sets the tile to the only potential number
// Parameters: board - stores the board state used to pass to setTile function call
//              box - pointer to the box you are testing
int checkBox(struct board* board, struct box* box){
    int hasUpdates = 0;
    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            struct tile* tileOn = board->tileArray[y][x];


            //only 1 number is available
            if(tileOn->numberOfPotentialNums == 1){

                //looks for available tile and sets it to that
                for(int i = 1; i < 10; i++){
                    if(tileOn->potentialNums[i] == 1){
                        setTile( tileOn->xCor, tileOn->yCor, i, board);
                        hasUpdates = 1;
                    }
                }
            }
        }
    }

    return hasUpdates;
}


int checkAllPotentialNumbersInRow(struct board* board, struct row* row){
    int hasUpdates = 0;
    struct tile*** tileArray = board->tileArray;
    for(int number = 1; number < 10; number++){
        if(row->potentialNumsInRow[number] != 1){
            continue;
        }

        for(int i = 0; i < 9; i++){
            struct tile* tileOn = NULL;
            if(row->isColumn == 1){
                tileOn = tileArray[i][row->rowNum];
            } else {
                tileOn = tileArray[row->rowNum][i];
            }

            if(tileOn->potentialNums[number] == 1){
                setTile(tileOn->xCor, tileOn->yCor, number, board);
                hasUpdates = 1;
            }
        }
    }
    return hasUpdates;
}


int checkAllPotentialNumberInBox(struct board* board, struct box* box){
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;
    int startX = box->xCor * 3;
    int startY = box->yCor * 3;

    for(int number = 1; number < 10; number++){

        if(box->potentialNumsInBox[number] != 1){
            continue;
        }

        for(int tileY = startY; tileY <startY + 3; tileY++){
            for(int tileX = startX; tileX < startX + 3; tileX++){
                struct tile* tileOn = tileArray[tileY][tileX];

                if(tileOn->potentialNums[number] == 1){
                    setTile(tileOn->xCor, tileOn->yCor, number, board);
                    hasUpdates = 1;
                }
            }
        }
    }
    return hasUpdates;
}


int checkRowsForOnePotentialNumber(struct board* board, int number){
    struct row** rowArray = board->rowArray;
    struct row** colArray = board->colArray;
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;

    for(int rowNum = 0; rowNum < 9; rowNum++){
        struct row* rowOn = rowArray[rowNum];
        if(rowOn->potentialNumsInRow[number] != 1){
            continue;
        }

        for(int i = 0; i < 9; i++){
            struct tile* tileOn = tileArray[rowNum][i];

            if(tileOn->potentialNums[number] == 1){
                setTile(tileOn->xCor, tileOn->yCor, number, board);
                hasUpdates = 1;
            }
        }
    }

    for(int colNum = 0; colNum < 9; colNum++){

        struct row* colOn = colArray[colNum];

        if(colOn->potentialNumsInRow[number] != 1){
            continue;
        }

        for(int i = 0; i < 9; i++){
            struct tile* tileOn = tileArray[i][colNum];

            if(tileOn->potentialNums[number] == 1){
                setTile(tileOn->xCor, tileOn->yCor, number, board);
                hasUpdates = 1;
            }
        }
    }

    return hasUpdates;
}

int checkBoxesForOnePotentialNumber(struct board* board, int number){
    struct box*** boxArray = board->boxArray;
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;

    for(int y = 0; y < 2; y++){
        for(int x = 0; x < 2; x++){

            struct box* boxOn = boxArray[y][x];
            if(boxOn->potentialNumsInBox[number] != 1){
                continue;
            }

            for(int tileY = y * 3; tileY < (y + 1) * 3; tileY++){
                for(int tileX = x * 3; tileX < (x+1) * 3; tileX++){
                    struct tile* tileOn = tileArray[tileY][tileX];

                    if(tileOn->potentialNums[number] == 1){
                        setTile(tileOn->xCor, tileOn->yCor, number, board);
                        hasUpdates = 1;
                    }
                }
            }
        }
    }

    return hasUpdates;
}



// Function: setTile
// Description: sets a tile to a number
//              if tile is invalid it will return -1
//              if tile is valid will set tile to that number
//                  will update tile's row col and box, to not include that number
//                  will check tile's row col and box if they have only one potential number   
//              Will not check for there being only 1 potential number 
// Parameters: x - x cord of the the tile
//              y - y cord of tile
//              number - number your setting tile to 
//              board - stores the board state, used to pass to other functions
int setTile(int x, int y, int number, struct board* board){
    struct box* box = getBoxFromCords(board->boxArray, x, y);
    struct row* row = board->rowArray[y];
    struct row* col = board->colArray[x];
    struct tile* tile = board->tileArray[y][x];

    //returns if number is not a available number on the tile
    if(tile->potentialNums[number] == 0){
        return -1;
    }

    box->potentialNumsInBox[number] = 0;
    row->potentialNumsInRow[number] = 0;
    col->potentialNumsInRow[number] = 0;
    tile->num = number;
    tile->potentialNums[number] = 0;

    //sets all numbers to be not available
    //updates each row & box to decrease the option
    for(int i = 0; i < 10; i++){
        if(tile->potentialNums[i] == 1){
            tile->potentialNums[i] = 0;

            if(row->potentialNumsInRow[i] != 0){
                row->potentialNumsInRow[i]--;
            }

            if(col->potentialNumsInRow[i] != 0){
                col->potentialNumsInRow[i]--;
            }

            if(box->potentialNumsInBox[i] != 0){
                box->potentialNumsInBox[i]--;
            }

            // // checks for pointers
            // if(box->potentialNumsInBox[i] == 2){
            //     checkBoxForPointers(board, box, i);
            // }
        }
    }

    updateBox(board, box, number);
    updateRow(board, row, number);
    updateRow(board, col, number);

    int hasUpdates = 0;
    do{
        hasUpdates = 0;
        hasUpdates += checkRow(board, row);
        hasUpdates += checkRow(board, col);
        hasUpdates += checkBox(board, box);

        hasUpdates += checkBoxesForOnePotentialNumber(board, number);
        hasUpdates += checkRowsForOnePotentialNumber(board, number);
        hasUpdates += checkAllPotentialNumbersInRow(board, row);
        hasUpdates += checkAllPotentialNumbersInRow(board, col);
        hasUpdates += checkAllPotentialNumberInBox(board, box);
        hasUpdates += checkBoxesForPointers(board);
        hasUpdates += checkTilesForPairs(board);
    } while(hasUpdates >  0);

    return 1;
}

// Function: getBoxFromCords
// Description: returns the box associated with the tile on the grid
//              returns null if cords are out of bounds
// Parameters: boxArray - array of all boxes on board
//      x - x cordinate
//      y - y cordinate
struct box* getBoxFromCords(struct box*** boxArray, int x, int y){
    if(x < 0 || 8 < x){
        return NULL;
    }

    if(y < 0 || 8 < y){
        return NULL;
    }

    int xCor = x / 3;
    int yCor = y / 3;

    return boxArray[yCor][xCor];
}


// Function: printBoard
// Description: prints out the state of the board
// Parameters: tile array - contains all the tiles
//              boardSelector - stores what tile the selector is on
void printBoard(struct tile*** tileArray, struct selector* boardSelector){
    printf("\n\n");
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(boardSelector->x == x && boardSelector->y == y){
                printf("[");
            } else {
                printf(" ");
            }

            if(tileArray[y][x]->num == -1){
                printf("·");
            } else {
                printf("%d", tileArray[y][x]->num);
            }

            if(boardSelector->x == x && boardSelector->y == y){
                printf("]");
            } else {
                printf(" ");
            }


            if(x == 2 || x == 5){
                printf("|");
            }
        }
        printf("\n");
        if(y == 2 || y == 5){
            printf("-----------------------------\n");
        }
    }
}



int main(){
    struct tile*** tileArray;
    struct box*** boxArray;
    struct row** rowArray;
    struct row** colArray;
    struct selector boardSelector;

    int sizeOfBoxes = 3;
    int numberOfBoxes = sizeOfBoxes * sizeOfBoxes;
    int rowLength = sizeOfBoxes * sizeOfBoxes;
    int numberOfTiles = rowLength * rowLength;

    boardSelector.x = 0;
    boardSelector.y = 0;

    tileArray = malloc(rowLength * sizeof(struct tile**));
    for(int y = 0; y < rowLength; y++){
        tileArray[y] = malloc(rowLength * sizeof(struct tile*));
        for(int x = 0; x < rowLength; x++){
            tileArray[y][x] = makeTile(x, y);
        }
    }

    boxArray = malloc(sizeOfBoxes * sizeof(struct box**));
    for(int y = 0; y < sizeOfBoxes; y++){
        boxArray[y] = malloc(sizeOfBoxes * sizeof(struct box*));
        for(int x = 0; x < sizeOfBoxes; x++){
            boxArray[y][x] = makeBox(x, y);
        }
    }

    colArray = malloc(rowLength * sizeof(struct row*));
    for(int i = 0; i < rowLength; i++){
        colArray[i] = makeRow(i, 1);
    }

    rowArray = malloc(rowLength * sizeof(struct row*));
    for(int i = 0; i < rowLength; i++){
        rowArray[i] = makeRow(i, 0);
    }

    struct board board;
    board.tileArray = tileArray;
    board.rowArray = rowArray;
    board.colArray = colArray;
    board.boxArray = boxArray;
    board.selector = boardSelector;

    printBoard(tileArray, &boardSelector);
    while(1){

        char in;
        scanf(" %c",&in);
        fflush(stdin);

        if(in == 'a'){
            boardSelector.x = boardSelector.x - 1;
            if(boardSelector.x < 0){
                boardSelector.x = 8;
            }
        } else if(in == 'w'){
            boardSelector.y = boardSelector.y - 1;
            if(boardSelector.y < 0){
                boardSelector.y = 8;
            }
        } else if(in == 's'){
            boardSelector.y = boardSelector.y + 1;
            if(boardSelector.y > 8){
                boardSelector.y = 0;
            }
        } else if(in == 'd'){
            boardSelector.x = boardSelector.x + 1;
            if(boardSelector.x > 8){
                boardSelector.x = 0;
            }
        } else if(in == 'q'){
            break;
        }

        int inputAscii = in;
        int isValid = -2;
        inputAscii = inputAscii - 48;

        if(1 <= inputAscii && inputAscii <= 9){
            isValid = setTile(boardSelector.x, boardSelector.y, inputAscii, &board);
        } 

        printf("Tile: 2, 0: %d\n", tileArray[0][2]->numberOfPotentialNums);
        printBoard(tileArray, &boardSelector);

        if(isValid == -1){
            printf("Value: %d, is invalid at cords x: %d y:%d \n", inputAscii, boardSelector.x, boardSelector.y);
        }
    }   

    freeBoxArray(boxArray, sizeOfBoxes);
    freeTileArray(tileArray, rowLength);
    freeRowArray(rowArray, rowLength);
    freeRowArray(colArray, rowLength);
}

