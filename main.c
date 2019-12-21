#include <stdio.h>

/*
TODO
- fix ask_for_dimensions (choosing board size?)
- think about latter mechanics (men? move? conditions?)
*/

struct sizeof_board
{
    // square dimensions

    int square_width;
    int square_height;

    // number of squares

    int dimensions;

    // size

    int rows;
    int columns;
};

struct selector
{
    // position
    // i - vertical axis, j - horizontal axis

    int i;
    int j;

    //dimensions

    int width;
    int height;
};

struct piece
{
	int colour; // 0 - black, 1 - white
	int type; // 0 - man, 1 - king
	int pos_x;
	int pos_y;
};

void clear_screen();
void fill_board(struct sizeof_board b1, char board[b1.rows][b1.columns]);
void print_board(struct sizeof_board b1, char board[b1.rows][b1.columns]);
void move_selector(struct sizeof_board b1, char board[b1.rows][b1.columns], struct selector *s1);
void set_selector(struct sizeof_board b1, char board[b1.rows][b1.columns], struct selector s1);
void clear_selector(struct sizeof_board b1, char board[b1.rows][b1.columns], struct selector *s1);
void ask_for_dimensions(int *x);

int main()
{
    int x = 0;
    //ask_for_dimensions(&x);
    struct sizeof_board b1 = {10, 5, 10/*x*/, b1.square_height*b1.dimensions+1, b1.square_width*b1.dimensions+1}; // define board size
    struct selector s1 = {1, 1}; // define selector starting position
    char board[b1.rows][b1.columns]; // create an array for board
    int end_of_game = 0; // variable for end of the game

    fill_board(b1, board); // fill board array with graphic symbols
    while(!end_of_game)
    {
        clear_screen(); // clear the console
        set_selector(b1, board, s1); // input the selector symbols into the board array
        print_board(b1, board); // print the board array
        printf("%d %d", s1.i, s1.j); // debugging line
        move_selector(b1, board, &s1); // change selector position
    }
    return 0;
}

void clear_screen()
{
    #ifdef _WIN32
        system("cls");
    #elif __APPLE__
        printf ("\33c\e[3J");
    #endif
}

void fill_board(struct sizeof_board b1, char board[b1.rows][b1.columns])
{
    // fill the board

    for(int i=0; i<b1.rows; i=i+b1.square_height) // every b1.square_height row starting from 0
    {
        for(int j=0; j<b1.columns; j++) // fill the row with dashes
        {
            board[i][j] = '-';
        }
    }

    for(int i=1; i<b1.rows; i=i+b1.square_height) // every b1.square_height row starting from 1
    {
        for(int k=i; k<i+b1.square_height-1; k++) // go b1.square_height - 1 rows at a time
        {
            for(int j=0; j<b1.columns; j=j+b1.square_width) // fill every b1.square_width space (start 0) with vertical line
            {
                board[k][j] = '|';
            }
        }
    }

    for(int i=1; i<b1.rows; i=i+b1.square_height) // every b1.square_height row starting from 1
    {
        for(int l=i; l<i+b1.square_height-1; l++) // go b1.square_height - 1 rows at a time
        {
            for(int j=1; j<b1.columns; j = j+b1.square_width) // set j on every b1.square_width space
            {
                for(int k=j; k<j+b1.square_width-1; k++) // fill b1.square_width - 1 consecutive spaces with space
                {
                    board[l][k] = ' ';
                }
            }
        }
    }

}

void print_board(struct sizeof_board b1, char board[b1.rows][b1.columns])
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

void move_selector(struct sizeof_board b1, char board[b1.rows][b1.columns], struct selector *s1)
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
            if(s1->j == 1) // doing nothing if the selector is on the left border
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
            if(s1->i == 1) // doing nothing if the selector is on the upper border
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
    }
}

void set_selector(struct sizeof_board b1, char board[b1.rows][b1.columns], struct selector s1)
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

void clear_selector(struct sizeof_board b1, char board[b1.rows][b1.columns], struct selector *s1)
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

void ask_for_dimensions(int *x)
{
    int f = 0;
    while(!f)
    {
        printf("What size of the board would you like to print? ");
        scanf("&d", &x);
        if(x<=0)
        {
            printf("\nInvalid value! Try once again\n\n");
            f = 0;
        }
        else
        {
            f = 1;
        }
   }
}
