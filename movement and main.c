int main(){
    int i, j, m, n;
    int X, Y, Xold, Yold, temp;

    int current_player = 1;

    int number_of_players = 2;
    int number_of_penguins;
    int unplaced_penguins_array[10];
    int player_points[10];

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

printf("==================== [ MOVEMENT PHASE ] ====================\n\n");
    delay(2000);

    int no_fish_left = 0;

    while(no_fish_left == 0){

        show_board(X_original, Y_original, board_tile_type, board_penguins_placement, board_fish_placement);

        printf("\n\n[ %d ] CURRENT PLAYER\n", current_player);

        movement_phase_player_types_in_coords_1:
        printf("Type the XY coordinates of the penguin you want to move:");
        scanf("%d %d", &Xold, &Yold);

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
        printf("[PLAYER %d] has %d points!\n", i, player_points[i]);
    }

    return 0;
}