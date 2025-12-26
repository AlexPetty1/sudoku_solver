#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

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
    newTile->setByNotBruteForce = 0;

    newTile->potentialNums[0] = 0;
    for(int i = 1; i < 10; i++){
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
        newRow->hiddenPairChecked[i] = 0;
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
        newBox->doublePointerChecked[i] = 0;
        newBox->hiddenPairChecked[i] = 0;
    }
}

struct board* makeBoard(){
    struct tile*** tileArray;
    struct box*** boxArray;
    struct row** rowArray;
    struct row** colArray;
    struct selector boardSelector;

    boardSelector.x = 0;
    boardSelector.y = 0;

    tileArray = malloc(ROW_LENGTH * sizeof(struct tile**));
    for(int y = 0; y < ROW_LENGTH; y++){
        tileArray[y] = malloc(ROW_LENGTH * sizeof(struct tile*));
        for(int x = 0; x < ROW_LENGTH; x++){
            tileArray[y][x] = makeTile(x, y);
        }
    }

    boxArray = malloc(SIZE_OF_BOXES * sizeof(struct box**));
    for(int y = 0; y < SIZE_OF_BOXES; y++){
        boxArray[y] = malloc(SIZE_OF_BOXES * sizeof(struct box*));
        for(int x = 0; x < SIZE_OF_BOXES; x++){
            boxArray[y][x] = makeBox(x, y);
        }
    }

    colArray = malloc(ROW_LENGTH * sizeof(struct row*));
    for(int i = 0; i < ROW_LENGTH; i++){
        colArray[i] = makeRow(i, 1);
    }

    rowArray = malloc(ROW_LENGTH * sizeof(struct row*));
    for(int i = 0; i < ROW_LENGTH; i++){
        rowArray[i] = makeRow(i, 0);
    }

    struct board* board = malloc(sizeof(struct board));

    board->tileArray = tileArray;
    board->rowArray = rowArray;
    board->colArray = colArray;
    board->boxArray = boxArray;
    board->selector = boardSelector;

    return board;
}


int resetBoard(struct board* board){
    if(board == NULL){
        return -1;
    }

    //resets tiles
    for(int y = 0; y < ROW_LENGTH; y++){
        for(int x = 0; x < ROW_LENGTH; x++){
            struct tile* tileOn = board->tileArray[y][x];
            tileOn->num = -1;
            tileOn->numberOfPotentialNums = 9;
            tileOn->testedForPair = 0;
            tileOn->setByNotBruteForce = 0;
            tileOn->potentialNums[0] = 0;

            for(int i = 1; i < 10; i++){
                tileOn->potentialNums[i] = 1;
            }
        }
    }

    //resets boxes
    for(int y = 0; y < SIZE_OF_BOXES; y++){
        for(int x = 0; x < SIZE_OF_BOXES; x++){
            struct box* boxOn = board->boxArray[y][x];
            for(int i = 0; i < 10; i++){
                boxOn->potentialNumsInBox[i] = 9;
                boxOn->doublePointerChecked[i] = 0;
                boxOn->hiddenPairChecked[i] = 0;
            }  
        }
    }

    //reset rows
    for(int i = 0; i < ROW_LENGTH; i++){
        struct row* rowOn = board->rowArray[i];
        for(int j = 0; j < 10; j++){
            rowOn->potentialNumsInRow[j] = 9;
            rowOn->hiddenPairChecked[j] = 0;
        }
    }

    //reset cols
    for(int i = 0; i < ROW_LENGTH; i++){
        struct row* colOn = board->colArray[i];
        for(int j = 0; j < 10; j++){
            colOn->potentialNumsInRow[j] = 9;
            colOn->hiddenPairChecked[j] = 0;
        }
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

// Function: freeBoard
// Description: frees the all the boards memory and sets it to null
// Parameters: board - 
void freeBoard(struct board* board){
    freeBoxArray(board->boxArray, SIZE_OF_BOXES);
    freeTileArray(board->tileArray, ROW_LENGTH);
    freeRowArray(board->rowArray, ROW_LENGTH);
    freeRowArray(board->colArray, ROW_LENGTH);
    free(board);
    board = NULL;
}

// Function: updateRow
// Description: updates the row to have the numberAddedToRow, 
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

            //updates box and other row of tile
            struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            if(otherRow->potentialNumsInRow[numberAddedToRow] != 0){
                otherRow->potentialNumsInRow[numberAddedToRow]--;
            }

            if(box->potentialNumsInBox[numberAddedToRow] != 0){
                box->potentialNumsInBox[numberAddedToRow]--;
            }
        }
    }

}


// Function: updateRow
// Description: a modification to update row, which removes the potential number of numberAddedToRow.
//              from each of the tiles, but skips tiles in tiles ignored array
//              Is useful for when checking the box for pointers test
// Parameters: tileArray - the array of all tiles
//              boxArray - pointer to the box being added to
//              numberAddedToBox - the number is added to the box
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

            //updates box and other row of tile
            struct box* box = getBoxFromCords(board->boxArray, tileOn->xCor, tileOn->yCor);
            if(otherRow->potentialNumsInRow[numberAddedToRow] != 0){
                otherRow->potentialNumsInRow[numberAddedToRow]--;
            }

            if(box->potentialNumsInBox[numberAddedToRow] != 0){
                box->potentialNumsInBox[numberAddedToRow]--;
            }
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
//              call this function once a tile reaches 2 potential numbers
// Parameters: board - saves board state
//              box - box that is being tested
//              knownDouble - tile that is confirmed to have 2 potential numbers aka as a double
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

            updateBoxIgnoretiles(board, box, value1, ignore);   //calls ignore tile version to skip the double pair
            updateBoxIgnoretiles(board, box, value2, ignore);
            checkBox(board, box);
            return 1;
        }
    }

    return 0;
}

// Function: checkBoxForDoublePointers
// Description: checks a box for a double pointer then updates the row if so
//                  a double pointer is where there 2 potential places for a number in a box
//                  and they are both in same row or column, if found you can remove all other
//                  potential cases of the number from the 
// Parameters: 
int checkBoxForDoublePointers(struct board* board, struct box* box, int numberDecreased){
    struct tile*** tileArray = board->tileArray;

    //needs to have 2 potential numbers
    if(box->potentialNumsInBox[numberDecreased] != 2){
        return 0;
    }

    if(box->doublePointerChecked[numberDecreased] == 1){
        return 0;
    }
    box->doublePointerChecked[numberDecreased] = 1;


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



// Function: checkBoxForTriplePointers
// Description: checks for a triple pointer in a box
//              a triple pointer is where there are only 3 places a tile can go in a box
//              
// Parameters: board - 
//              box - box testing
//              numberDecreased - number testing for if a triple is in box
int checkBoxForTriplePointers(struct board* board, struct box* box, int numberDecreased){
    struct tile*** tileArray = board->tileArray;

    //needs to have 3 potential numbers
    if(box->potentialNumsInBox[numberDecreased] != 3){
        return 0;
    }

    if(box->triplePointerChecked[numberDecreased] == 1){
        return 0;
    }
    box->triplePointerChecked[numberDecreased] = 1;

    struct tile* tile1 = NULL;
    struct tile* tile2 = NULL;
    struct tile* tile3 = NULL;
    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            struct tile* tileOn = tileArray[y][x];
            if(tileOn->potentialNums[numberDecreased] == 1){
                
                if(tile1 == NULL){
                    tile1 = tileOn;
                } else if(tile2 == NULL){
                    tile2 = tileOn;
                } else {
                    tile3 = tileOn;
                }
            }
        }
    }

    //could not find both tiles
    if(tile3 == NULL){
        return 0;
    }

    // tests if tile1 and tile2 is in column
    if(tile1->xCor == tile2->xCor){
        int ignore[9] = {0};
        ignore[tile1->yCor] = 1;
        ignore[tile2->yCor] = 1;
        ignore[tile3->yCor] = 1;

        struct row* col = board->colArray[tile1->xCor];
        updateRowIgnoreTiles(board, col, numberDecreased, ignore);
        checkRow(board, col);
        return 1;
    
      // tests if tile1 or tile2 is in a row
    } else if(tile1->yCor == tile2->yCor){
        int ignore[9] = {0};
        ignore[tile1->xCor] = 1;
        ignore[tile2->xCor] = 1;
        ignore[tile3->xCor] = 1;

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

            //checks for doubles
            for(int number = 1; number < 10; number++){
                if(boxOn->potentialNumsInBox[number] == 2){
                    boxOn->doublePointerChecked[number] = 1;
                    hasUpdates += checkBoxForDoublePointers(board, boxOn, number);
                }

                if(boxOn->potentialNumsInBox[number] == 3){
                    boxOn->triplePointerChecked[number] = 1;
                    hasUpdates += checkBoxForTriplePointers(board, boxOn, number);
                }
            }
        }
    }
    return hasUpdates;
}

// Function: checkTilesForPairs
// Description: checks all the tiles for if they can form a pair with another
// Parameters: board - stores the board state
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


int checkAllTilesForSingles(struct board* board){
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            struct tile* tileOn = tileArray[y][x];
            if(tileOn->numberOfPotentialNums != 1){
                continue;
            }

            //has 1 potential looks for available number
            for(int num = 1; num < 10; num++){
                if(tileOn->potentialNums[num] == 1){
                    setTile( tileOn->xCor, tileOn->yCor, num, board);
                    hasUpdates = 1;
                }
            }
            
        }
    }

    return hasUpdates;
}

// Function: checkRowsForHiddenSingles
// Description: checks all rows for hidden singles
// Parameters: board - stores the state of the board
int checkRowsForHiddenSingles(struct board* board){
    struct row** rowArray = board->rowArray;
    struct row** colArray = board->colArray;

    int hasUpdates = 0;
    for(int rowNum = 0; rowNum < 9; rowNum++){
        struct row* row = rowArray[rowNum];
        hasUpdates += checkRowForHiddenSingle(board, row);
    }

    for(int colNum = 0; colNum < 9; colNum++){
        struct row* col = colArray[colNum];
        hasUpdates += checkRowForHiddenSingle(board, col);
    }

    return hasUpdates;
}

// Function: checkBoxForHiddenSingle
// Description: checks a row for a hidden single
//                  a hidden single is where only one tile in a row can have that number
//                  so the tile must be that number
// Parameters: board - stores the state of the board
//             box - box to check for hidden singles
int checkRowForHiddenSingle(struct board* board, struct row* row){
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;
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


// Function: checkBoxesForHiddenSingles
// Description: checks all boxes for hidden singles
// Parameters: board - stores the state of the board
int checkBoxesForHiddenSingles(struct board* board){
    int hasUpdates = 0;
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            struct box* boxOn = board->boxArray[y][x];
            hasUpdates += checkBoxForHiddenSingle(board, boxOn);
        }
    }

    return hasUpdates;
}

// Function: checkBoxForHiddenSingle
// Description: checks a box for a hidden single
//                  a hidden single is where only one tile in a box can have that number
//                  so the tile must be that number
// Parameters: board - stores the state of the board
//             box - box to check for hidden singles
int checkBoxForHiddenSingle(struct board* board, struct box* box){
    struct tile*** tileArray = board->tileArray;

    int hasUpdates = 0;
    for(int number = 1; number < 10; number++){
        //ensures there is only 1 option for the number in the box
        if(box->potentialNumsInBox[number] != 1){
            continue;
        }

        int startTileX = box->xCor * 3;
        int startTileY = box->yCor * 3;

        for(int tileY = startTileY; tileY < startTileY + 3; tileY++){
            for(int tileX = startTileX; tileX < startTileX + 3; tileX++){
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


// Function: checkBoxesForHiddenPairs
// Description: checks all boxes for a hidden pair
// Parameters: board - stores the state of the board
int checkBoxesForHiddenPairs(struct board* board){
    struct box*** boxArray = board->boxArray;
    int hasUpdates = 0;
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            struct box* box = boxArray[y][x];
            hasUpdates += checkBoxForHiddenPair(board, box);
        }
    }
    return hasUpdates;
}


// Function: checkRowsForHiddenPair
// Description: checks all rows and cols in a board for hidden pairs
// Parameters: board - stores the state of the board
int checkRowsForHiddenPairs(struct board* board){
    struct row** rowArray = board->rowArray;
    struct row** colArray = board->colArray;
    int hasUpdates = 0;
    
    for(int i = 0; i < 9; i++){
        struct row* row = rowArray[i];
        hasUpdates += checkRowForHiddenPair(board, row);
    }

    for(int i = 0; i < 9; i++){
        struct row* col = colArray[i];
        hasUpdates += checkRowForHiddenPair(board, col);
    }

    return hasUpdates;
}



// Function: checkRowForHiddenPair
// Description: checks a row for a hidden pair
//                  a hidden pair is when 2 numbers can only be found on 2 tiles in a row
//                  any other potential number can be removed from the tiles 
// Parameters: board - stores the state of the board
//             row - row to be checked for a hidden pair
int checkRowForHiddenPair(struct board* board, struct row* row){
    struct tile*** tileArray = board->tileArray;
    int hasUpdates = 0;
    for(int number = 1; number < 10; number++){
        if(row->potentialNumsInRow[number] != 2){
            continue;
        }

        if(row->hiddenPairChecked[number] == 1){
            continue;
        }
        row->hiddenPairChecked[number] = 1;

        struct tile* tile1 = NULL;
        struct tile* tile2 = NULL;
        
        //gets 2 tiles that have number as a potential tile
        for(int i = 0; i < 9; i++){
            struct tile* tileOn = NULL;
            if(row->isColumn == 1){
                tileOn = tileArray[i][row->rowNum];
            } else {
                tileOn = tileArray[row->rowNum][i];
            }

            if(tileOn->potentialNums[number] == 0){
                continue;
            }

            if(tile1 == NULL){
                tile1 = tileOn;
            } else {
                tile2 = tileOn;
            }
        }

        //look for other number with 2 potential in box and in both numbers if exists
        for(int number2 = 1; number2 < 10; number2++){
            if(number2 == number){
                continue;
            }

            if(row->potentialNumsInRow[number2] == 2 &&
                tile1->potentialNums[number2] == 1 &&
                tile2->potentialNums[number2] == 1){
                
                hasUpdates += updateTileHiddenPair(board, tile1, number, number2);
                hasUpdates += updateTileHiddenPair(board, tile2, number, number2);
            }
        }
    }
    return hasUpdates;
}


// Function: checkBoxForHiddenPair
// Description: checks a box for a hidden pair
//                  a hidden pair is when 2 numbers can only be found on 2 tiles in a box
//                  any other potential number can be removed from the tile 
// Parameters: board - stores the state of the board
//             box - box to be checked for a hidden pair
int checkBoxForHiddenPair(struct board* board, struct box* box){
    int hasUpdates = 0;
    for(int number = 1; number < 10; number++){
        if(box->potentialNumsInBox[number] != 2){
            continue;
        }

        if(box->hiddenPairChecked[number] == 1){
            continue;
        }
        box->hiddenPairChecked[number] = 1;

        struct tile* tile1 = NULL;
        struct tile* tile2 = NULL;
        
        //gets 2 tiles that have number as a potential tile
        int startTileX = box->xCor * 3;
        int startTileY = box->yCor * 3;
        for(int tileY = startTileY; tileY < startTileY + 3; tileY++){
            for(int tileX = startTileX; tileX < startTileX + 3; tileX++){
                struct tile* tileOn = board->tileArray[tileY][tileX];
                if(tileOn->potentialNums[number] == 0){
                    continue;
                }

                if(tile1 == NULL){
                    tile1 = tileOn;
                } else {
                    tile2 = tileOn;
                }
            }
        }

        //look for other number with 2 potential in box and in both numbers if exists
        for(int number2 = 1; number2 < 10; number2++){
            if(number2 == number){
                continue;
            }

            if(box->potentialNumsInBox[number2] == 2 &&
                tile1->potentialNums[number2] == 1 &&
                tile2->potentialNums[number2] == 1){
                
                hasUpdates += updateTileHiddenPair(board, tile1, number, number2);
                hasUpdates += updateTileHiddenPair(board, tile2, number, number2);
            }
        }
    }
    return hasUpdates;
}

// Function: updateTileHiddenPair
// Description: called from check for hidden pairs
//                  sets all potential numbers to 0, except for num1 and num2
//                  will return if potential numbers got updated
// Parameters: tile array - contains all the tiles
//              boardSelector - stores what tile the selector is on
int updateTileHiddenPair(struct board* board, struct tile* tile, int num1, int num2){
    struct box* box = getBoxFromCords(board->boxArray, tile->xCor, tile->yCor);
    struct row* row = board->rowArray[tile->yCor];
    struct row* col = board->colArray[tile->xCor];

    int hasUpdates = 0;
    for(int number = 1; number < 10; number++){
        if(number == num1){
            continue;
        }

        if(number == num2){
            continue;
        }

        if(tile->potentialNums[number] == 1){
            tile->potentialNums[number] = 0;
            tile->numberOfPotentialNums--;
            row->potentialNumsInRow[number]--;
            col->potentialNumsInRow[number]--;
            box->potentialNumsInBox[number]--;
            hasUpdates = 1;
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

    if(number < 1 || number > 9){
        return -1;
    }

    //returns if number is not a available number on the tile
    if(tile->potentialNums[number] == 0){
        return -1;
    }

    box->potentialNumsInBox[number] = 0;
    row->potentialNumsInRow[number] = 0;
    col->potentialNumsInRow[number] = 0;
    tile->num = number;
    tile->potentialNums[number] = 0;
    tile->setByNotBruteForce = 1;

    //sets all numbers in tile to not be available
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
        }
    }

    updateBox(board, box, number);
    updateRow(board, row, number);
    updateRow(board, col, number);

    int hasUpdates = 0;
    do{
        hasUpdates = 0;
        hasUpdates += checkAllTilesForSingles(board);

        hasUpdates += checkBoxesForHiddenSingles(board);
        hasUpdates += checkRowsForHiddenSingles(board);
        hasUpdates += checkBoxesForHiddenPairs(board);
        hasUpdates += checkRowsForHiddenPairs(board);

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
void printBoard(struct board* board){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(board->selector.x == x && board->selector.y == y){
                printf("[");
            } else {
                printf(" ");
            }

            if(board->tileArray[y][x]->num == -1){
                printf("·");
            } else {
                printf("%d", board->tileArray[y][x]->num);
            }

            if(board->selector.x == x && board->selector.y == y){
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
    printf("\n\n");
}


// Function: isTileAddedValid
// Description: tests if a tile is valid used in brute force algo
// Parameters: board - contains state of the board
//              x - x value of tile
//              y - y value of tile
int isTileAddedValid(struct board* board, int x, int y){

    struct tile*** tileArray = board->tileArray;

    //checks row
    int rowNumsSeen[10] = {0};
    for(int tileNum = 0; tileNum < 9; tileNum++){

        struct tile* tileOn = tileArray[y][tileNum];
        if(tileOn->num != -1){          //skips blank tiles
            
            //number has already been seen in row so return invalid board
            if(rowNumsSeen[tileOn->num] == 1){
                return 0;
            }

            rowNumsSeen[tileOn->num] = 1;
        }
    }

    //checks col
    int colNumsSeen[10] = {0};
    for(int tileNum = 0; tileNum < 9; tileNum++){

        struct tile* tileOn = tileArray[tileNum][x];
        if(tileOn->num != -1){
            
            //number has already been seen in row so return invalid board
            if(colNumsSeen[tileOn->num] == 1){
                return 0;
            }

            colNumsSeen[tileOn->num] = 1;
        }
    }

    //checks box
    int boxNumsSeen[10] = {0};
    int tileXStart = (x / 3) * 3;  //floors into to multiple of 3
    int tileYStart = (y / 3) * 3;
    for(int tileY = tileYStart; tileY < tileYStart + 3; tileY++){
        for(int tileX = tileXStart; tileX < tileXStart + 3; tileX++){

            struct tile* tileOn = tileArray[tileY][tileX];
            if(tileOn->num != -1){
                
                //number has already been seen in row so return invalid board
                if(boxNumsSeen[tileOn->num] == 1){
                    return 0;
                }

                boxNumsSeen[tileOn->num] = 1;
            }
        }  
    }
    return 1;
}

// Function: isTileAddedValid
// Description: tests if a board is valid used in brute force algo
// Parameters: board - contains state of the sudoku board
int isBoardValid(struct board* board){
    struct tile*** tileArray = board->tileArray;

    //checks rows
    for(int i = 0; i < 9; i++){
        int numsSeen[10] = {0};
        for(int tileNum = 0; tileNum < 9; tileNum++){
            struct tile* tileOn = tileArray[i][tileNum];
            if(tileOn->num != -1){
                
                //number has already been seen in row so return invalid board
                if(numsSeen[tileOn->num] == 1){
                    return 0;
                }

                numsSeen[tileOn->num] = 1;
            }
        }
    }

    //checks cols
    for(int i = 0; i < 9; i++){
        int numsSeen[10] = {0};
        for(int tileNum = 0; tileNum < 9; tileNum++){

            struct tile* tileOn = tileArray[tileNum][i];
            if(tileOn->num != -1){
                
                //number has already been seen in row so return invalid board
                if(numsSeen[tileOn->num] == 1){
                    return 0;
                }

                numsSeen[tileOn->num] = 1;
            }
        }
    }

    //checks boxes
    for(int boxY = 0; boxY < 3; boxY++){
        for(int boxX = 0; boxX < 3; boxX++){
            
            int numsSeen[10] = {0};
            int tileXStart = boxX * 3;
            int tileYStart = boxY * 3;


            for(int tileY = tileYStart; tileY < tileYStart + 3; tileY++){
                for(int tileX = tileXStart; tileX < tileXStart + 3; tileX++){

                    struct tile* tileOn = tileArray[tileY][tileX];
                    if(tileOn->num != -1){
                        
                        //number has already been seen in row so return invalid board
                        if(numsSeen[tileOn->num] == 1){
                            return 0;
                        }

                        numsSeen[tileOn->num] = 1;
                    }
                }
            }
        }   
    }
    return 1;
}

// Function: bruteForceOnTile
// Description: runs brute force on each tile then returns the next step needed
//          return 1 for valid tile
//          return -1 for invalid backtrace needed
//          return 0 if already set
// Parameters: board - contains all the tiles
//              x - x value of tile
//              y - y value of tile
int bruteForceOnTile(struct tile* tile, struct board* board, int direction){
    struct box* boxOn = getBoxFromCords(board->boxArray, tile->xCor, tile->yCor);
    struct row* rowOn = board->rowArray[tile->yCor];
    struct row* colOn = board->colArray[tile->xCor];

    //skips if tile is not set by brute force
    if(tile->setByNotBruteForce == 1){
        return 0;
    }

    // moves to next tile as the tile is valid
    if(tile->num != -1 && direction == 1){
        if(boxOn->potentialNumsInBox[tile->num] > 0 &&
            rowOn->potentialNumsInRow[tile->num] > 0 &&
            colOn->potentialNumsInRow[tile->num] > 0){
                
                return 1;
        }
    }

    //if backtracing tile will change and so opens tile up
    if(direction == -1 && tile->num != -1){
        boxOn->potentialNumsInBox[tile->num] = 1;
        rowOn->potentialNumsInRow[tile->num] = 1;
        colOn->potentialNumsInRow[tile->num] = 1;
    }

    //sets first number to test
    int numberTested;           
    if(tile->num == -1){
        numberTested = 1;
    } else {
        numberTested = tile->num + 1;
    }

    //looks for next tile
    while(numberTested < 10){
        if(tile->potentialNums[numberTested] == 1){     //makes sure tile is a potential number from normal solve

            if(boxOn->potentialNumsInBox[numberTested] > 0 &&
                rowOn->potentialNumsInRow[numberTested] > 0 &&
                colOn->potentialNumsInRow[numberTested] > 0){
                

                boxOn->potentialNumsInBox[numberTested] = 0;
                rowOn->potentialNumsInRow[numberTested] = 0;
                colOn->potentialNumsInRow[numberTested] = 0;
                tile->num = numberTested;
                return 1;
            }
        }

        numberTested++;
    }

    //could not find any more potential tiles 
    tile->num = -1;
    return -1;
}


// Function: bruteForceOnTile
// Description: runs brute force on each tile then returns the next step needed
//          return 1 for valid tile
//          return -1 for invalid backtrace needed
//          return 0 if already set
// Parameters: board - contains all the tiles
//              x - x value of tile
//              y - y value of tile
void bruteForceAlgorithm(struct board* board){
    struct tile*** tileArray = board->tileArray;
    struct box*** boxArray = board->boxArray;
    struct row** colArray = board->colArray;
    struct row** rowArray = board->rowArray;

    int tileOnX = 0;
    int tileOnY = 0;
    int direction = 1;
    int nextStep = 0;   // -1 if backtrace, 0 if tile already solved, 1 if move to next tile
    int index = 0;
    while(1){
        struct tile* tileOn = tileArray[tileOnY][tileOnX];
        struct box* boxOn = getBoxFromCords(board->boxArray, tileOnX, tileOnY);
        struct row* rowOn = board->rowArray[tileOnY];
        struct row* colOn = board->colArray[tileOnX];

        nextStep = bruteForceOnTile(tileOn, board, direction);
        
        switch (nextStep){
            case -1:
                direction = -1;
                tileOnX--;
                break;
            case 0:
                if(direction == 1){
                    tileOnX++;
                } else {
                    tileOnX--;
                }
                break;
            case 1:
                direction = 1;
                tileOnX++;
                break;

        };

        if(tileOnX == 9){
            tileOnX = 0;
            tileOnY++;
        }

        if(tileOnX == -1){
            tileOnX = 8;
            tileOnY--;
        }

        if(tileOnY == -1){
            printf("No solution\n");
            break;
        }
        

        if(tileOnY == 9){
            printf("Found a solution\n");
            break;
        }
        index++;
    }
}



void interactiveMode(struct board* board){

    char input = 'a'; 
    struct selector* boardSelector = &board->selector;

    getchar();  // fixes a bug with first input being ignored, might find a better fix later

    printf("Interactive mode, will attempt to solve the puzzle as you play\n"); 
    printf("    - Use wasd to move the selector\n");
    printf("    - Use 1-9 to input a number\n");
    printf("    - Enter b to bruteforce the solution after inputing all the numbers");
    printf("    - Enter q to quit\n");
    printf("Note: There is no way to delete, undo, or replace a number in this mode\n\n");


    // inside of insert tile mode
    while((input != 'b') && (input != 'q')){

        printBoard(board);

        //gets input and removes trailing characters
        input = getchar();
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if(input == 'a'){
            boardSelector->x = boardSelector->x - 1;
            if(boardSelector->x < 0){
                boardSelector->x = 8;
            }
        } else if(input == 'w'){
            boardSelector->y = boardSelector->y - 1;
            if(boardSelector->y < 0){
                boardSelector->y = 8;
            }
        } else if(input == 's'){
            boardSelector->y = boardSelector->y + 1;
            if(boardSelector->y > 8){
                boardSelector->y = 0;
            }
        } else if(input == 'd'){
            boardSelector->x = boardSelector->x + 1;
            if(boardSelector->x > 8){
                boardSelector->x = 0;
            }
        } 

        int inputAscii = input;
        int isTileValid = -2;
        inputAscii = inputAscii - '0';

        if(1 <= inputAscii && inputAscii <= 9){
            isTileValid = setTile(boardSelector->x, boardSelector->y, inputAscii, board);
        } 

        if(isTileValid == -1){
            printf("Value: %d, is invalid at cords x: %d y:%d \n", inputAscii, boardSelector->x, boardSelector->y);
        }
    }

    if(input == 'b'){
        struct timeval stop, start;
        gettimeofday(&start, NULL);

        bruteForceAlgorithm(board);

        gettimeofday(&stop, NULL);
        printf("took %lu microseconds for brute force\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

        printBoard(board);
    }

    if(input == 'q'){
        printf("Exiting from interactive mode\n\n");
    }
}

void stringMode(struct board* board){
    printf("\n\nString mode, enter a 81 character string to input the sudoku board\n");
    printf("Many sites with sudokus will provide the string\n");
    printf("    - Numbers are inputed left to right, top to bottom\n");
    printf("    - 1-9, will correspond to inputting a number at that place\n");
    printf("    - 0, is a blank tile\n\n");

    //gets rid rest of buff needed as switching from scanf to fgets is weird
    int c;
    while ( (c = getchar()) != EOF && c != '\n') { }

    char buffer[NUMBER_OF_TILES + 2];
    printf("Input a string: \n");
    fgets(buffer, sizeof(buffer), stdin);

    int bufferLen = strlen(buffer);
    if(buffer[bufferLen - 1] == '\n'){
        buffer[bufferLen - 1] = '\0';
        bufferLen = bufferLen - 1;
    }

    if(bufferLen > NUMBER_OF_TILES){
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("\nWarning, more than 81 characters string entered\n");
        printf("    Any characters above 81 will be ignored\n");
    }

    if(bufferLen < NUMBER_OF_TILES){
        printf("\nWarning, less than 81 characters string entered\n");
        printf("    Will assume the remaining tiles are blank\n");
    }

    int numTilesToEnter = NUMBER_OF_TILES;
    if(bufferLen < NUMBER_OF_TILES){
        numTilesToEnter = bufferLen;
    }

    struct board* displayBoard = makeBoard();   //display board is used as using set tile on real board will try and solve it
    int invalidChars = 0;
    int x = 0;
    int y = 0;
    for(int i = 0; i < numTilesToEnter; i++){
        x = i % 9;
        y = i / 9;

        char charOn = buffer[i];
        if('1' <= charOn && charOn <= '9'){
            int charOnInt = charOn - '0';
            setTile(x, y, charOnInt, board);

            //sets display board
            struct tile* tileOn = displayBoard->tileArray[y][x];
            tileOn->num = charOnInt;

        } else if (charOn == '0'){
            //blank on purpose
        } else {
            invalidChars++;
        }
    }

    if(invalidChars > 0){
        printf("Warning: %d invalid chars inputed in string\n", invalidChars);
        printf("    Invalid chars will be treated as blanks\n\n");
    }

    printf("\nInputted board: \n");
    printBoard(displayBoard);
    
    if(isBoardValid(displayBoard) == 0){
        printf("Inputted board is invalid!!\n\n");
        freeBoard(displayBoard);
        return;
    }

    bruteForceAlgorithm(board);
    printBoard(board);

    freeBoard(displayBoard);
}


void placeMode(struct board* board){
    
    struct board* displayBoard = makeBoard();   //display board is used for placing as it comes, 
                                                //  with print board and verify board functions instead of a basic 2d array
    struct tile*** tempBoardTiles = displayBoard->tileArray;

    char input = 'a'; 
    struct selector* boardSelector = &displayBoard->selector;
    int validAndReadyBoard = 0;

    getchar();  // fixes a bug with first input being ignored, might find a better fix later

    printf("\n\nPlace mode, place all the tiles then enter 'c' to crack the puzzle.\n"); 
    printf("    - Use wasd to move the selector\n");
    printf("    - Use 1-9 to input a number\n");
    printf("    - Enter 0 to delete a number\n");
    printf("    - Enter q to quit\n\n");

    while((validAndReadyBoard == 0) && (input != 'q')){
        printBoard(displayBoard);

        //gets input and removes trailing characters
        input = getchar();
        int c;
        while ((c = getchar()) != '\n' && c != EOF);


        if(input == 'a'){
            boardSelector->x = boardSelector->x - 1;
            if(boardSelector->x < 0){
                boardSelector->x = 8;
            }
        } else if(input == 'w'){
            boardSelector->y = boardSelector->y - 1;
            if(boardSelector->y < 0){
                boardSelector->y = 8;
            }
        } else if(input == 's'){
            boardSelector->y = boardSelector->y + 1;
            if(boardSelector->y > 8){
                boardSelector->y = 0;
            }
        } else if(input == 'd'){
            boardSelector->x = boardSelector->x + 1;
            if(boardSelector->x > 8){
                boardSelector->x = 0;
            }
        } else if('1' <= input && input <= '9'){
            int inputAscii = input - '0';

            //does not use setTile as that does not allow for replacing tile values
            //  and can lead to additional tiles being placed
            struct tile* tileOn = tempBoardTiles[boardSelector->y][boardSelector->x];  
            tileOn->num = inputAscii;
        } else if(input == '0'){
            struct tile* tileOn = tempBoardTiles[boardSelector->y][boardSelector->x];  
            tileOn->num = -1;                       //-1 means tile is a blank tile
        }else if(input == 'c'){
            if(isBoardValid(displayBoard)){
                validAndReadyBoard = 1;
            } else {
                printf("The board is currently invalid with at least 2 squares that conflict\n");
            }
        } else {
            printf("Input '%c' is invalid\n", input);
        }
    }

    struct timeval start, endNormal, endBrutforce;
    gettimeofday(&start, NULL);
    //sets tiles from the temp board on to the actual board used for solving
    for(int y = 0; y < ROW_LENGTH; y++){
        for(int x = 0; x < ROW_LENGTH; x++){
            struct tile* tileOn = tempBoardTiles[boardSelector->y][boardSelector->x];
            if(tileOn->num != -1){
                setTile(x, y, tileOn->num, board);
            }
        }
    }

    gettimeofday(&endNormal, NULL);
    


    bruteForceAlgorithm(board);
    gettimeofday(&endBrutforce, NULL);
    printf("took %lu microseconds for normal algorithm\n", (endNormal.tv_sec - start.tv_sec) * 1000000 + endNormal.tv_usec - start.tv_usec);
    printf("took %lu microseconds for brute force\n", (endBrutforce.tv_sec - endNormal.tv_sec) * 1000000 + endBrutforce.tv_usec - endNormal.tv_usec);
    printf("took %lu microseconds for whole process\n", (endBrutforce.tv_sec - start.tv_sec) * 1000000 + endBrutforce.tv_usec - start.tv_usec);

    printBoard(board);


    freeBoard(displayBoard);
}





int main(){
    struct board* board = makeBoard();
    int input = -1;

    do{
        //gets user input
        int result = -1;
        do{
            printf("Input:\n");
            printf("    1: Interactive mode\n");
            printf("    2: Place mode\n");
            printf("    3: String mode\n");
            printf("    4: Quit\n");
            printf(": ");

            result = scanf("%d", &input);
        }while((result == -1) ||
            (input < 1) ||
            (input > 4));


        switch(input){
            case 1:
                interactiveMode(board);
                break;
            case 2:
                placeMode(board);
                break;
            case 3: 
                stringMode(board);
                break;
        }


        resetBoard(board);
    } while(input != 4);
    

    
    freeBoard(board);
}

