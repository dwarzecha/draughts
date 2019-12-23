#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
TODO
- fill men function 
- print men function
- choosing game type?
- [FIX] why initial position of selector + 1 is the same as +2?
        (the board starts printing at 0)
- [CODE] change everything to bool
- [CODE] systematize iterator names
- [CODE] check_for_errors non universal, but we'll see close to the end of project
- [CODE] separate headers file?
*/

#define TRUE 1
#define FALSE 0

/*
#ifdef _WIN32
    #define clear_screen system("cls")
#elif __APPLE__
    // printf ("\33c\e[3J");
    #define clear_screen system("clear") // fake, only prints a bunch of \n
#endif
*/

typedef struct game_parameters
{
    int board_dimensions;
    int number_of_pieces;
    int number_of_games;
}game_parameters;

typedef struct sizeof_board
{
    // square dimensions

    int square_width;
    int square_height;

    // distance from the screen border

    int border_x;
    int border_y;

    // size

    int rows;
    int columns;
}sizeof_board;

typedef struct selector
{
    // position
    // i - vertical axis, j - horizontal axis

    int i;
    int j;

    //dimensions

    int width;
    int height;
}selector;

typedef struct piece
{
	int alive;
	int colour;
	int type;

	int pos_x;
	int pos_y;
}piece;

void fill_board(sizeof_board b1, char board[b1.rows][b1.columns]);
void print_board(sizeof_board b1, char board[b1.rows][b1.columns]);
void move_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1);
void set_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector s1);
void clear_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1);
void fill_men(sizeof_board b1, char board[b1.rows][b1.columns], game_parameters g1, piece p1[g1.number_of_pieces]);
void print_men(sizeof_board b1, char board[b1.rows][b1.columns], game_parameters g1, piece p1[g1.number_of_pieces]);
// void ask_for_dimensions(game_parameters *g1); // just for debugging properties
void check_for_errors(game_parameters g1);

int main()
{
	// initial parameters

    game_parameters g1 = {10, 30, 1};
    check_for_errors(g1); // check if g1.number_of_pieces won't be too big for this g1.board_dimensions
    // ask_for_dimensions(&g1);
    int end_of_game = FALSE; // variable for end of the game


    // defining game objects

    sizeof_board b1 = {10, 5, 6, 3, b1.square_height*g1.board_dimensions+b1.border_y+1, b1.square_width*g1.board_dimensions+b1.border_x+1}; // define board size with default dimensions 10x10
    selector s1 = {b1.border_y+1, b1.border_x+1}; // define selector starting position
    char board[b1.rows][b1.columns]; // create an array for board
    piece p1[g1.number_of_pieces]; // create an array of structures for pieces 


    // filling game objects with initial content

    fill_board(b1, board); // fill board array with graphic symbols
    fill_men(b1, board, g1, p1);


    // gameplay

    while(!end_of_game)
    {
        printf("\33c\e[3J"); // clear the console, replace with clear_screen; below if necessary
        //clear_screen; // use if running in standard windows terminals, unsafe (system())
        set_selector(b1, board, s1); // input the selector symbols into the board array
        print_board(b1, board); // print the board array
        printf("%d %d", s1.i, s1.j); // debugging line
        for (int i=0; i<g1.number_of_pieces/2; i++)
        {
        	printf("\n%d\n%d\n%d\n%d\n%d\n", p1[i].alive, p1[i].colour, p1[i].type, p1[i].pos_x, p1[i].pos_y); // debugging line
        }
        move_selector(b1, board, &s1); // change selector position
    }
    return 0;
}

void fill_board(sizeof_board b1, char board[b1.rows][b1.columns])
{
    // fill the board

	// fill the first b1.border_y rows with spaces
    for(int i=0; i<b1.border_y; i++)
    {
    	for(int j=0; j<b1.columns; j++)
    	{
    		board[i][j] = ' ';
    	}
    }

    for(int i=b1.border_y; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y
    {
    	for(int j=0; j<b1.border_x; j++)
    	{
    		board[i][j] = ' '; // fill the first b1.border_x cells with spaces
    	}

        for(int j=b1.border_x; j<b1.columns; j++) // fill the row with dashes
        {
            board[i][j] = '-';
        }
    }

    for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y
    {
        for(int k=i; k<i+b1.square_height-1; k++) // go b1.square_height-1 rows at a time
        {
        	for(int j=0; j<b1.border_x; j++)
    		{
    			board[k][j] = ' '; // fill the first b1.border_x cells with spaces
    		}

            for(int j=b1.border_x; j<b1.columns; j+=b1.square_width) // fill every b1.square_width cell starting from b1.border_x with vertical line
            {
                board[k][j] = '|';
            }
        }
    }

    for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y+1
    {
        for(int l=i; l<i+b1.square_height-1; l++) // go b1.square_height-1 rows at a time
        {
			for(int k=0; k<b1.border_x; k++)
    		{
    			board[l][k] = ' '; // fill the first b1.border_x cells with spaces
    		}

            for(int j=b1.border_x+1; j<b1.columns; j+=b1.square_width) // set j on every b1.square_width cell starting from b1.border_x+1
            {          
                for(int k=j; k<j+b1.square_width-1; k++) // fill b1.square_width-1 consecutive cells with space
                {
                    board[l][k] = ' ';
                }
            }
        }
    }

}

void print_board(sizeof_board b1, char board[b1.rows][b1.columns])
{
    // print the board

    for(int i=0; i<b1.rows; i++)
    {
        for(int j=0; j<b1.columns; j++)
        {
            printf("%c", board[i][j]);
        }

        printf("\n");
    }

}

void move_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1)
{
    char key = 0; // buffer for key pressed

    while(1) // always until break
    {
        key = getchar(); // get the ASCII value of the key pressed

        if(key==100) // if d was pressed
        {
            if(s1->j == b1.columns - b1.square_width) // doing nothing if the selector is on the right border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->j = s1->j + b1.square_width; // move b1.square_height spaces to the right
                break;
            }
        }
        else if(key == 97) // if a was pressed
        {
            if(s1->j == b1.border_x+1) // doing nothing if the selector is on the left border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->j = s1->j - b1.square_width; // move b1.square_height spaces to the left
                break;
            }
        }
        else if(key == 119) // if w was pressed
        {
            if(s1->i == b1.border_y+1) // doing nothing if the selector is on the upper border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->i = s1->i - b1.square_height; // move b1.square_height spaces up
                break;
            }
        }
        else if(key == 115) // if s was pressed
        {
            if(s1->i == b1.rows - b1.square_height) // doing nothing if the selector is on the lower border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->i = s1->i + b1.square_height; // move b1.square_height spaces down
                break;
            }
        }
        else if(key == 'x') // debugging line
        {
        	printf("\33c\e[3J"); // clear the console
        	exit(0);
        }
    }
}

void set_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector s1)
{
    board[s1.i][s1.j] = '|'; // set upper left corner
    board[s1.i][s1.j+b1.square_width-2] = '|'; // set upper right corner b1.square_width-2 spaces to the right
    board[s1.i+b1.square_height-2][s1.j] = '|'; // set bottom left corner b1.square_height-2 spaces down
    board[s1.i+b1.square_height-2][s1.j+b1.square_width-2] = '|'; // set bottom right corner b1.square_width-2 spaces to the right, b1.square_height-2 spaces down


    // man graphics test commands

    // white man
    /*
    board[s1.i][s1.j+1] = 201;
    board[s1.i][s1.j+b1.square_width-3] = 187;
    board[s1.i+b1.square_height-2][s1.j+1] = 200;
    board[s1.i+b1.square_height-2][s1.j+2] = 188;
    */

    // black man
    /*
    board[s1.i][s1.j+1] = 218;
    board[s1.i][s1.j+b1.square_width-3] = 191;
    board[s1.i+b1.square_height-2][s1.j+1] = 192;
    board[s1.i+b1.square_height-2][s1.j+2] = 217;
    */
}

void clear_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1)
{
    board[s1->i][s1->j] = ' '; // clear upper left corner
    board[s1->i][s1->j+b1.square_width-2] = ' '; // clear upper right corner x spaces to the right (now x = 3)
    board[s1->i+b1.square_height-2][s1->j] = ' '; // clear bottom left corner x spaces down (now x = 1)
    board[s1->i+b1.square_height-2][s1->j+b1.square_width-2] = ' '; // clear bottom right corner x spaces to the right, y spaces down (now  x = 3, y = 1)


    // man graphics test commands

    /*
    board[s1->i][s1->j+1] = ' ';
    board[s1->i][s1->j+b1.square_width-3] = ' ';
    board[s1->i+b1.square_height-2][s1->j+1] = ' ';
    board[s1->i+b1.square_height-2][s1->j+b1.square_width-3] = ' ';
    */
}

void fill_men(sizeof_board b1, char board[b1.rows][b1.columns], game_parameters g1, piece p1[g1.number_of_pieces])
{
	int k = 0;

	for(int i=0; i<g1.number_of_pieces/2; i++) // define the starting parameters for first 15 pieces (black)
	{
		p1[i].alive = 1;
		p1[i].colour = 0;
		p1[i].type = 0;
	}

	/*
	while(k<g1.number_of_pieces/2)
	{
		for(int i=0; i<g1.board_dimensions; i++) // go as many times as there are rows
		{
			for(int j=0; j<g1.board_dimensions; j++) // go as many times as there are columns
			{
				p1[k+j].pos_y = b1.border_y+1+i*b1.square_height;
				p1[k+j].pos_x = b1.border_x+1+j*b1.square_width;
				k += j;
			}
		}
	}
	*/
}

void print_men(sizeof_board b1, char board[b1.rows][b1.columns], game_parameters g1, piece p1[g1.number_of_pieces])
{
    for(int i=b1.border_y+1; i<b1.rows; i=i+b1.square_height) // every b1.square_height row starting from b1.border_y+1
    {
        //for(int l=i; l<i+b1.square_height-1; l++) // go b1.square_height-1 rows at a time
        //{
            for(int k=b1.border_x+1; k<b1.columns; k = k+b1.square_width) // set j on every b1.square_width cell starting on b1.border_x+1
            {

                /*      
                for(int k=j; k<j+b1.square_width-1; k++) // fill b1.square_width-1 consecutive cells with space
                {
                    
                }
                */
          	}
      	//}
    }

    /*
    board[s1.i][s1.j] = '|'; // set upper left corner
    board[s1.i][s1.j+b1.square_width-2] = '|'; // set upper right corner b1.square_width-2 spaces to the right
    board[s1.i+b1.square_height-2][s1.j] = '|'; // set bottom left corner b1.square_height-2 spaces down
    board[s1.i+b1.square_height-2][s1.j+b1.square_width-2] = '|'; // set bottom right corner b1.square_width-2 spaces to the right, b1.square_height-2 spaces down
    */
}


/*
void ask_for_dimensions(game_parameters *g1)
{
    int f = FALSE;
    while(!f)
    {
        printf("What size of the board would you like to print? ");
        scanf("%d", &g1->board_dimensions);
        if(g1->board_dimensions<=0)
        {
            printf("\nInvalid value! Try once again\n\n");
            f = FALSE;
        }
        else
        {
            f = TRUE;
        }
   }
}
*/

void check_for_errors(game_parameters g1)
{
	if(g1.number_of_pieces>(g1.board_dimensions*g1.board_dimensions)/2)
    {
    	printf("ERROR: Too big number_of_pieces for this board_dimensions, please change variable\nThe program will now terminate.\nPress [Enter] to continue...");
    	getchar();
    	exit(0);
    }
    else if(g1.number_of_pieces<0)
    {
    	printf("ERROR: Too small number_of_pieces for this board_dimensions, please change variable.\nThe program will now terminate.\nPress [Enter] to continue...");
    	getchar();
    	exit(0);
    }
}