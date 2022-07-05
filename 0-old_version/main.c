#include <stdio.h>
#include <stdlib.h>

#include "STD_TYPES.h"

/* Maximum posions that one player can play is 5 as
    as there is only 9 positions in the matrix */
#define MAX_NUM_OF_MOVES          5
#define NUM_OF_WIN_POSSIBILITIES  8

typedef enum En_Error{
    Error_no,
    Player1_won,
    Player2_won,
    Error_draw,
    Error_invalid_position,
    Error_used_poition
}En_Error_t;

typedef struct St_playerData{
    uint8_t positionsMatrix[MAX_NUM_OF_MOVES];
    uint8_t scoreSub;
    uint8_t scoreTotol;
    uint8_t numOfMoves;
    uint8_t playerChar;
}St_playerData_t;


uint8_t matrixGame[3][3]={'-'};
/*{
    {'-','-','-'},
    {'-','-','-'},
    {'-','-','-'}
};*/

uint8_t matrixPositions[3][3]={
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}
};

uint8_t matrixWinPositions[NUM_OF_WIN_POSSIBILITIES][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {1, 5, 9},
    {3, 5, 7},
    {3, 6, 9},
    {2, 5, 8},
    {1, 4, 7}
};

uint8_t numOfUsedPositions = 0;
uint8_t usedPositions[9] = {0};

void matrixPrint(uint8_t matrix[3][3]);
void matrixClear(uint8_t matrix[3][3]);
En_Error_t validateInput(uint8_t *playerInput);
En_Error_t performMove(St_playerData_t* playerx, uint8_t* matrixGame_arr);
En_Error_t checkScore(St_playerData_t* playerx, uint8_t* matrixGame_arr);
En_Error_t handleWinState(St_playerData_t* playerx);
void pritError(En_Error_t error);
void resetPlayerData(St_playerData_t* playerx);


int main(){
    St_playerData_t player1;
    player1.numOfMoves = 0;
    player1.scoreSub = 0;
    player1.playerChar = 'X';
    St_playerData_t player2;
    player2.numOfMoves = 0;
    player2.scoreSub = 0;
    player2.playerChar = 'O';
    En_Error_t error = Error_no;
    uint8_t turn = 1;
    uint8_t wantToContinue = 'y';
    matrixClear(matrixGame);

    while ('y' == wantToContinue){
        matrixPrint(matrixGame);
        printf("Choose one of these position:\n");
        matrixPrint(matrixPositions);
        printf("Please, make your move!");

        if(turn == 1){
            printf("\tPlayer X\n");
            scanf("%d", &(player1.positionsMatrix[player1.numOfMoves]));
            error = validateInput(&(player1.positionsMatrix[player1.numOfMoves]));
            pritError(error);
            if(error == Error_no){
                error = performMove(&player1, matrixGame);
                error = checkScore(&player1, matrixGame);
            }
            player1.numOfMoves++;
            turn = 2;
        }
        else if(turn == 2){
            printf("\tPlayer O\n");
            scanf("%d", &(player2.positionsMatrix[player2.numOfMoves]));
            error = validateInput(&(player2.positionsMatrix[player2.numOfMoves]));
            pritError(error);
            if(error == Error_no){
                error = performMove(&player2, matrixGame);
                error = checkScore(&player2, matrixGame);
            }
            player2.numOfMoves++;
            turn = 1;
        }
        if(Player1_won == error ){
            printf("***************************************\n");
            printf("CONGRATULATIONS!! PLAYER 1 (X) WON\n");
            matrixPrint(matrixGame);
            printf("Do you want to continue(y/n): ");
            scanf("%*c%c", &wantToContinue);
            matrixClear(matrixGame);
            resetPlayerData(&player1);
            resetPlayerData(&player2);
        }
        else if(Player2_won == error ){
            printf("***************************************\n");
            printf("CONGRATULATIONS!! PLAYER 2 (O) WON\n");
            matrixPrint(matrixGame);
            printf("Do you want to continue(y/n): ");
            scanf("%*c%c", &wantToContinue);
            matrixClear(matrixGame);
            resetPlayerData(&player1);
            resetPlayerData(&player2);
        }
        else if(player1.numOfMoves == 5 || player2.numOfMoves == 5){
            error = Error_draw;
            printf("***************************************\n");
            printf("DRAW!!\n");
            matrixPrint(matrixGame);
            printf("Do you want to continue(y/n): ");
            scanf("%*c%c", &wantToContinue);
            matrixClear(matrixGame);
            resetPlayerData(&player1);
            resetPlayerData(&player2);
        }
    }
    return 0;
}

void matrixClear(uint8_t matrix[3][3]){
    uint8_t counter1 = 0;
    uint8_t counter2 = 0;
    for (counter1 = 0; counter1 < 3; counter1++)
    {
        for (counter2 = 0; counter2 < 3; counter2++)
        {
            matrix[counter1][counter2] = '-';
        }
    }
}

void resetPlayerData(St_playerData_t* playerx){
    uint8_t i = 0;
    for(i = 0; i < MAX_NUM_OF_MOVES; i++){
        playerx->positionsMatrix[i] = 0;
    }
    playerx->numOfMoves = 0;
    playerx->scoreSub = 0;
    for(i = 0; i < 9; i++){
        usedPositions[i] = 0;
    }
}

/**
 * @brief Prints all the matrix content
 *
 * @param matrix
 */
void matrixPrint(uint8_t matrix[3][3]){
    uint8_t counter1 = 0;
    uint8_t counter2 = 0;

    for(counter1 = 0; counter1 < 3; counter1++){
        for (counter2 = 0; counter2 < 3; counter2++){
            printf("%c ", matrix[counter1][counter2]);
        }
        printf("\n");
    }
}

/**
 * @brief
 *
 * @param playerInput
 */
En_Error_t validateInput(uint8_t* playerInput){
    En_Error_t error = Error_no;
    uint8_t i = 0;
    printf("\t%d\n", *playerInput);
    if((*playerInput > 9) || (*playerInput < 1)){
        printf("Invalid position!! Please enter another one: \n");
        error = Error_invalid_position;
    }
    else{
        for(i = 0; i < 9; i++){
            if(*playerInput == usedPositions[i]){
                printf("This position is already used! Please enter another one: \n");
                error = Error_used_poition;
                break;
            }
            else{
                /* Do Nothing */
            }
        }
    }
    if(Error_no != error){
        scanf("%d", playerInput);
        validateInput(playerInput);
    }
    else{
        /* Do Nothing */
    }
    error = Error_no;
    return error;
}

/**
 * @brief
 *
 * @param playerx
 * @param matrixGame_arr
 * @return En_Error_t
 */
En_Error_t performMove(St_playerData_t* playerx, uint8_t* matrixGame_arr){
    En_Error_t error  = Error_no;

    uint8_t inputPosition = playerx->positionsMatrix[playerx->numOfMoves];
    usedPositions[numOfUsedPositions] = inputPosition;
    numOfUsedPositions++;
    /* Aray's indices are less than its poitions by 1 ex.(position 1 -> index 0) */
    matrixGame_arr[(inputPosition - 1)] = playerx->playerChar;
    return error;
}

En_Error_t checkScore(St_playerData_t* playerx, uint8_t* matrixGame_arr){
    uint8_t counter1 = 0;
    uint8_t counter2 = 0;
    uint8_t i = 0;
    En_Error_t error = Error_no;
    for(counter1 = 0; counter1 < NUM_OF_WIN_POSSIBILITIES; counter1++){
        for (i = 0; i < MAX_NUM_OF_MOVES; i++){
            for (counter2 = 0; counter2 < 3; counter2++){
                if (playerx->positionsMatrix[i] == matrixWinPositions[counter1][counter2]){
                    playerx->scoreSub++;
                }
                else{
                    /* Do Nothing */
                }
            }
        }
        if(playerx->scoreSub == 3 && playerx->playerChar == 'X'){
            error = Player1_won;
            break;
        }
        else if(playerx->scoreSub == 3 && playerx->playerChar == 'O'){
            error = Player2_won;
            break;
        }
        else{
            //printf("Subscore = %d\n", playerx->scoreSub);
            playerx->scoreSub = 0;
        }
    }
    printf("player inputs are: ");
    for (i = 0; i < MAX_NUM_OF_MOVES; i++)
    {
        printf("%d ", (playerx->positionsMatrix[i]));
    }
    printf("\n");
    return error;
}

void pritError(En_Error_t error){
    switch (error)
    {
    case Error_no:
        printf("\tNo error\n");
        break;
    case Player1_won:
        printf("\tPlayer1_won\n");
        break;
    case Player2_won:
        printf("\tPlayer1_won\n");
        break;
    case Error_draw:
       printf("\tDRAW\n");
        break;
    case Error_invalid_position:
        printf("\tError_invalid_position\n");
        break;
    case Error_used_poition:
        printf("\tError_used_poition\n");
        break;
    default:
        break;
    }
}
