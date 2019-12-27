#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <math.h>

/*
#ifdef WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>

int getch (void)
{
    int key;
    struct termios oldSettings, newSettings;    // stuktury z ustawieniami terminala

    tcgetattr(STDIN_FILENO, &oldSettings);    // pobranie ustawień terminala
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);    // ustawienie odpowiednich flag
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);    // zastosowanie ustawień
    key = getchar();    // pobranie znaku ze standardowego wejścia
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);    // przywrócenie poprzednich ustawień terminala
    return key;
}
#endif
*/

/*
TODO

- do I want to use getch?
- make black men appear on top when moving (why append_checker inside fill_pieces not working?)
- merge conditions for move?

- black men move conditions not working
- capturing other men
- king mechanics (move possibilities, when to become king)

- changing turns
- menu (start game, instructions, exit)
- konami code
- conditions for end_of_game = true
- return to menu
- start new game option (malloc?)

- [CODE] make controls operate on switch ?
- [CODE] connect ifs ?
- [CODE] make men a 2D array ?

- [CODE] check_for_errors non universal, but we'll see close to the end of project (also prepare ask_for_dimensions)

- [CODE] sort the passed variables in function headers
*/

typedef struct game_parameters
{
	bool end_of_game;
    int board_dimensions;
    int number_of_pieces;
    int number_of_games;
    int game_status_indicator;
}game_parameters;

typedef struct sizeof_board
{
    int square_width;
    int square_height;

    int border_x;
    int border_y;

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
	bool alive;
	bool white;
	bool king;
	bool if_append;

	int pos_x;
	int pos_y;

	int previous_pos_x;
	int previous_pos_y;
}piece;

typedef struct square
{
	// coordinates

	int x;
	int y;

	// attributes

	bool can_move;
	bool empty;
}square;

// assigning initial attributes
void assign_square_attributes(game_parameters g1, sizeof_board b1, square square_attributes[g1.board_dimensions][g1.board_dimensions]);
void create_men(sizeof_board b1, game_parameters g1, piece p1[g1.number_of_pieces]);

// filling the board array
void fill_board(sizeof_board b1, char board[b1.rows][b1.columns]);
void fill_pieces(game_parameters g1, sizeof_board b1, char board[b1.rows][b1.columns], piece p1[g1.number_of_pieces]);
void set_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector s1);
void clear_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1, game_parameters g1, piece p1[g1.number_of_pieces]); // organize variables
void fill_move_possibility(game_parameters g1, square square_attributes[g1.board_dimensions][g1.board_dimensions], sizeof_board b1, char board[b1.rows][b1.columns], piece p1[g1.number_of_pieces]); // organize variables

// printing stuff
void print_board(sizeof_board b1, char board[b1.rows][b1.columns]);
void print_info(sizeof_board b1, game_parameters *g1, selector s1, piece p1[g1->number_of_pieces], square square_attributes[g1->board_dimensions][g1->board_dimensions]); // organize variables

// mechanics
void controls(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1, game_parameters *g1, piece p1[g1->number_of_pieces], square square_attributes[g1->board_dimensions][g1->board_dimensions]); // organize variables
void append(sizeof_board b1, selector *s1, game_parameters *g1, piece p1[g1->number_of_pieces], square square_attributes[g1->board_dimensions][g1->board_dimensions]); // functionality connected with appending/unappending a piece to the selector and moving the piece // organize variables

// useful functions
int append_check(game_parameters g1, piece p1[g1.number_of_pieces]); // check if any piece is appended to the selector
void check_men_position(game_parameters g1, piece p1[g1.number_of_pieces], square square_attributes[g1.board_dimensions][g1.board_dimensions]); // organize variables

// void ask_for_dimensions(game_parameters *g1); // for Konami Code // ask the users for the board dimensions
// void ask_for_number_of_pieces() // will be necessary
// void check_for_errors(game_parameters g1); // for Konami Code // check if the number of pieces isnt't too big for this board dimensions

int main()
{
	// initial parameters
    game_parameters g1 = {false, 10, 40, 1, 0};
    // check_for_errors(g1); // check if g1.number_of_pieces won't be too big for this g1.board_dimensions
    // ask_for_dimensions(&g1);

    // defining game objects
    sizeof_board b1 = {10, 5, 0, 0, b1.square_height*g1.board_dimensions+b1.border_y+1, b1.square_width*g1.board_dimensions+b1.border_x+1};
    square square_attributes[g1.board_dimensions][g1.board_dimensions];
    assign_square_attributes(g1, b1, square_attributes);
    selector s1 = {b1.border_y+1, b1.border_x+1};
    char board[b1.rows][b1.columns];
    piece p1[g1.number_of_pieces];
    create_men(b1, g1, p1);

    // filling game objects with initial content
    fill_board(b1, board);

    // gameplay
    while(!g1.end_of_game)
    {
        printf("\33c\e[3J");
        check_men_position(g1, p1, square_attributes);
        fill_move_possibility(g1, square_attributes, b1, board, p1);
        fill_pieces(g1, b1, board, p1);
        set_selector(b1, board, s1);
        print_board(b1, board);
        print_info(b1, &g1, s1, p1, square_attributes);
        controls(b1, board, &s1, &g1, p1, square_attributes);
    }
    return 0;
}

// assigning initial attributes

void assign_square_attributes(game_parameters g1, sizeof_board b1, square square_attributes[g1.board_dimensions][g1.board_dimensions])
{
	for(int i=0; i<g1.board_dimensions; i++)
	{
		for(int j=0; j<g1.board_dimensions; j++)
		{
			square_attributes[i][j].x = b1.border_x+1+j*b1.square_width;
			square_attributes[i][j].y = b1.border_y+1+i*b1.square_height;
			square_attributes[i][j].can_move = false;
		}
	}
}

void create_men(sizeof_board b1, game_parameters g1, piece p1[g1.number_of_pieces])
{
	for(int i=0; i<g1.number_of_pieces; i++)
	{
		p1[i].alive = true;
		p1[i].king = false;
		p1[i].if_append = false;

		if(i<g1.number_of_pieces/2)
		{
			p1[i].white = false;
		}
		else
		{
			p1[i].white = true; 
		}
	}
	
	for(int i=0; i<2*g1.number_of_pieces/g1.board_dimensions; i++)
	{
		for(int j=0; j<g1.board_dimensions/2; j++)
		{
			if(p1[j+g1.board_dimensions/2*i].white == false)
			{
				p1[j+g1.board_dimensions/2*i].pos_x = b1.border_x+1+(j*2+!(i%2))*b1.square_width;
				p1[j+g1.board_dimensions/2*i].pos_y = b1.border_y+1+i*b1.square_height;
			}
			else
			{
				p1[j+g1.board_dimensions/2*i].pos_x = b1.columns-b1.square_width-(j*2+!(i%2))*b1.square_width;
				p1[j+g1.board_dimensions/2*i].pos_y = b1.rows-b1.square_height-(i-g1.number_of_pieces/g1.board_dimensions)*b1.square_height;
			}

			p1[j+g1.board_dimensions/2*i].previous_pos_x = p1[j+g1.board_dimensions/2*i].pos_x;
			p1[j+g1.board_dimensions/2*i].previous_pos_y = p1[j+g1.board_dimensions/2*i].pos_y;
		}	
	}
}


// filling the board array

void fill_board(sizeof_board b1, char board[b1.rows][b1.columns])
{
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
        for(int j=i; j<i+b1.square_height-1; j++) // go b1.square_height-1 rows at a time
        {
        	for(int k=0; k<b1.border_x; k++)
    		{
    			board[j][k] = ' '; // fill the first b1.border_x cells with spaces
    		}

            for(int k=b1.border_x; k<b1.columns; k+=b1.square_width) // fill every b1.square_width cell starting from b1.border_x with vertical line
            {
                board[j][k] = '|';
            }
        }
    }

    for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y+1
    {
        for(int j=i; j<i+b1.square_height-1; j++) // go b1.square_height-1 rows at a time
        {
			for(int k=0; k<b1.border_x; k++)
    		{
    			board[j][k] = ' '; // fill the first b1.border_x cells with spaces
    		}

            for(int k=b1.border_x+1; k<b1.columns; k+=b1.square_width) // set j on every b1.square_width cell starting from b1.border_x+1
            {          
                for(int l=k; l<k+b1.square_width-1; l++) // fill b1.square_width-1 consecutive cells with space
                {
                    board[j][l] = ' ';
                }
            }
        }
    }
}

void fill_pieces(game_parameters g1, sizeof_board b1, char board[b1.rows][b1.columns], piece p1[g1.number_of_pieces])
{
	int append_checker = append_check(g1, p1);

	for(int i=0; i<g1.number_of_pieces; i++)
	{
		if(p1[i].alive == true)
		{
			if(p1[i].white == true)
			{
				if(p1[i].king == true)
				{
					for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
							board[p1[i].pos_y+j][p1[i].pos_x+k] = 'O';

							board[p1[i].pos_y+j][p1[i].pos_x] = ' ';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-2] = ' ';

							board[p1[i].pos_y+j][p1[i].pos_x+1] = ' ';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-3] = ' ';

							board[p1[i].pos_y+j][p1[i].pos_x+2] = ' ';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-4] = ' ';
        				}
        			}

        			for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
        					board[p1[i].pos_y][p1[i].pos_x+k] = '|';

        					board[p1[i].pos_y][p1[i].pos_x] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-2] = ' ';

							board[p1[i].pos_y][p1[i].pos_x+1] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-3] = ' ';
        				}
        			}

        			for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+k] ='-';
							
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x] = ' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-2] = ' ';

							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+1] = ' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-3] = ' ';
        				}
        			}
        		}
				else
				{
					for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
							board[p1[i].pos_y+j][p1[i].pos_x+k] = 'O';

							board[p1[i].pos_y+j][p1[i].pos_x] = ' ';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-2] = ' ';

							board[p1[i].pos_y][p1[i].pos_x+1] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-3] = ' ';
							
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+1] =' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-3] =' ';
						}
					}
				}
			}
			else
			{
				if(p1[i].king == true)
				{
					for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
							board[p1[i].pos_y+j][p1[i].pos_x+3] = 'O';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-5] = 'O';

        					board[p1[i].pos_y][p1[i].pos_x+k] = '|';

        					board[p1[i].pos_y][p1[i].pos_x] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-2] = ' ';

							board[p1[i].pos_y][p1[i].pos_x+1] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-3] = ' ';
        				}
        			}

        			for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+k] ='-';
							
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x] = ' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-2] = ' ';

							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+1] = ' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-3] = ' ';
        				}
        			}
				}
				else
				{
					for(int j=0; j<b1.square_height-1; j++)
					{
						for(int k=0; k<b1.square_width-1; k++)
						{
							board[p1[i].pos_y][p1[i].pos_x+k] = 'O';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+k] ='O';

							board[p1[i].pos_y+j][p1[i].pos_x+1] = 'O';
							board[p1[i].pos_y+j][p1[i].pos_x+2] = 'O';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-3] ='O';
							board[p1[i].pos_y+j][p1[i].pos_x+b1.square_width-4] ='O';

							board[p1[i].pos_y][p1[i].pos_x] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+1] = ' ';

							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-2] = ' ';
							board[p1[i].pos_y][p1[i].pos_x+b1.square_width-3] = ' ';

							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x] = ' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-2] = ' ';
							
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+1] =' ';
							board[p1[i].pos_y+b1.square_height-2][p1[i].pos_x+b1.square_width-3] =' ';
						}
					}
				}
			}

			if(append_checker != false)
			{
				if(p1[append_checker].white == true)
				{
					if(p1[append_checker].king == true)
					{
						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+k] = 'O';

								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-2] = ' ';

								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+1] = ' ';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-3] = ' ';

								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+2] = ' ';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-4] = ' ';
							}
						}

						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+k] = '|';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-2] = ' ';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x+1] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-3] = ' ';
							}
						}

						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+k] ='-';
								
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-2] = ' ';

								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+1] = ' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-3] = ' ';
							}
						}
					}
					else
					{
						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+k] = 'O';

								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-2] = ' ';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x+1] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-3] = ' ';
								
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+1] =' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-3] =' ';
							}
						}
					}
				}
				else
				{
					if(p1[append_checker].king == true)
					{
						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+3] = 'O';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-5] = 'O';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x+k] = '|';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-2] = ' ';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x+1] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-3] = ' ';
							}
						}

						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+k] ='-';
								
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-2] = ' ';

								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+1] = ' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-3] = ' ';
							}
						}
					}
					else
					{
						for(int j=0; j<b1.square_height-1; j++)
						{
							for(int k=0; k<b1.square_width-1; k++)
							{
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+k] = 'O';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+k] ='O';

								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+1] = 'O';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+2] = 'O';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-3] ='O';
								board[p1[append_checker].pos_y+j][p1[append_checker].pos_x+b1.square_width-4] ='O';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+1] = ' ';

								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-2] = ' ';
								board[p1[append_checker].pos_y][p1[append_checker].pos_x+b1.square_width-3] = ' ';

								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x] = ' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-2] = ' ';
								
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+1] =' ';
								board[p1[append_checker].pos_y+b1.square_height-2][p1[append_checker].pos_x+b1.square_width-3] =' ';
							}
						}
					}
				}
			}
		}
	}
}

void set_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector s1)
{
    board[s1.i][s1.j] = '|';
    board[s1.i][s1.j+b1.square_width-2] = '|';
    board[s1.i+b1.square_height-2][s1.j] = '|';
    board[s1.i+b1.square_height-2][s1.j+b1.square_width-2] = '|';
}

void clear_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1, game_parameters g1, piece p1[g1.number_of_pieces])
{
    board[s1->i][s1->j] = ' ';
    board[s1->i][s1->j+b1.square_width-2] = ' ';
    board[s1->i+b1.square_height-2][s1->j] = ' ';
    board[s1->i+b1.square_height-2][s1->j+b1.square_width-2] = ' ';

    for(int i=0; i<g1.number_of_pieces; i++)
	{
		if(p1[i].pos_x == s1->j && p1[i].pos_y == s1->i)
		{
			if(p1[i].if_append == true)
			{
		        for(int j=p1[i].pos_y; j<p1[i].pos_y+b1.square_height-1; j++) // go b1.square_height-1 rows at a time
		        {
	                for(int k=p1[i].pos_x; k<p1[i].pos_x+b1.square_width-1; k++) // fill b1.square_width-1 consecutive cells with space
	                {
	                    board[j][k] = ' ';
	                }
		        }
			    
			}
		}
	}
}

void fill_move_possibility(game_parameters g1, square square_attributes[g1.board_dimensions][g1.board_dimensions], sizeof_board b1, char board[b1.rows][b1.columns], piece p1[g1.number_of_pieces])
{
	bool append_checker = append_check(g1, p1);

	for(int i=0; i<g1.board_dimensions; i++)
	{
		for(int j=0; j<g1.board_dimensions; j++)
		{
			if(square_attributes[i][j].can_move == true && append_checker == true)
			{
				if(square_attributes[i][j].empty == true)
				{
					for (int k=square_attributes[i][j].y; k<square_attributes[i][j].y+b1.square_height-1; k++)
					{
						board[k][square_attributes[i][j].x] = '|';
				    	board[k][square_attributes[i][j].x+b1.square_width-2] = '|';
					}
				}
			}
			else
			{
				for (int k=square_attributes[i][j].y; k<square_attributes[i][j].y+b1.square_height-1; k++)
				{
					for (int l=square_attributes[i][j].x; l<square_attributes[i][j].x+b1.square_width-1; l++)
					{
						board[k][l] = ' ';
					}
				}
			}	
		}
	}
}


// printing stuff

void print_board(sizeof_board b1, char board[b1.rows][b1.columns])
{
    for(int i=0; i<b1.rows; i++)
    {
        for(int j=0; j<b1.columns; j++)
        {
            printf("%c", board[i][j]);
        }

        printf("\n");
    }
}

void print_info(sizeof_board b1, game_parameters *g1, selector s1, piece p1[g1->number_of_pieces], square square_attributes[g1->board_dimensions][g1->board_dimensions])
{
	printf("\n%d %d\n", s1.i, s1.j); // debugging line
	
	for(int i=0; i<g1->board_dimensions; i++)
	{
		for (int j=0; j<g1->board_dimensions; j++)
		{
			printf("%d ", square_attributes[i][j].can_move);
		}
		printf("    ");

		for (int j=0; j<g1->board_dimensions; j++)
		{
			printf("%d ", square_attributes[i][j].empty);
		}
		printf("\n");
	}

   	/*
    for (int i=0; i<g1->number_of_pieces; i++)
    {
    	printf("a:%d c:%d x:%d y:%d, p_x:%d, p_y:%d\n", p1[i].if_append, p1[i].white, p1[i].pos_x, p1[i].pos_y, p1[i].previous_pos_x, p1[i].previous_pos_y);
    }
	*/

	/*
	for(int i=0; i<g1->board_dimensions; i++)
	{
		for (int j=0; j<g1->board_dimensions; j++)
		{
			printf("x:%d y:%d %d %d\n", square_attributes[i][j].x, square_attributes[i][j].y, i, j);
		}
		printf("\n");
	}
	*/
    switch(g1->game_status_indicator)
	{
		case 0:
			printf("\nGood luck!\n");
			break;
		case 1:
			printf("\nThis square is taken!\n");
			g1->game_status_indicator = 0;
			break;
		case 2:
			printf("\nCan't touch this!\n");
			g1->game_status_indicator = 0;
			break;
		default:
			printf("\nGood luck!\n");
			break;
	}
}


// mechanics

void controls(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1, game_parameters *g1, piece p1[g1->number_of_pieces], square square_attributes[g1->board_dimensions][g1->board_dimensions])
{
    char key = 0;

    while(1)
    {
        key = getchar();

        if(key == 'd')
        {
            if(s1->j == b1.columns - b1.square_width) // doing nothing if the selector is on the right border
            {

            }
            else
            {
                clear_selector(b1, board, s1, *g1, p1);

                for(int i=0; i<g1->number_of_pieces; i++)
				{
					if(p1[i].pos_x == s1->j && p1[i].pos_y == s1->i)
					{
						if(p1[i].if_append == true)
						{
							p1[i].pos_x += b1.square_width;
							break;
						}
					}
				}

                s1->j += b1.square_width;
                break;
            }
        }
        else if(key == 'a')
        {
            if(s1->j == b1.border_x+1) // doing nothing if the selector is on the left border
            {

            }
            else
            {
                clear_selector(b1, board, s1, *g1, p1);

                for(int i=0; i<g1->number_of_pieces; i++)
				{
					if(p1[i].pos_x == s1->j && p1[i].pos_y == s1->i)
					{
						if(p1[i].if_append == true)
						{
							p1[i].pos_x -= b1.square_width;
							break;
						}
					}
				}

                s1->j -= b1.square_width;
                break;
            }
        }
        else if(key == 'w')
        {
            if(s1->i == b1.border_y+1) // doing nothing if the selector is on the upper border
            {

            }
            else
            {
                clear_selector(b1, board, s1, *g1, p1);

                for(int i=0; i<g1->number_of_pieces; i++)
				{
					if(p1[i].pos_x == s1->j && p1[i].pos_y == s1->i)
					{
						if(p1[i].if_append == true)
						{
							p1[i].pos_y -= b1.square_height;
							break;
						}
					}
				}

                s1->i -= b1.square_height;
                break;
            }
        }
        else if(key == 's')
        {
            if(s1->i == b1.rows - b1.square_height) // doing nothing if the selector is on the lower border
            {

            }
            else
            {
                clear_selector(b1, board, s1, *g1, p1);

                for(int i=0; i<g1->number_of_pieces; i++)
				{
					if(p1[i].pos_x == s1->j && p1[i].pos_y == s1->i)
					{
						if(p1[i].if_append == true)
						{
							p1[i].pos_y += b1.square_height;
							break;
						}
					}
				}

                s1->i += b1.square_height;
                break;
            }
        }
        else if(key == 'e')
		{
			append(b1, s1, g1, p1, square_attributes);
			break;
		}
        else if(key == 'x')
        {
        	printf("\33c\e[3J");
        	exit(0);
        }
    }
}

void append(sizeof_board b1, selector *s1, game_parameters *g1, piece p1[g1->number_of_pieces], square square_attributes[g1->board_dimensions][g1->board_dimensions]) // functionality connected with appending/unappending a piece to the selector and moving the piece
{
	bool append_checker = append_check(*g1, p1);
	bool other_under_appended = false;
	bool can_not_move = false;

	for(int i=0; i<g1->number_of_pieces; i++)
	{
		if(p1[i].alive == true)
		{
			if(p1[i].pos_x == s1->j && p1[i].pos_y == s1->i)
			{
				if(append_checker == false)
				{
					p1[i].previous_pos_y = p1[i].pos_y;
					p1[i].previous_pos_x = p1[i].pos_x;
					p1[i].if_append = true; // picking the piece up

					for(int j=0; j<g1->board_dimensions; j++) // move conditions for various piece types
					{
						for(int k=0; k<g1->board_dimensions; k++)
						{
							if (p1[i].king == true)
							{
								if(p1[i].white == true)
								{
									if(square_attributes[j][k].x == p1[i].pos_x + b1.square_width && square_attributes[j][k].y == p1[i].pos_y - b1.square_height)
									{
										square_attributes[j][k].can_move = true;
									}
									else if(square_attributes[j][k].x == p1[i].pos_x - b1.square_width && square_attributes[j][k].y == p1[i].pos_y - b1.square_height)
									{
										square_attributes[j][k].can_move = true;
									}
									else if(square_attributes[j][k].x == p1[i].previous_pos_x && square_attributes[j][k].y == p1[i].previous_pos_y)
									{
										square_attributes[j][k].can_move = true;
									}
									else
									{
										square_attributes[j][k].can_move = false;
									}
								}
								else
								{
									if(square_attributes[j][k].x == p1[i].pos_x + b1.square_width && square_attributes[j][k].y == p1[i].pos_y + b1.square_height)
									{
										square_attributes[j][k].can_move = true;
									}
									else if(square_attributes[j][k].x == p1[i].pos_x - b1.square_width && square_attributes[j][k].y == p1[i].pos_y + b1.square_height)
									{
										square_attributes[j][k].can_move = true;
									}
									else if(square_attributes[j][k].x == p1[i].previous_pos_x && square_attributes[j][k].y == p1[i].previous_pos_y)
									{
										square_attributes[j][k].can_move = true;
									}
									else
									{
										square_attributes[j][k].can_move = false;
									}
								}
							}
							else
							{
								if(p1[i].white == true)
								{
									if(square_attributes[j][k].x == p1[i].pos_x + b1.square_width && square_attributes[j][k].y == p1[i].pos_y - b1.square_height)
									{
										if(square_attributes[j][k].empty == true)
										{
											square_attributes[j][k].can_move = true;
										}
										else
										{
											if(j>0 && k<g1->board_dimensions-1)
											{
												square_attributes[j-1][k+1].can_move = true;
											}
										}

									}
									else if(square_attributes[j][k].x == p1[i].pos_x - b1.square_width && square_attributes[j][k].y == p1[i].pos_y - b1.square_height)
									{
										if(square_attributes[j][k].empty == true)
										{
											square_attributes[j][k].can_move = true;
										}
										else
										{
											if(j>0 && k>0)
											{
												square_attributes[j-1][k-1].can_move = true;
											}
										}
									}
									else if(square_attributes[j][k].x == p1[i].previous_pos_x && square_attributes[j][k].y == p1[i].previous_pos_y)
									{
										square_attributes[j][k].can_move = true;
									}
									else
									{
										square_attributes[j][k].can_move = false;
									}	
								}
								else
								{
									if(square_attributes[j][k].x == p1[i].pos_x + b1.square_width && square_attributes[j][k].y == p1[i].pos_y + b1.square_height)
									{
										if(square_attributes[j][k].empty == true)
										{
											square_attributes[j][k].can_move = true;
										}
										else
										{
											if(j<g1->board_dimensions-1 && k<g1->board_dimensions-1)
											{
												square_attributes[j+1][k+1].can_move = true;
											}
										}

									}
									else if(square_attributes[j][k].x == p1[i].pos_x - b1.square_width && square_attributes[j][k].y == p1[i].pos_y + b1.square_height)
									{
										if(square_attributes[j][k].empty == true)
										{
											square_attributes[j][k].can_move = true;
										}
										else
										{
											if(j<g1->board_dimensions-1 && k>0)
											{
												square_attributes[j+1][k-1].can_move = true;
											}
										}
									}
									else if(square_attributes[j][k].x == p1[i].previous_pos_x && square_attributes[j][k].y == p1[i].previous_pos_y)
									{
										square_attributes[j][k].can_move = true;
									}
									else
									{
										square_attributes[j][k].can_move = false;
									}
								}
							}
						}
					}
				}
				else // checking if putting the piece back is possible
				{
					for(int j=0; j<g1->number_of_pieces; j++)
					{
						for(int k=0; k<g1->number_of_pieces; k++)
						{
							if(square_attributes[j][k].x == s1->j && square_attributes[j][k].y == s1->i && square_attributes[j][k].can_move == false)
							{
								can_not_move = true;
								break;
							}
						}
					}

					for(int j=0; j<g1->number_of_pieces; j++)
					{
						if(p1[j].pos_x == s1->j && p1[j].pos_y == s1->i && p1[j].alive == true && p1[j].if_append == false)
						{
							other_under_appended = true;
							break;
						}
					}
					
					// final checking for conditions
					if(other_under_appended == true)
					{
						g1->game_status_indicator = 1;
					}
					else if(can_not_move == true)
					{
						g1->game_status_indicator = 2;
					}
					else
					{
						p1[i].if_append = false; // putting the piece back
					}
				}
			}	
		}
	}
}


// useful functions

int append_check(game_parameters g1, piece p1[g1.number_of_pieces])
{
	int append_checker = 0;
	for(int i=0; i<g1.number_of_pieces; i++)
	{
		if(p1[i].alive == true && p1[i].if_append == true)
		{
			append_checker = i;
			break;
		}
	}

	return append_checker;
}

void check_men_position(game_parameters g1, piece p1[g1.number_of_pieces], square square_attributes[g1.board_dimensions][g1.board_dimensions])
{
	for(int i=0; i<g1.board_dimensions; i++)
	{
		for(int j=0; j<g1.board_dimensions; j++)
		{
			square_attributes[i][j].empty = true;
			for(int k=0; k<g1.number_of_pieces; k++)
			{
				if(p1[k].pos_x == square_attributes[i][j].x && p1[k].pos_y == square_attributes[i][j].y)
				{
					square_attributes[i][j].empty = false;
					break;
				}
			}
		}
	}
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

/*
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
*/
