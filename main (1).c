#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#import <math.h>

int rng(int min, int max){

    int rng;

    rng = (rand() % (max-min+1)) + min;

    return rng;
}

int rngperc(int perc){

    int rngperc;
    int min = 1;
    int max = 100;

    rngperc = (rand() % (max-min+1)) + min;

    if(rngperc > perc){
        return 0;
    }
    else{
        return 1;
    }
}

void delay(int milli_seconds){
    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds){};
}

void move_penguin(int X_original, int Y_original, int Xold, int Yold, int X, int Y, int current_player, int board_penguins_placement[X_original][Y_original]){
    board_penguins_placement[Xold][Yold] = 0;
    board_penguins_placement[X][Y] = current_player;
}

int check_if_move_valid(int X_original, int Y_original, int Xold, int Yold, int X, int Y, int current_player, int board_penguins_placement[X_original][Y_original], int board_tile_type[X_original][Y_original]){
    if(board_penguins_placement[Xold][Yold] == current_player && board_penguins_placement[X][Y] == 0 && board_tile_type[X][Y] == 1){
        if((X == Xold+1 || X == Xold-1) || (Y == Yold+1 || Y == Yold-1)){
            if(X == Xold+1 || X == Xold-1){
                if(Y == Yold+1 || Y == Yold-1){
                    return 0;
                }
                else{
                    return 1;
                }
            }
            else{
                return 1;
            }
        }
    }
    return 0;
}

int move_penguin_ownership_check(int X_original, int Y_original, int Xold, int Yold, int current_player, int board_penguins_placement[X_original][Y_original]){
    //// checks if the current player can move the penguin on X and Y (or if there even is a penguin there in the first place)
    if(board_penguins_placement[Xold][Yold] == current_player){
        return 1;
    }
    else{
        return 0;
    }
}

int check_if_any_fish_left(int X_original, int Y_original, int board_fish_placement[X_original][Y_original]){
    int i, j;
    for(i=1; i<=Y_original; i++){
        for(j=1; j<=X_original; j++){
            if(board_fish_placement[i][j] > 0){
                return 1;
            }
        }
    }
    return 0;
}

int generate_board(int X_original, int Y_original, int number_of_penguins, int number_of_players, int board_tile_type[X_original][Y_original], int board_penguins_placement[X_original][Y_original], int board_fish_placement[X_original][Y_original]){
    int i, j;

    /// minimum floe tiles needed for number_of_penguins:
    int a = number_of_penguins * number_of_players;

    /// optimal floe tiles needed for number_of_penguins:
    int b = a + round(X_original*Y_original-a)/2 + round(X_original*Y_original-a)/6;

    /// percentage of the map that has to be used for penguins:
    float c_min = (float)a / ((float)X_original * (float)Y_original);
    float c_opt = (float)b / ((float)X_original * (float)Y_original);

    if(X_original*Y_original < a + number_of_penguins){
        printf("Invalid parameters - the board can't be generated.\n");
        return -1;
    }
    else{
//        printf("a = %d; b = %d; c_min = %f; c_opt = %f; perc = %d%% \n\n", a, b, c_min, c_opt, (int)(c_opt * 100));
    }


    //// board generation can commence

    // set tile type
    for(i=1; i<=Y_original; i++){
        for(j=1; j<=X_original; j++){
            board_tile_type[i][j] = rngperc((int)(c_opt * 100));
        }
    }

    // set amount of fish on each ice floe
    for(i=1; i<=Y_original; i++){
        for(j=1; j<=X_original; j++){
            if(board_tile_type[i][j] == 1){
                board_fish_placement[i][j] = rng(1, 3);
            }
            else{
                board_fish_placement[i][j] = -1;
            }
        }
    }

    int good_tiles=0;
    for(i=1;i<X_original;i++)
    {
        for(j=1;j<Y_original;j++)
        {
            if(board_fish_placement[i][j]==1){
                good_tiles++;
            }
        }
    }
    while(good_tiles<number_of_penguins) {
        for (i = 1; i < Y_original; i++) {
            for (j = 1; j < X_original; j++) {
                if(board_fish_placement[i][j]!=1)
                {
                    board_fish_placement[i][j]=1;
                    good_tiles++;
                }
            }
        }
    }

    // create array for player penguins
    for(i=1; i<=Y_original; i++){
        for(j=1; j<=X_original; j++){
            if(board_tile_type[i][j] == 1){
                board_penguins_placement[i][j] = 0;
            }
            else{
                board_penguins_placement[i][j] = -1;
            }
        }
    }
    return 0;
}

int show_board(int X_original, int Y_original, int board_tile_type[X_original][Y_original], int board_penguins_placement[X_original][Y_original], int board_fish_placement[X_original][Y_original]){
    int i, j;

    printf("\n      ");

    printf("\t");
    for(i=1; i<=X_original; i++){
        if(i < 10){
            printf("[  %dY]\t", i);
        }
        else{
            printf("[ %dY]\t", i);
        }
    }

    printf("\n");
    for(i=1; i<=Y_original; i++){
        if(i < 10){
            printf("[  %dX]\t", i);
        }
        else{
            printf("[ %dX]\t", i);
        }

        for(j=1; j<=X_original; j++){
            if(board_tile_type[i][j] == 1){
                if(board_penguins_placement[i][j] > 0){
                    printf("<<%d>>\t", board_penguins_placement[i][j]);
                }
                else{
                    if(board_fish_placement[i][j] != 0){
                        printf("[ %d ]\t", board_fish_placement[i][j]);
                    }
                    else{
                        printf("[ X ]\t");
                    }
                }
            }
            else{
                printf("[   ]\t");
            }
        }
        printf("\n");
    }
    return 0;
}

int check_if_player_can_place_penguin_here(int X_original, int Y_original, int X, int Y, int board_tile_type[X_original][Y_original], int board_penguins_placement[X_original][Y_original]){
    if(board_tile_type[X][Y] == 1 && board_penguins_placement[X][Y] == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int only_one_fish(int X_original, int Y_original, int X, int Y, int current_player, int board_fish_placement[X_original][Y_original]){
    if(board_fish_placement[X][Y] == 1){
        return 1;
    }
    else{
        return -1;
    }
}

int place_penguin(int X_original, int Y_original, int X, int Y, int current_player, int board_penguins_placement[X_original][Y_original]){
    board_penguins_placement[X][Y] = current_player;
}

int claim_fish(int X_original, int Y_original, int X, int Y, int current_player, int player_points[], int board_fish_placement[X_original][Y_original]){
    if(board_fish_placement[X][Y] > 0){
        player_points[current_player] += board_fish_placement[X][Y];
        printf("\n\n%d fish claimed by [PLAYER %d] and his current score is %d\n\n", board_fish_placement[X][Y], current_player, player_points[current_player]);
        board_fish_placement[X][Y] = 0;
        return 1;
    }
    else{
        return 0;
    }
}

int switch_players(current_player, number_of_players){
    if(current_player + 1 > number_of_players){
        return 1;
    }
    else{
        return(current_player + 1);
    }
}



int main(){
    int i, j, m, n;
    int X, Y, Xold, Yold, temp;

    int current_player = 1;

    int number_of_players = 2;
    int number_of_penguins;
    int unplaced_penguins_array[10];
    int player_points[10];
    int player_surrender[10];

    for(i=1; i <= number_of_players; i++){
        player_points[i] = 0;
    }

    srand(time(NULL));

    printf("Type in board dimensions X and Y: \n");
    scanf("%d %d", &X, &Y);
    int X_original = X;
    int Y_original = Y;


    int board_tile_type[X][Y];
    //// 1 -- ice floe; 0 -- water
    int board_fish_placement[X][Y];
    //// penguins belonging to each player (0 - number_of_players)
    int board_penguins_placement[X][Y];


    type_in_the_number_of_penguins:
    printf("Type the number of penguins: \n");
    scanf("%d", &number_of_penguins);
    if(X_original * Y_original < number_of_penguins){
        printf("The board can't fit this amount of penguins!\n");
        goto type_in_the_number_of_penguins;
    }

    int unplaced_penguins = number_of_penguins * number_of_players;
    for(i=1; i < number_of_players+1; i++){
        unplaced_penguins_array[i] = number_of_penguins;
    }

    char resp = 'a';
    board_functions:

    generate_board(X_original, Y_original, number_of_penguins, number_of_players, board_tile_type, board_penguins_placement, board_fish_placement);
    show_board(X_original, Y_original, board_tile_type, board_penguins_placement, board_fish_placement);

    printf("Is the current board OK? (y/n): ");
    scanf(" %c", &resp);
    printf("\n\n\n\n");
    if(resp != 'y'){
        goto board_functions;
    }

    printf("\n\n==================== [ PLACEMENT PHASE ] ====================\n\n");
    while(unplaced_penguins != 0){
        for(i=1; i < number_of_players+1; i++){
            if(unplaced_penguins_array[i] != 0){

                show_board(X_original, Y_original, board_tile_type, board_penguins_placement, board_fish_placement);

                printf("\n\n[ %d ] CURRENT PLAYER\n",current_player);
                printf("\tHAS\n");
                printf("( %d ) PENGUINS LEFT TO PLACE\n", unplaced_penguins_array[i]);

                placement_phase_player_types_in_coords:
                printf("Type the XY coordinates to place your penguin: ");
                scanf("%d %d", &X, &Y);
                if(!check_if_player_can_place_penguin_here(X_original, Y_original, X, Y, board_tile_type, board_penguins_placement)){
                    printf("\nInvalid coordinates!\n\n");
                    delay(2000);
                    goto placement_phase_player_types_in_coords;
                }
                else{
                    temp = only_one_fish(X_original, Y_original, X, Y, current_player, board_fish_placement);
                    if(temp == -1){
                        printf("\nYou can only place your penguin on a tile with 1 fish!\n\n");
                        delay(2000);
                        goto placement_phase_player_types_in_coords;
                    }
                    else{
                        place_penguin(X_original, Y_original, X, Y, current_player, board_penguins_placement);
                        claim_fish(X_original, Y_original, X, Y, current_player, player_points, board_fish_placement);

                        unplaced_penguins_array[i] = unplaced_penguins_array[i] - 1;
                        unplaced_penguins--;

                        current_player = switch_players(current_player, number_of_players);
                        delay(500);
                    }
                }
            }
        }
    }
    printf("\n\nAll penguins placed!\n\n");

    printf("\n\n--------- | PLAYER SCOREBOARD | ---------\n",current_player);
    for(i=1; i <= number_of_players; i++){
        printf("[PLAYER %d] has %d points!\n", i, player_points[i]);
    }



    printf("\n\n\n\n==================== [ MOVEMENT PHASE ] ====================\n\n");
    delay(2000);

    printf("If you want to skip your move / request end of the game enter coordinates (0,0)");
    delay(5000);
    printf("\n\nWhen all players skip their turn right after each other the game ends\n\n");
    delay(6000);

    int no_fish_left = 0;
    int endgamevalue = 0;

    while(no_fish_left == 0){

        if(current_player == 1){
            for(i=1; i<= number_of_players; i++){
                player_surrender[i] = 0;
            }
        }

        show_board(X_original, Y_original, board_tile_type, board_penguins_placement, board_fish_placement);

        printf("\n\n[ %d ] CURRENT PLAYER\n", current_player);

        movement_phase_player_types_in_coords_1:
        printf("Type the XY coordinates of the penguin you want to move:");
        scanf("%d %d", &Xold, &Yold);

        if(Xold == 0||Yold == 0){
            player_surrender[current_player] = 1;
            endgamevalue = 0;
            for(i=1; i<= number_of_players; i++){
                endgamevalue += player_surrender[i];
            }

            printf("\n\n\n[PLAYER %d] SKIPPED HIS MOVE! (%d / %d)\n\n\n",current_player,endgamevalue,number_of_players);

            if(endgamevalue == number_of_players){
                break;
            }
            current_player = switch_players(current_player, number_of_players);
            continue;
        }

        if(move_penguin_ownership_check(X_original, Y_original, Xold, Yold, current_player, board_penguins_placement)){
            //// that penguin can be moved by the current player
            movement_phase_player_types_in_coords_2:
            printf("Type the coordinates where you want your penguin to be placed: ");
            scanf("%d %d", &X, &Y);
            if (!check_if_move_valid(X_original, Y_original, Xold, Yold, X, Y, current_player, board_penguins_placement, board_tile_type)){
                printf("\nInvalid coordinates!\n\n");
                delay(1000);
                goto movement_phase_player_types_in_coords_2;
            }
            else{
                printf("\n\nFISH ON [%d, %d] CLAIMED BY PLAYER [%d]!\n\n\n", X, Y, current_player);
                move_penguin(X_original, Y_original, Xold, Yold, X, Y, current_player, board_penguins_placement);
                board_tile_type[Xold][Yold] = 0;
                claim_fish(X_original, Y_original, X, Y, current_player, player_points, board_fish_placement);
            }
        }
        else{
            printf("\nThat penguin doesn't belong to you!\n\n");
            delay(1000);
            goto movement_phase_player_types_in_coords_1;
        }
        current_player = switch_players(current_player, number_of_players);
        if(!check_if_any_fish_left(X_original, Y_original, board_fish_placement)){
            no_fish_left = 1;
        }
        else{
            printf("\n\n--------- | PLAYER SCOREBOARD | ---------\n",current_player);
            for(i=1; i <= number_of_players; i++){
                printf("[PLAYER %d] has %d points!\n", i, player_points[i]);
            }

            printf("\n\n\n");
        }
    }
    printf("Game Over\n");

    printf("\n\n--------- | FINAL SCORE | ---------\n",current_player);
    for(i=1; i <= number_of_players; i++){
        printf("[PLAYER %d] got %d points in total!\n", i, player_points[i]);
    }
    printf("\n\n");
    int maxpoints = 0;
    int maxpointindx = 0;
    for(i=1; i <= number_of_players; i++){
        if(player_points[i] > maxpoints){
            maxpoints = player_points[i];
            maxpointindx = i;
        }
    }
    printf("[PLAYER %d] WON WITH %d POINTS!!\n", maxpointindx, player_points[maxpointindx]);

    return 0;
}