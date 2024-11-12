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
        newRow->numsInRow[i] = 0;
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
        newBox->numsInBox[i] = 0;
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
void updateRow(struct tile*** tileArray, struct row* rowArray, int numberAddedToRow){
    rowArray->numsInRow[numberAddedToRow] = 1;

    for(int i = 0; i < 9; i++){
        struct tile* tileOn = NULL;
        if(rowArray->isColumn == 1){
            tileOn = tileArray[i][rowArray->rowNum];
        } else {
            tileOn = tileArray[rowArray->rowNum][i];
        }
            
        if(tileOn->potentialNums[numberAddedToRow] == 1){
            tileOn->potentialNums[numberAddedToRow] = 0;
            tileOn->numberOfPotentialNums = tileOn->numberOfPotentialNums - 1;
        }
    }

}


// Function: updateBox
// Description: updates the box to have the numberAddedToBox, 
//              updates each tile in box to not have numberAddedToBox among potential numbers
// Parameters: tileArray - the array of all tiles
//              boxArray - pointer to the box being added to
//              numberAddedToBox - the number is added to the box
void updateBox(struct tile*** tileArray, struct box* box, int numberAddedToBox){
    box->numsInBox[numberAddedToBox] = 1;

    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            struct tile* tileOn = tileArray[y][x];

            if(tileOn->potentialNums[numberAddedToBox] == 1){
                tileOn->potentialNums[numberAddedToBox] = 0;
                tileOn->numberOfPotentialNums = tileOn->numberOfPotentialNums - 1;
            }
        }
    }
}

// Function: checkRow
// Description: checks all tiles in a row if they only have 1 potential number
//              if so, sets the tile to the only potential number
// Parameters: board - stores the board state used to pass to setTile function call
//              row - pointer to the row you are testing
void checkRow(struct board* board, struct row* row){
    struct tile*** tileArray = board->tileArray;

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
                }
            }

        }
    }
}


// Function: checkBox
// Description: checks all tiles in a box if they only have 1 potential number
//              if so, sets the tile to the only potential number
// Parameters: board - stores the board state used to pass to setTile function call
//              box - pointer to the box you are testing
void checkBox(struct board* board, struct box* box){

    int startTileX = box->xCor * 3;
    int startTileY = box->yCor * 3;
    for(int y = startTileY; y < startTileY + 3; y++){
        for(int x = startTileX; x < startTileX + 3; x++){
            struct tile* tileOn = board->tileArray[y][x];


            //only 1 number is available
            if(tileOn->numberOfPotentialNums == 1 && tileOn->num == -1){

                //looks for available tile and sets it to that
                for(int i = 1; i < 10; i++){
                    if(tileOn->potentialNums[i] == 1){
                        setTile( tileOn->xCor, tileOn->yCor, i, board);
                    }
                }

            }
        }
    }
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

    box->numsInBox[number] = 1;
    row->numsInRow[number] = 1;
    col->numsInRow[number] = 1;
    tile->num = number;
    tile->potentialNums[number] = 0;

    updateBox(board->tileArray, box, number);
    updateRow(board->tileArray, row, number);
    updateRow(board->tileArray, col, number);

    checkRow(board, row);
    checkRow(board, col);
    checkBox(board, box);

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
    

    printf("Box array: %p, box array in board %p\n", boxArray, board.boxArray);

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

        printBoard(tileArray, &boardSelector);
        printf("Num potential: %d \n", tileArray[0][0]->numberOfPotentialNums);
        if(isValid == -1){
            printf("Value: %d, is invalid at cords x: %d y:%d \n", inputAscii, boardSelector.x, boardSelector.y);
        }
    }   



    freeBoxArray(boxArray, sizeOfBoxes);
    freeTileArray(tileArray, rowLength);
    freeRowArray(rowArray, rowLength);
    freeRowArray(colArray, rowLength);
}

