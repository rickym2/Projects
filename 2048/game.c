/*In this MP, the objective is to create a series of helper functions that mimic the function of the 2048 game. The main concepts used in this project are structs and pointers. The supporting helper functions include
*remake_game, make_game, get_cell, and get_cell. These functions primarily deal with board initialization, resizing, and cell access. Within these functions, dynamic memory allocation was utilized along with pointers
*to enable the functions of the game. The primary functions that were created are move_w, move_s, move_a, and move_d. These deal with tile movement and merges. The core logic remains the same in each, but they are 
*tweaked to ensure they properly deal with moving upwards, downwards, leftward, and rightward effectively. The final function, legal_move_check, is crucial in understanding if a legal move can be performed. Overall,
*I struggled the most with creating the move functions. The logic was difficult to grasp, but through trial and error I found a structure that worked utilizing different for loops for first sliding then merging.
*For instance, I had to adjust the boundaries for moving downwards as opposed to upwards. The logic is to instead start from  row = cur_game->-1 instead of row = 0;. Overall, the project served to improve my understanding
*of structs and pointers in a challenging manner.
*
*
*/
#include "game.h"


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->cols=cols; //We use this syntax because mygame is a pointer
    mygame->rows=rows;
    mygame->score=0;

    for(int i = 0; i < (rows*cols); i++) //initialize everything to -1
    { mygame->cells[i] = -1; }
    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*_cur_game_ptr)->rows = new_rows; //dereference the double pointer to new_rows pointer
    (*_cur_game_ptr)->cols = new_cols; //dereference the double pointer to new_cols
    (*_cur_game_ptr)->score = 0;

    for(int i = 0; i < (new_rows * new_cols); i++) //initialize everything to -1
    { (*_cur_game_ptr)->cells[i] = -1; } 

}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if(row < 0 || row >= cur_game->rows || col < 0 || col >= cur_game->cols) {
        return NULL;    //We return NULL if its < 0 or >= limit
    }
    return cur_game->cells + (row* cur_game->cols + col); // This returns the pointer plus  the offset
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    int move_occurred = 0; 
    int col, row;
    //We create our outer loop that deals with each column, because we are sliding upward, we only need to deal with the column in this outer loop as everything goes up in the column. 
    // Iterate through each col 
    for(col = 0; col < cur_game->cols; col++) {
        //This first section will deal with sliding everything upward.
        int insert_row = 0;

        for(row = 0; row < cur_game->rows; row++)  {   //Iterate through each row top to bottom
        cell *current_cell_ptr = get_cell(cur_game, row, col); //Create a new pointer to current cell using helper function get_cell
            if(*current_cell_ptr != -1) { // Check if empty
        
                if(insert_row != row) { //Check if it needs to be moved
        
                *get_cell(cur_game, insert_row, col) = *current_cell_ptr;  // Essentially moves tile to next spot
                 *current_cell_ptr = -1; // Set the cell to be empty 
                 move_occurred = 1; //A tile moved, so a move occurred.
                 }
            insert_row++; 
            }
         }
        
        // This next section will check to merge the tiles
        for(row = 0; row < cur_game->rows-1;row++) { //We stop at rows - 1 because we do not want to merge the last row with above since its empty
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            cell *next_cell_ptr = get_cell(cur_game, row + 1, col); // We make these two pointers to get current and next cell so we can do merging
                if(*current_cell_ptr != -1 && *current_cell_ptr == *next_cell_ptr) { //Logically, if both pointers are not -1 and are equal, than it is a valid condition for us to merge them.
                    *current_cell_ptr *= 2; // Double top tile
                    cur_game->score += *current_cell_ptr; // Add value to score
                    *next_cell_ptr = -1; // Make the merged cell empty
                    move_occurred = 1; //It merged, so a move occurred.
            }
        }
        // This final section will reslide tiles after merging them.
        insert_row = 0; // Reset the counter for insert_row

        for(row = 0; row < cur_game->rows; row++) { //This section utilized the same logic in our first section where we check if its empty and if the row is the same as our index, then we change it and set to empty.
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            if(*current_cell_ptr != -1) {
                if(insert_row != row) {
                    *get_cell(cur_game, insert_row, col) = *current_cell_ptr;
                    *current_cell_ptr = -1;
                }
                insert_row++;
            }
        }
    }
        return move_occurred;     
}



int move_s(game * cur_game) //slide down
{
    int move_occurred = 0; 
    int col, row;
    //We create our outer loop that deals with each column, because we are sliding downward, we only need to deal with the column in this outer loop as everything goes up in the column. 
    // Iterate through each col 
    for(col = 0; col < cur_game->cols; col++) {
        //This first section will deal with sliding everything upward.
        int insert_row = cur_game->rows -1; //Unlike move_w we start at the max and we will decrement, the core logic remains the same however hence you will see the code essentially identical.

        for(row = cur_game->rows -1; row >= 0; row--)  {   //For move_s, we well start from the max and decrement.
        cell *current_cell_ptr = get_cell(cur_game, row, col); //Create a new pointer to current cell using helper function get_cell
            if(*current_cell_ptr != -1) { // Check if empty
        
                if(insert_row != row) { //Check if it needs to be moved
        
                *get_cell(cur_game, insert_row, col) = *current_cell_ptr;  // Essentially moves tile to next spot
                 *current_cell_ptr = -1; // Set the cell to be empty 
                 move_occurred = 1; //A tile moved, so a move occurred.
                 }
            insert_row--; 
            }
         }
        
        // This next section will check to merge the tiles
        for(row = cur_game->rows -1; row > 0; row--) { // Start at max, we go to row > 0 since we need to merge and do not want to merge at 0.
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            cell *next_cell_ptr = get_cell(cur_game, row - 1, col); // We make these two pointers to get current and next cell so we can do merging
                if(*current_cell_ptr != -1 && *current_cell_ptr == *next_cell_ptr) { //Logically, if both pointers are not -1 and are equal, than it is a valid condition for us to merge them.
                    *current_cell_ptr *= 2; // Double top tile
                    cur_game->score += *current_cell_ptr; // Add value to score
                    *next_cell_ptr = -1; // Make the merged cell empty
                    move_occurred = 1; //It merged, so a move occurred.
            }
        }
        // This final section will reslide tiles after merging them.
        insert_row = cur_game->rows - 1; // Reset the counter for insert_row

        for(row = cur_game->rows -1; row >= 0; row--) { //This section utilized the same logic in our first section where we check if its empty and if the row is the same as our index, then we change it and set to empty.
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            if(*current_cell_ptr != -1) {
                if(insert_row != row) {
                    *get_cell(cur_game, insert_row, col) = *current_cell_ptr;
                    *current_cell_ptr = -1;
                }
                insert_row--;
            }
        }
    }
        return move_occurred; 
}


int move_a(game * cur_game) //slide left
{
    int move_occurred = 0; 
    int col, row;
    //We create our outer loop that deals with each column, because we are sliding upward, we only need to deal with the column in this outer loop as everything goes up in the row. 
    // Iterate through each row 
    for(row = 0; row < cur_game->rows; row++) {
        //This first section will deal with sliding everything upward.
        int insert_col = 0;

        for(col = 0; col < cur_game->cols; col++)  {   //Iterate through each row top to bottom
        cell *current_cell_ptr = get_cell(cur_game, row, col); //Create a new pointer to current cell using helper function get_cell
            if(*current_cell_ptr != -1) { // Check if empty
        
                if(insert_col != col) { //Check if it needs to be moved
        
                *get_cell(cur_game, row, insert_col) = *current_cell_ptr;  // Essentially moves tile to next spot
                 *current_cell_ptr = -1; // Set the cell to be empty 
                 move_occurred = 1; //A tile moved, so a move occurred.
                 }
            insert_col++; 
            }
         }
        
        // This next section will check to merge the tiles
        for(col = 0; col < cur_game->cols-1; col++) { //We stop at cols - 1 because we do not want to merge the last row with above since its empty
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            cell *next_cell_ptr = get_cell(cur_game, row, col+1); // We make these two pointers to get current and next cell so we can do merging
                if(*current_cell_ptr != -1 && *current_cell_ptr == *next_cell_ptr) { //Logically, if both pointers are not -1 and are equal, than it is a valid condition for us to merge them.
                    *current_cell_ptr *= 2; // Double top tile
                    cur_game->score += *current_cell_ptr; // Add value to score
                    *next_cell_ptr = -1; // Make the merged cell empty
                    move_occurred = 1; //It merged, so a move occurred.
            }
        }
        // This final section will reslide tiles after merging them.
        insert_col = 0; // Reset the counter for insert_col

        for(col = 0; col < cur_game->cols; col++) { //This section utilized the same logic in our first section where we check if its empty and if the row is the same as our index, then we change it and set to empty.
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            if(*current_cell_ptr != -1) {
                if(insert_col != col) {
                    *get_cell(cur_game, row, insert_col) = *current_cell_ptr;
                    *current_cell_ptr = -1;
                }
                insert_col++;
            }
        }
    }
        return move_occurred;    
}

int move_d(game * cur_game){ //slide to the right
    int move_occurred = 0; 
    int col, row;
    //We create our outer loop that deals with each column, because we are sliding right
    // Iterate through each row 
    for(row = 0; row < cur_game->rows; row++) {
        //This first section will deal with sliding everything upward.
        int insert_col = cur_game->cols-1;

        for(col = cur_game->cols -1; col >= 0; col--)  {   //Iterate through each row top to bottom
        cell *current_cell_ptr = get_cell(cur_game, row, col); //Create a new pointer to current cell using helper function get_cell
            if(*current_cell_ptr != -1) { // Check if empty
        
                if(insert_col != col) { //Check if it needs to be moved
        
                *get_cell(cur_game, row, insert_col) = *current_cell_ptr;  // Essentially moves tile to next spot
                 *current_cell_ptr = -1; // Set the cell to be empty 
                 move_occurred = 1; //A tile moved, so a move occurred.
                 }
            insert_col--; 
            }
         }
        
        // This next section will check to merge the tiles
        for(col = cur_game->cols -1; col > 0; col--) { //Similar logic to going downward and leftward, just now tweaked to go right.
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            cell *next_cell_ptr = get_cell(cur_game, row, col-1); // We make these two pointers to get current and next cell so we can do merging
                if(*current_cell_ptr != -1 && *current_cell_ptr == *next_cell_ptr) { //Logically, if both pointers are not -1 and are equal, than it is a valid condition for us to merge them.
                    *current_cell_ptr *= 2; // Double top tile
                    cur_game->score += *current_cell_ptr; // Add value to score
                    *next_cell_ptr = -1; // Make the merged cell empty
                    move_occurred = 1; //It merged, so a move occurred.
            }
        }
        // This final section will reslide tiles after merging them.
        insert_col = cur_game->cols-1; // Reset the counter for insert_col

        for(col = cur_game->cols -1; col >= 0; col--) { //This section utilized the same logic in our first section where we check if its empty and if the row is the same as our index, then we change it and set to empty.
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            if(*current_cell_ptr != -1) {
                if(insert_col != col) {
                    *get_cell(cur_game, row, insert_col) = *current_cell_ptr;
                    *current_cell_ptr = -1;
                }
                insert_col--;
            }
        }
    }
        return move_occurred;  
}

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //The strategy here is to have three seperate loops to check for three cases: empty spaces, adjacent merges, and vertical merges.
    int row, col; 
    
    //Check for empty cells
    for(row = 0; row < cur_game->rows; row++) {
        for(col = 0; col < cur_game->cols; col++) {
            if(*get_cell(cur_game, row, col) == -1) {
                return 1; // We return 1 if we see ANY cell that is empty. A valid move can thus be made
            }

        }
    }
    //If no empty cells, lets check for horizontal merges.
    for(row = 0; row < cur_game->rows; row++) {
        for(col = 0; col < cur_game->cols -1; col++) { 
            cell *current_cell_ptr = get_cell(cur_game, row, col);
            cell *next_cell_ptr = get_cell(cur_game, row, col+1);
            if(*current_cell_ptr == *next_cell_ptr && *current_cell_ptr != -1) //If not both empty and both have same value, legal move.
            { return 1; } // A legal move exists because two tiles can be merged since equal 
        } 
    }
    //Check for vertical merges
    for(col = 0; col < cur_game->cols; col++) {
        for(row = 0; row < cur_game->rows -1; row++) { 
            cell *current_cell_ptr = get_cell(cur_game, row, col); //Initialize pointer to current and next cell
            cell *next_cell_ptr = get_cell(cur_game, row+1, col);
            if(*current_cell_ptr == *next_cell_ptr && *current_cell_ptr != -1) //If not both empty and both have same value, legal move.
            { return 1; } // A legal move exists because two tiles can be merged since equal 
        } 
    }

    //If we get here, then it means no legal moves.
    return 0;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}

