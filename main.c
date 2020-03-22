/* pythoncase functions included for readibility,
they serve as "child functions" to the ones above */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//versioning
#define MAJOR_VER 0
#define MIN0R_VER 9

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>

int getch (void)
{
    int key;
    struct termios oldSettings, newSettings;

    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
    key = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    return key;
}
#endif

// b_ stands for boolean
#define b_BLACK false
#define b_WHITE true

#define SIGN(x) (((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))


typedef struct game_parameters
{
    bool menu_on;
    bool back_to_menu;
    bool end_of_game;
    bool who_won;

    bool whose_turn;
    bool turn_changed;

    enum game_status_indicator
    {
        gsi_nothing,
        gsi_square_taken,
        gsi_move_not_possible,
        gsi_capturing_mandatory
    }game_status_indicator;
    int prev_game_status_indicator;

    int board_dimensions;
    int number_of_pieces;

    struct save_parameters
    {
        bool was_loaded;
        bool save_error;
        bool load_error;
    }sp1;
}game_parameters;

typedef struct size_of_board
{
    int square_width;
    int square_height;

    int border_x;
    int border_y;

    int rows;
    int cols;
}size_of_board;

typedef struct square_attributes
{
    bool can_move;
    bool empty;

    // coordinates
    int x;
    int y;
}square_attributes;

typedef struct selector
{
    int pos_x;
    int pos_y;
}selector;

typedef struct piece
{
    bool alive;
    bool colour;
    bool king;
    bool if_append;
    bool can_capture;
    bool captured;
    bool other_on_the_way;

    struct position
    {
        int x;
        int y;

        int prev_x;
        int prev_y;
    }pos;

    struct where_to_capture
    {
        bool up_right;
        bool up_left;
        bool down_right;
        bool down_left;
    }wtc;
}piece;


// assigning initial attributes

void assign_square_attributes(game_parameters gp1,
                              size_of_board sob1,
                              square_attributes sa1[gp1.board_dimensions]
                                                   [gp1.board_dimensions]);

void create_pieces(game_parameters gp1,
                   size_of_board sob1,
                   piece p1[gp1.number_of_pieces]);


// filling the board array

void fill_board(size_of_board sob1,
                char board[sob1.rows][sob1.cols]);

void fill_pieces(game_parameters gp1,
                 size_of_board sob1,
                 char board[sob1.rows][sob1.cols],
                 piece p1[gp1.number_of_pieces]);

void Fill(char colour[],
          char type[],
          int piece_index,
          game_parameters gp1,
          size_of_board sob1,
          char board[sob1.rows][sob1.cols],
          piece p1[gp1.number_of_pieces]);

void set_selector(size_of_board sob1,
                  char board[sob1.rows][sob1.cols],
                  selector s1);

void clear_selector(game_parameters gp1,
                    size_of_board sob1,
                    char board[sob1.rows][sob1.cols],
                    selector *s1,
                    piece p1[gp1.number_of_pieces]);

void fill_move_possibility(game_parameters gp1,
                           size_of_board sob1,
                           char board[sob1.rows][sob1.cols],
                           square_attributes sa1[gp1.board_dimensions]
                                                [gp1.board_dimensions],
                           piece p1[gp1.number_of_pieces]);


// printing stuff

void print_board(size_of_board sob1,
                 char board[sob1.rows][sob1.cols]);

void print_info(game_parameters *gp1,
                size_of_board sob1,
                square_attributes sa1[gp1->board_dimensions]
                                     [gp1->board_dimensions],
                selector s1,
                piece p1[gp1->number_of_pieces]);

void show_menu(game_parameters *gp1,
               square_attributes sa1[gp1->board_dimensions]
                                    [gp1->board_dimensions],
               selector *s1,
               piece p1[gp1->number_of_pieces]);

void Konami();


// mechanics

void controls(game_parameters *gp1,
              size_of_board sob1,
              char board[sob1.rows][sob1.cols],
              square_attributes sa1[gp1->board_dimensions]
                                   [gp1->board_dimensions],
              selector *s1, piece p1[gp1->number_of_pieces]);

void Move(char direction[],
          game_parameters *gp1,
          size_of_board sob1,
          char board[sob1.rows][sob1.cols],
          selector *s1,
          piece p1[gp1->number_of_pieces]);

void append(game_parameters *gp1,
            size_of_board sob1,
            square_attributes sa1[gp1->board_dimensions]
                                 [gp1->board_dimensions],
            selector *s1,
            piece p1[gp1->number_of_pieces]);
/* functionality connected with appending/unappending a piece to
the selector and moving the piece */

void check_move_possibility(game_parameters *gp1,
                            size_of_board sob1,
                            square_attributes sa1[gp1->board_dimensions]
                                                [gp1->board_dimensions],
                            selector *s1,
                            piece p1[gp1->number_of_pieces]);

void CheckMove(char piece_type[],
               char capturing_mandatory[],
               game_parameters *gp1,
               square_attributes sa1[gp1->board_dimensions]
                                    [gp1->board_dimensions],
               piece p1[gp1->number_of_pieces],
               int piece_index,
               int vert,
               int horiz);

void check_putback_possibility(game_parameters *gp1,
                               square_attributes sa1[gp1->board_dimensions]
                                                    [gp1->board_dimensions],
                               selector *s1,
                               piece p1[gp1->number_of_pieces],
                               bool *can_not_move,
                               bool *other_under_appended);

void check_capture_possibility(game_parameters *gp1,
                               square_attributes sa1[gp1->board_dimensions]
                                                    [gp1->board_dimensions],
                               selector s1,
                               piece p1[gp1->number_of_pieces]);

void CheckCapture(char piece_type[],
                  game_parameters *gp1,
                  square_attributes sa1[gp1->board_dimensions]
                                       [gp1->board_dimensions],
                  selector s1,
                  piece p1[gp1->number_of_pieces],
                  int piece_index,
               	  int vert,
               	  int horiz);

void capture(game_parameters *gp1,
             size_of_board sob1,
             square_attributes sa1[gp1->board_dimensions]
                                  [gp1->board_dimensions],
             piece p1[gp1->number_of_pieces]);

void CapturePiece(char piece_type[],
				  game_parameters *gp1,
				  size_of_board sob1,
				  square_attributes sa1[gp1->board_dimensions]
				                       [gp1->board_dimensions],
				  piece p1[gp1->number_of_pieces],
				  int piece_index,
				  int vert,
				  int horiz);

void check_for_kings(game_parameters gp1, 
                     size_of_board sob1,
                     piece p1[gp1.number_of_pieces]);

void change_turns(game_parameters *gp1,
                  piece p1[gp1->number_of_pieces]);

void check_end_of_game(game_parameters *gp1,
                       size_of_board sob1,
                       square_attributes sa1[gp1->board_dimensions]
                                            [gp1->board_dimensions],
                       piece p1[gp1->number_of_pieces]);

void end_of_game(game_parameters *gp1);


// game save/read

void game_save(game_parameters *gp1,
               square_attributes sa1[gp1->board_dimensions]
                                    [gp1->board_dimensions],
               selector s1,
               piece p1[gp1->number_of_pieces]);

void load_game_save(game_parameters *gp1,
                    square_attributes sa1[gp1->board_dimensions]
                                         [gp1->board_dimensions],
                    selector *s1,
                    piece p1[gp1->number_of_pieces]);


// useful functions

int append_check(game_parameters gp1,
                 piece p1[gp1.number_of_pieces]);

void check_men_position(game_parameters gp1,
                        square_attributes sa1[gp1.board_dimensions]
                                             [gp1.board_dimensions],
                        piece p1[gp1.number_of_pieces]);

void restore_previous_position(game_parameters gp1,
                               piece p1[gp1.number_of_pieces]);


int main()
{
    // initial parameters
    game_parameters gp1 = {true, false, false, b_WHITE, b_WHITE, false,
                           gsi_nothing, 0, 10, 40, false, false};
    do
    {
        gp1.menu_on = true;
        gp1.back_to_menu = false;
        gp1.whose_turn = b_WHITE;
        gp1.turn_changed = false;
        gp1.game_status_indicator = gsi_nothing;
        gp1.prev_game_status_indicator = 0;

        // defining initial game objects
        size_of_board sob1 = {10, 5, 0, 0,
                              sob1.square_height * gp1.board_dimensions
                              + sob1.border_y + 1,
                              sob1.square_width * gp1.board_dimensions
                              + sob1.border_x + 1};

        square_attributes sa1[gp1.board_dimensions][gp1.board_dimensions];
        assign_square_attributes(gp1, sob1, sa1);
        selector s1 = {sob1.border_x + 1, sob1.border_y + 1};
        char board[sob1.rows][sob1.cols];
        piece p1[gp1.number_of_pieces];
        create_pieces(gp1, sob1, p1);

        // filling game objects with initial content
        fill_board(sob1, board);

        while(gp1.menu_on)
        {
            show_menu(&gp1, sa1, &s1, p1);
        }

        // gameplay
        while(!gp1.end_of_game && !gp1.back_to_menu)
        {
            printf("\33c\e[3J");
            check_men_position(gp1, sa1, p1);
            fill_move_possibility(gp1, sob1, board, sa1, p1);
            fill_pieces(gp1, sob1, board, p1);
            set_selector(sob1, board, s1);
            print_board(sob1, board);
            print_info(&gp1, sob1, sa1, s1, p1);
            controls(&gp1, sob1, board, sa1, &s1, p1);
            if(gp1.game_status_indicator == gsi_capturing_mandatory)
            {
                capture(&gp1, sob1, sa1, p1);
            }
            check_capture_possibility(&gp1, sa1, s1, p1);
            change_turns(&gp1, p1);
            if(gp1.turn_changed == true)
            {
                check_capture_possibility(&gp1, sa1, s1, p1);
            }
            check_for_kings(gp1, sob1, p1);
            restore_previous_position(gp1, p1);

            check_end_of_game(&gp1, sob1, sa1, p1);
        }

	    if(gp1.end_of_game)
	    {
	    	end_of_game(&gp1);
	    }

    }while(gp1.back_to_menu);

    return 0;
}


// assigning initial attributes

void assign_square_attributes(game_parameters gp1,
                              size_of_board sob1,
                              square_attributes sa1[gp1.board_dimensions]
                                                   [gp1.board_dimensions])
{
    for(int vert = 0; vert < gp1.board_dimensions; ++vert)
    {
        for(int horiz = 0; horiz < gp1.board_dimensions; ++horiz)
        {
            sa1[vert][horiz].x = sob1.border_x + 1 + horiz * sob1.square_width;
            sa1[vert][horiz].y = sob1.border_y + 1 + vert * sob1.square_height;
            sa1[vert][horiz].can_move = false;
        }
    }
}

void create_pieces(game_parameters gp1,
                   size_of_board sob1,
                   piece p1[gp1.number_of_pieces])
{
    for(int piece_index = 0; piece_index < gp1.number_of_pieces; ++piece_index)
    {
        p1[piece_index].alive = true;
        p1[piece_index].king = false;
        p1[piece_index].if_append = false;
        p1[piece_index].can_capture = false;
        p1[piece_index].captured = false;
        p1[piece_index].other_on_the_way = false;

        p1[piece_index].wtc.up_right = false;
        p1[piece_index].wtc.up_left = false;
        p1[piece_index].wtc.down_right = false;
        p1[piece_index].wtc.down_left = false;

        if(piece_index < gp1.number_of_pieces / 2)
        {
            p1[piece_index].colour = b_BLACK;
        }
        else
        {
            p1[piece_index].colour = b_WHITE; 
        }
    }
    
    for(int col_step = 0;
        col_step < 2 * gp1.number_of_pieces / gp1.board_dimensions; ++col_step)
    {
        for(int row_step = 0; row_step < gp1.board_dimensions / 2; ++row_step)
        {
            if(p1[row_step + gp1.board_dimensions / 2 * col_step].colour
               == b_BLACK)
            {
                p1[row_step + gp1.board_dimensions / 2 * col_step].pos.x
                = sob1.border_x + 1 + ( row_step * 2 + !(col_step % 2) )
                * sob1.square_width;

                p1[row_step + gp1.board_dimensions / 2 * col_step].pos.y
                = sob1.border_y + 1 + col_step * sob1.square_height;
            }
            else
            {
                p1[row_step + gp1.board_dimensions / 2 * col_step].pos.x
                = sob1.cols - sob1.square_width
                - ( row_step * 2 + !(col_step % 2) ) * sob1.square_width;

                p1[row_step + gp1.board_dimensions / 2 * col_step].pos.y
                = sob1.rows - sob1.square_height
                - ( col_step - gp1.number_of_pieces / gp1.board_dimensions )
                * sob1.square_height;
            }

            p1[row_step + gp1.board_dimensions / 2 * col_step].pos.prev_x 
            = p1[row_step + gp1.board_dimensions / 2 * col_step].pos.x;

            p1[row_step + gp1.board_dimensions / 2 * col_step].pos.prev_y
            = p1[row_step + gp1.board_dimensions / 2 * col_step].pos.y;
        }   
    }
}


// filling the board array

void fill_board(size_of_board sob1,
                char board[sob1.rows][sob1.cols])
{
    // fill the first sob1.border_y rows with spaces
    for(int row = 0; row < sob1.border_y; ++row)
    {
        for(int col = 0; col < sob1.cols; ++col)
        {
            board[row][col] = ' ';
        }
    }

    for(int row = sob1.border_y; row < sob1.rows; row += sob1.square_height)
    // every sob1.square_height row starting from sob1.border_y
    {
        for(int col = 0; col < sob1.border_x; ++col)
        {
            board[row][col] = ' ';
            // fill the first sob1.border_x cells with spaces
        }

        for(int col = sob1.border_x; col < sob1.cols; ++col)
        // fill the row with dashes
        {
            board[row][col] = '-';
        }
    }

    for(int row_step = sob1.border_y+1; row_step < sob1.rows;
        row_step += sob1.square_height)
    // every sob1.square_height row starting from sob1.border_y
    {
        for(int row = row_step; row < row_step+sob1.square_height-1; ++row)
        // go sob1.square_height-1 rows at a time
        {
            for(int col = 0; col < sob1.border_x; ++col)
            {
                board[row][col] = ' ';
                // fill the first sob1.border_x cells with spaces
            }

            for(int col = sob1.border_x; col < sob1.cols;
                col += sob1.square_width)
            /* fill every sob1.square_width cell starting from sob1.border_x
               with vertical line */
            {
                board[row][col] = '|';
            }
        }
    }

    for(int row_step = sob1.border_y+1; row_step < sob1.rows;
        row_step += sob1.square_height)
    // every sob1.square_height row starting from sob1.border_y+1
    {
        for(int row = row_step; row < row_step+sob1.square_height-1; ++row)
        // go sob1.square_height-1 rows at a time
        {
            for(int col = 0; col < sob1.border_x; ++col)
            {
                board[row][col] = ' ';
                // fill the first sob1.border_x cells with spaces
            }

            for(int col_step = sob1.border_x+1; col_step < sob1.cols;
                col_step += sob1.square_width)
            /* set col_step on every sob1.square_width cell starting from
               sob1.border_x+1 */
            {          
                for(int col = col_step; col < col_step+sob1.square_width-1;
                    ++col)
                // fill sob1.square_width-1 consecutive cells with space
                {
                    board[row][col] = ' ';
                }
            }
        }
    }
}

void fill_pieces(game_parameters gp1,
                 size_of_board sob1,
                 char board[sob1.rows][sob1.cols],
                 piece p1[gp1.number_of_pieces])
{
    bool append_checker = append_check(gp1, p1);
    int which_appended = append_check(gp1, p1) - 1;

    for(int piece_index=0; piece_index<gp1.number_of_pieces; ++piece_index)
    {
        if(p1[piece_index].alive == true)
        {
            for (int row = p1[piece_index].pos.y;
            	 row < p1[piece_index].pos.y + sob1.square_height - 1; ++row)
            {
                for (int col = p1[piece_index].pos.x;
                	 col < p1[piece_index].pos.x + sob1.square_width - 1;
                	 ++col)
                {
                    board[row][col] = ' ';
                }
            }

            if(p1[piece_index].colour == b_WHITE)
            {
                if(p1[piece_index].king == true)
                {
                    Fill("white", "king", piece_index, gp1, sob1, board, p1);
                }
                else
                {
                    Fill("white", "man", piece_index, gp1, sob1, board, p1);
                }
            }
            else
            {
                if(p1[piece_index].king == true)
                {
                    Fill("black", "king", piece_index, gp1, sob1, board, p1);
                }
                else
                {
                    Fill("black", "man", piece_index, gp1, sob1, board, p1);
                }
            }
        }
    }

    if(append_checker != false)
    {
        for (int row = p1[which_appended].pos.y;
        	 row < p1[which_appended].pos.y + sob1.square_height - 1; ++row)
        {
            for (int col = p1[which_appended].pos.x;
            	 col < p1[which_appended].pos.x + sob1.square_width - 1; ++col)
            {
                board[row][col] = ' ';
            }
        }

        if(p1[which_appended].colour == b_WHITE)
        {
            if(p1[which_appended].king == true)
            {
                Fill("white", "king", which_appended, gp1, sob1, board, p1);
            }
            else
            {
                Fill("white", "man", which_appended, gp1, sob1, board, p1);
            }
        }
        else
        {
            if(p1[which_appended].king == true)
            {
                Fill("black", "king", which_appended, gp1, sob1, board, p1);
            }
            else
            {
                Fill("black", "man", which_appended, gp1, sob1, board, p1);
            }
        }
    }
}

void Fill(char colour[],
          char type[],
          int piece_index,
          game_parameters gp1,
          size_of_board sob1,
          char board[sob1.rows][sob1.cols],
          piece p1[gp1.number_of_pieces])
{
    if(strcmp(colour, "white") == 0 && strcmp(type, "king") == 0)
    {
        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+col] = 'O';

                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';

                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+1] = ' ';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';

                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+2] = ' ';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-4] = ' ';
            }
        }

        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+col] = '|';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+1] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';
            }
        }

        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+col] ='-';
                
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';

                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+1] = ' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';
            }
        }
    }
    
    if(strcmp(colour, "white") == 0 && strcmp(type, "man") == 0)
    {
        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+col] = 'O';

                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+1] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';
                
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+1] =' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-3] =' ';
            }
        }
    }

    if(strcmp(colour, "black") == 0 && strcmp(type, "king") == 0)
    {
        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+3] = 'O';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-5] = 'O';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+col] = '|';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+1] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';
            }
        }

        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+col] ='-';
                
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';

                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+1] = ' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';
            }
        }
    }

    if(strcmp(colour, "black") == 0 && strcmp(type, "man") == 0)
    {
        for(int row = 0; row < sob1.square_height - 1; ++row)
        {
            for(int col = 0; col < sob1.square_width - 1; ++col)
            {
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+col] = 'O';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+col] ='O';

                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+1] = 'O';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+2] = 'O';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-3] ='O';
                board[p1[piece_index].pos.y+row]
                	 [p1[piece_index].pos.x+sob1.square_width-4] ='O';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+1] = ' ';

                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';
                board[p1[piece_index].pos.y]
                	 [p1[piece_index].pos.x+sob1.square_width-3] = ' ';

                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x] = ' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-2] = ' ';
                
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+1] =' ';
                board[p1[piece_index].pos.y+sob1.square_height-2]
                	 [p1[piece_index].pos.x+sob1.square_width-3] =' ';
            }
        }
    }
}

void set_selector(size_of_board sob1,
                  char board[sob1.rows][sob1.cols],
                  selector s1)
{
    board[s1.pos_y][s1.pos_x] = '|';
    board[s1.pos_y][s1.pos_x+sob1.square_width-2] = '|';
    board[s1.pos_y+sob1.square_height-2][s1.pos_x] = '|';
    board[s1.pos_y+sob1.square_height-2][s1.pos_x+sob1.square_width-2] = '|';
}

void clear_selector(game_parameters gp1,
                    size_of_board sob1,
                    char board[sob1.rows][sob1.cols],
                    selector *s1,
                    piece p1[gp1.number_of_pieces])
{
    board[s1->pos_y][s1->pos_x] = ' ';
    board[s1->pos_y][s1->pos_x+sob1.square_width-2] = ' ';
    board[s1->pos_y+sob1.square_height-2][s1->pos_x] = ' ';
    board[s1->pos_y+sob1.square_height-2][s1->pos_x+sob1.square_width-2] = ' ';

    for(int piece_index = 0; piece_index < gp1.number_of_pieces; ++piece_index)
    {
        if(p1[piece_index].pos.x == s1->pos_x
           && p1[piece_index].pos.y == s1->pos_y)
        {
            if(p1[piece_index].if_append == true)
            {
                for(int row = p1[piece_index].pos.y;
                	row < p1[piece_index].pos.y + sob1.square_height - 1;
                	++row)
                {
                    for(int col = p1[piece_index].pos.x;
                    	col < p1[piece_index].pos.x + sob1.square_width - 1;
                    	++col)
                    {
                        board[row][col] = ' ';
                    }
                }
                
            }
        }
    }
}

void fill_move_possibility(game_parameters gp1,
                           size_of_board sob1,
                           char board[sob1.rows][sob1.cols],
                           square_attributes sa1[gp1.board_dimensions]
                                                [gp1.board_dimensions],
                           piece p1[gp1.number_of_pieces])
{
    bool append_checker = append_check(gp1, p1);

    for(int vert = 0; vert < gp1.board_dimensions; ++vert)
    {
        for(int horiz = 0; horiz < gp1.board_dimensions; ++horiz)
        {
            if(sa1[vert][horiz].can_move == true && append_checker == true)
            {
                if(sa1[vert][horiz].empty == true)
                {
                    for (int row = sa1[vert][horiz].y;
                    	 row < sa1[vert][horiz].y + sob1.square_height - 1;
                    	 ++row)
                    {
                        board[row][sa1[vert][horiz].x] = '|';
                        board[row][sa1[vert]
                        	 [horiz].x+sob1.square_width-2] = '|';
                    }
                }
            }
            else
            {
                for (int row = sa1[vert][horiz].y;
                	 row < sa1[vert][horiz].y + sob1.square_height - 1; ++row)
                {
                    for (int col = sa1[vert][horiz].x;
                    	 col < sa1[vert][horiz].x + sob1.square_width - 1;
                    	 ++col)
                    {
                        board[row][col] = ' ';
                    }
                }
            }   
        }
    }
}


// printing stuff

void print_board(size_of_board sob1,
                 char board[sob1.rows][sob1.cols])
{
    for(int row = 0; row < sob1.rows; ++row)
    {
        for(int col = 0; col < sob1.cols; ++col)
        {
            printf("%c", board[row][col]);
        }

        printf("\n");
    }
}

void print_info(game_parameters *gp1,
                size_of_board sob1,
                square_attributes sa1[gp1->board_dimensions]
                                     [gp1->board_dimensions],
                selector s1,
                piece p1[gp1->number_of_pieces])
{
    if(gp1->whose_turn == b_WHITE)
    {
        printf("White turn");
    }
    else
    {
        printf("Black turn");
    }

    switch(gp1->game_status_indicator)
    {
        case gsi_nothing:
            printf("\n");
            break;
        case gsi_square_taken:
            printf("\nThis square is taken!\n");
            gp1->game_status_indicator = gp1->prev_game_status_indicator;
            gp1->prev_game_status_indicator = gsi_nothing;
            break;
        case gsi_move_not_possible:
            printf("\nCan't touch this!\n");
            gp1->game_status_indicator = gp1->prev_game_status_indicator;
            gp1->prev_game_status_indicator = gsi_nothing;
            break;
        case gsi_capturing_mandatory:
            printf("\nCapturing mandatory!\n");
            break;
        default:
            break;
    }

    if(gp1->sp1.save_error == true)
    {
        gp1->sp1.save_error = false;
        printf("Game save error!");
    }
}

void show_menu(game_parameters *gp1,
               square_attributes sa1[gp1->board_dimensions]
                                    [gp1->board_dimensions],
               selector *s1,
               piece p1[gp1->number_of_pieces])
{
    printf("\33c\e[3J");
    printf("Draughts ver. %d.%d\n\n", MAJOR_VER, MIN0R_VER);
    printf("\nMAIN MENU:\n\n");
    printf("1) Start a new game\n");
    printf("2) Load a saved game\n");
    printf("3) Instructions\n");
    printf("4) Exit\n");
    printf("\nChoose your action: ");

    char choice = '0';
    choice = getch();

    switch(choice)
    {
        case '1':
            gp1->menu_on = false;
            break;
        case '2':
            load_game_save(gp1, sa1, s1, p1);
            if(gp1->sp1.load_error == true)
            {
                gp1->sp1.load_error = false;
                printf("\n\nError loading game!");
                printf("\nPress [Enter] to return to menu...");
                getch();
                break;
            }
            else
            {
                gp1->menu_on = false;
                break;   
            }
        case '3':
            printf("\33c\e[3J");
            printf("Use W, A, S and D to move the selector "
                   "and E to grab the piece. "
                   "Use X to return to menu and Z "
                   "to save current game state.\n");
            printf("\nPress [Enter] to return to menu...");
            getch();
            break;
        case '4':
            printf("\33c\e[3J");
            exit(0);
            break;
        case 'w':
            Konami();           
            break;
        default:
            break;
    }
}

void Konami()
{
    if(getch() == 'w')
    {
        if(getch() == 's')
        {
            if(getch() == 's')
            {
                if(getch() == 'a')
                {
                    if(getch() == 'd')
                    {
                        if(getch() == 'a')
                        {
                            if(getch() == 'd')
                            {
                                if(getch() == 'b')
                                {
                                    if(getch() == 'a')
                                    {
                                        printf("\33c\e[3J");
                                        printf("It's over 9000!");
                                        getch();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


// mechanics

void controls(game_parameters *gp1,
              size_of_board sob1,
              char board[sob1.rows][sob1.cols],
              square_attributes sa1[gp1->board_dimensions]
                                   [gp1->board_dimensions],
              selector *s1, piece p1[gp1->number_of_pieces])
{
    char key = 0;

    while(1)
    {
        key = getch();

        if(key == 'd')
        {
            if(s1->pos_x == sob1.cols - sob1.square_width)
            // doing nothing if the selector is on the right border
            {

            }
            else
            {
                Move("right", gp1, sob1, board, s1, p1);   
                break;
            }
        }
        else if(key == 'a')
        {
            if(s1->pos_x == sob1.border_x + 1)
            // doing nothing if the selector is on the left border
            {

            }
            else
            {
                Move("left", gp1, sob1, board, s1, p1);
                break;
            }
        }
        else if(key == 'w')
        {
            if(s1->pos_y == sob1.border_y + 1)
            // doing nothing if the selector is on the upper border
            {

            }
            else
            {
                Move("up", gp1, sob1, board, s1, p1);
                break;
            }
        }
        else if(key == 's')
        {
            if(s1->pos_y == sob1.rows - sob1.square_height)
            // doing nothing if the selector is on the lower border
            {

            }
            else
            {
                Move("down", gp1, sob1, board, s1, p1);
                break;
            }
        }
        else if(key == 'e')
        {
            append(gp1, sob1, sa1, s1, p1);
            break;
        }
        else if(key == 'z')
        {
            game_save(gp1, sa1, *s1, p1);
            break;
        }
        else if(key == 'x')
        {
            printf("\33c\e[3J");
            gp1->back_to_menu = true;
            break;
        }
    }
}

void Move(char direction[],
          game_parameters *gp1,
          size_of_board sob1,
          char board[sob1.rows][sob1.cols],
          selector *s1,
          piece p1[gp1->number_of_pieces])
{
    if(strcmp(direction, "right") == 0)
    {
        clear_selector(*gp1, sob1, board, s1, p1);

        for(int piece_index = 0; piece_index < gp1->number_of_pieces;
            ++piece_index)
        {
            if(p1[piece_index].pos.x == s1->pos_x
               && p1[piece_index].pos.y == s1->pos_y)
            {
                if(p1[piece_index].if_append == true)
                {
                    p1[piece_index].pos.x += sob1.square_width;
                    break;
                }
            }
        }

        s1->pos_x += sob1.square_width;
    }

    if(strcmp(direction, "left") == 0)
    {
        clear_selector(*gp1, sob1, board, s1, p1);

        for(int piece_index = 0; piece_index < gp1->number_of_pieces;
            ++piece_index)
        {
            if(p1[piece_index].pos.x == s1->pos_x
               && p1[piece_index].pos.y == s1->pos_y)
            {
                if(p1[piece_index].if_append == true)
                {
                    p1[piece_index].pos.x -= sob1.square_width;
                    break;
                }
            }
        }

        s1->pos_x -= sob1.square_width;
    }

    if(strcmp(direction, "up") == 0)
    {
        clear_selector(*gp1, sob1, board, s1, p1);

        for(int piece_index = 0; piece_index < gp1->number_of_pieces;
            ++piece_index)
        {
            if(p1[piece_index].pos.x == s1->pos_x
               && p1[piece_index].pos.y == s1->pos_y)
            {
                if(p1[piece_index].if_append == true)
                {
                    p1[piece_index].pos.y -= sob1.square_height;
                    break;
                }
            }
        }

        s1->pos_y -= sob1.square_height;
    }

    if(strcmp(direction, "down") == 0)
    {
        clear_selector(*gp1, sob1, board, s1, p1);

        for(int piece_index = 0; piece_index < gp1->number_of_pieces;
            ++piece_index)
        {
            if(p1[piece_index].pos.x == s1->pos_x
               && p1[piece_index].pos.y == s1->pos_y)
            {
                if(p1[piece_index].if_append == true)
                {
                    p1[piece_index].pos.y += sob1.square_height;
                    break;
                }
            }
        }

        s1->pos_y += sob1.square_height;
    }
}

void append(game_parameters *gp1,
            size_of_board sob1,
            square_attributes sa1[gp1->board_dimensions]
                                 [gp1->board_dimensions],
            selector *s1,
            piece p1[gp1->number_of_pieces])
/* functionality connected with appending/unappending a piece to
the selector and moving the piece */
{
    bool append_checker = append_check(*gp1, p1);
    bool other_under_appended = false;
    bool can_not_move = false;

    for(int piece_index = 0; piece_index < gp1->number_of_pieces;
    	++piece_index)
    {
        if(p1[piece_index].alive == true
           && p1[piece_index].pos.x == s1->pos_x
           && p1[piece_index].pos.y == s1->pos_y
           && p1[piece_index].colour == gp1->whose_turn)
        {
            if(append_checker == false)
            {
                p1[piece_index].pos.prev_y = p1[piece_index].pos.y;
                p1[piece_index].pos.prev_x = p1[piece_index].pos.x;
                p1[piece_index].if_append = true; // picking the piece up

                check_move_possibility(gp1, sob1, sa1, s1, p1);
                break;
            }
            else // checking if putting the piece back is possible
            {
                check_putback_possibility(gp1, sa1, s1, p1, &can_not_move,
                                          &other_under_appended);

                // final checking for conditions
                if(other_under_appended == true)
                {
                    gp1->prev_game_status_indicator
                    = gp1->game_status_indicator;

                    gp1->game_status_indicator = gsi_square_taken;
                    break;
                }
                else if(can_not_move == true)
                {
                    gp1->prev_game_status_indicator
                    = gp1->game_status_indicator;

                    gp1->game_status_indicator = gsi_move_not_possible;
                    break;
                }
                else
                {
                    p1[piece_index].if_append = false;
                    // putting the piece back

                    for(int vert = 0; vert < gp1->board_dimensions; ++vert)
                    {
                        for (int horiz = 0; horiz < gp1->board_dimensions;
                        	 ++horiz)
                        {
                            sa1[vert][horiz].can_move = false;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void check_move_possibility(game_parameters *gp1,
                            size_of_board sob1,
                            square_attributes sa1[gp1->board_dimensions]
                                                [gp1->board_dimensions],
                            selector *s1,
                            piece p1[gp1->number_of_pieces])
{
    for (int piece_index = 0; piece_index < gp1->number_of_pieces; 
         ++piece_index)
    {
        if(p1[piece_index].pos.x == s1->pos_x
           && p1[piece_index].pos.y == s1->pos_y
           && p1[piece_index].alive == true)
        {
            for(int vert = 0; vert < gp1->board_dimensions; ++vert)
            {
                for (int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
                {
                    if(gp1->game_status_indicator != gsi_capturing_mandatory)
                    {
                        if(sa1[vert][horiz].x == p1[piece_index].pos.x
                           && sa1[vert][horiz].y == p1[piece_index].pos.y)
                        {
                            if(p1[piece_index].king == false)
                            {
                                CheckMove("man", "false",
                                          gp1, sa1, p1, piece_index, vert,
                                          horiz);
                            }
                            else
                            {
                                CheckMove("king", "false",
                                          gp1, sa1, p1, piece_index, vert,
                                          horiz);
                            }  
                        }
                    }       
                    else
                    {
                        if(p1[piece_index].king == false)
                        {
                            CheckMove("man", "true",
                                      gp1, sa1, p1, piece_index, vert, horiz);
                        }
                        else
                        {
                            CheckMove("king", "true",
                                      gp1, sa1, p1, piece_index, vert, horiz);
                        }
                    }
                    
                    if(sa1[vert][horiz].x == p1[piece_index].pos.prev_x
                       && sa1[vert][horiz].y == p1[piece_index].pos.prev_y)
                    {
                        sa1[vert][horiz].can_move = true;
                    }
                }   
            }
        }
    }
}

void CheckMove(char piece_type[],
               char capturing_mandatory[],
               game_parameters *gp1,
               square_attributes sa1[gp1->board_dimensions]
                                    [gp1->board_dimensions],
               piece p1[gp1->number_of_pieces],
               int piece_index,
               int vert,
               int horiz)
{
    if(strcmp(piece_type, "man") == 0
       && strcmp(capturing_mandatory, "false") == 0)
    {
        if (p1[piece_index].colour == b_WHITE)
        {
            if(sa1[vert-1][horiz+1].empty == true)
            {
                if(vert > 0 && horiz < gp1->board_dimensions - 1)
                {
                    sa1[vert-1][horiz+1].can_move = true;
                }
            }

            if(sa1[vert-1][horiz-1].empty == true)
            {
                if(vert > 0 && horiz > 0)
                {
                    sa1[vert-1][horiz-1].can_move = true;
                }
            }   
        }
        else
        {
            if(sa1[vert+1][horiz+1].empty == true)
            {
                if(vert < gp1->board_dimensions - 1
                   && horiz < gp1->board_dimensions - 1)
                {
                    sa1[vert+1][horiz+1].can_move = true;
                }
            }

            if(sa1[vert+1][horiz-1].empty == true)
            {
                if(vert < gp1->board_dimensions - 1 && horiz > 0)
                {
                    sa1[vert+1][horiz-1].can_move = true;
                }
            }
        }
    }

    if(strcmp(piece_type, "king") == 0
       && strcmp(capturing_mandatory, "false") == 0)
    {
        int square_iterator = 1;
        
        while(1)
        {
            if(vert - square_iterator >= 0
               && horiz + square_iterator < gp1->board_dimensions)
            {
                if(sa1[vert-square_iterator]
                	  [horiz+square_iterator].empty == true)
                {
                    sa1[vert-square_iterator]
                       [horiz+square_iterator].can_move = true;
                }
                else
                {
                    bool other_on_the_way = false;
                    for(int piece_2_index = 0;
                    	piece_2_index < gp1->number_of_pieces;
                        ++piece_2_index)
                    {
                        if(p1[piece_2_index].alive == true
                           && p1[piece_2_index].pos.x
                           == sa1[vert-square_iterator]
                           		 [horiz+square_iterator].x
                           && p1[piece_2_index].pos.y
                           == sa1[vert-square_iterator]
                           		 [horiz+square_iterator].y
                           && p1[piece_index].colour
                           == p1[piece_2_index].colour)
                        {
                            other_on_the_way = true;
                        }
                    }

                    if(other_on_the_way)
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }

            ++square_iterator;
        }

        square_iterator = 1;
        while(1)
        {
            if(vert - square_iterator >= 0 && horiz - square_iterator >= 0)
            {
                if(sa1[vert-square_iterator]
                	  [horiz-square_iterator].empty == true)
                {
                    sa1[vert-square_iterator]
                       [horiz-square_iterator].can_move = true;
                }
                else
                {
                    bool other_on_the_way = false;
                    for(int piece_2_index = 0;
                    	piece_2_index < gp1->number_of_pieces;
                        ++piece_2_index)
                    {
                        if(p1[piece_2_index].alive == true
                           && p1[piece_2_index].pos.x
                           == sa1[vert-square_iterator]
                           		 [horiz-square_iterator].x
                           && p1[piece_2_index].pos.y
                           == sa1[vert-square_iterator]
                           		 [horiz-square_iterator].y
                           && p1[piece_index].colour
                           == p1[piece_2_index].colour)
                        {
                            other_on_the_way = true;
                        }
                    }

                    if(other_on_the_way)
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }

            ++square_iterator;
        }

        while(1)
        {
            if(vert + square_iterator < gp1->board_dimensions
               && horiz + square_iterator < gp1->board_dimensions)
            {
                if(sa1[vert+square_iterator]
                	  [horiz+square_iterator].empty == true)
                {
                    sa1[vert+square_iterator]
                       [horiz+square_iterator].can_move = true;
                }
                else
                {
                    bool other_on_the_way = false;
                    for(int piece_2_index = 0;
                    	piece_2_index < gp1->number_of_pieces;
                        ++piece_2_index)
                    {
                        if(p1[piece_2_index].alive == true
                           && p1[piece_2_index].pos.x
                           == sa1[vert+square_iterator]
                           		 [horiz+square_iterator].x
                           && p1[piece_2_index].pos.y
                           == sa1[vert+square_iterator]
                           		 [horiz+square_iterator].y
                           && p1[piece_index].colour
                           == p1[piece_2_index].colour)
                        {
                            other_on_the_way = true;
                        }
                    }

                    if(other_on_the_way)
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }

            ++square_iterator;
        }

        square_iterator = 1;
        while(1)
        {
            if(vert + square_iterator < gp1->board_dimensions
               && horiz - square_iterator >= 0)
            {
                if(sa1[vert+square_iterator]
                	  [horiz-square_iterator].empty == true)
                {
                    sa1[vert+square_iterator]
                       [horiz-square_iterator].can_move = true;
                }
                else
                {
                    bool other_on_the_way = false;
                    for(int piece_2_index = 0;
                    	piece_2_index < gp1->number_of_pieces;
                        ++piece_2_index)
                    {
                        if(p1[piece_2_index].alive == true
                           && p1[piece_2_index].pos.x
                           == sa1[vert+square_iterator]
                           		 [horiz-square_iterator].x 
                           && p1[piece_2_index].pos.y
                           == sa1[vert+square_iterator]
                           		 [horiz-square_iterator].y
                           && p1[piece_index].colour
                           == p1[piece_2_index].colour)
                        {
                            other_on_the_way = true;
                        }
                    }

                    if(other_on_the_way)
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }

            ++square_iterator;
        }
    }

    if(strcmp(piece_type, "man") == 0
       && strcmp(capturing_mandatory, "true") == 0)
    {
        if(p1[piece_index].can_capture == true
           && sa1[vert][horiz].x == p1[piece_index].pos.x
           && sa1[vert][horiz].y == p1[piece_index].pos.y)
        {
            if(p1[piece_index].wtc.up_right == true)
            {
                sa1[vert-2][horiz+2].can_move = true;
            }

            if(p1[piece_index].wtc.up_left == true)
            {
                sa1[vert-2][horiz-2].can_move = true;
            }

            if(p1[piece_index].wtc.down_right == true)
            {
                sa1[vert+2][horiz+2].can_move = true;
            }

            if(p1[piece_index].wtc.down_left == true)
            {
                sa1[vert+2][horiz-2].can_move = true;
            }
        }
    }

    if(strcmp(piece_type, "king") == 0
       && strcmp(capturing_mandatory, "true") == 0)
    {
        if(p1[piece_index].can_capture == true
           && sa1[vert][horiz].x == p1[piece_index].pos.x
           && sa1[vert][horiz].y == p1[piece_index].pos.y)
        {
            if(p1[piece_index].wtc.up_right == true)
            {
                int square_iterator = 1;
                while(1)
                {
                    if(vert - square_iterator >= 1 
                       && horiz + square_iterator < gp1->board_dimensions - 1)
                    {
                        for(int piece_2_index = 0;
                        	piece_2_index < gp1->number_of_pieces;
                            ++piece_2_index)
                        {
                            if(p1[piece_2_index].alive == true
                               && sa1[vert-square_iterator]
                               		 [horiz+square_iterator].x
                               == p1[piece_2_index].pos.x
                               && sa1[vert-square_iterator]
                               		 [horiz+square_iterator].y 
                               == p1[piece_2_index].pos.y
                               && p1[piece_2_index].colour
                               == !p1[piece_index].colour)
                            {
                                int square_iterator_2 = 1;
                                while(1)
                                {
                                    if(vert - square_iterator-square_iterator_2
                                       >= 0
                                       && horiz + square_iterator 
                                       + square_iterator_2
                                       < gp1->board_dimensions)
                                    {
                                        sa1[vert-square_iterator
                                        	-square_iterator_2]
                                           [horiz+square_iterator
                                           	+square_iterator_2].can_move = true;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                ++square_iterator_2;
                                }
                            break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                    ++square_iterator;
                }
            }

            if(p1[piece_index].wtc.up_left == true)
            {
                int square_iterator = 1;
                while(1)
                {
                    if(vert - square_iterator >= 1
                       && horiz - square_iterator >= 1)
                    {
                        for(int piece_2_index = 0;
                        	piece_2_index < gp1->number_of_pieces;
                            ++piece_2_index)
                        {
                            if(p1[piece_2_index].alive == true
                               && sa1[vert-square_iterator]
                               		 [horiz-square_iterator].x
                               == p1[piece_2_index].pos.x
                               && sa1[vert-square_iterator]
                               		 [horiz-square_iterator].y
                               == p1[piece_2_index].pos.y
                               && p1[piece_2_index].colour
                               == !p1[piece_index].colour)
                            {
                                int square_iterator_2 = 1;
                                while(1)
                                {
                                    if(vert - square_iterator
                                       - square_iterator_2 >= 0
                                       && horiz - square_iterator
                                       - square_iterator_2 >= 0)
                                    {
                                        sa1[vert-square_iterator
                                        	-square_iterator_2]
                                           [horiz-square_iterator
                                           	-square_iterator_2].can_move = true;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                ++square_iterator_2;
                                }
                            break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                    ++square_iterator;
                }
            }

            if(p1[piece_index].wtc.down_right == true)
            {
                int square_iterator = 1;
                while(1)
                {
                    if(vert + square_iterator < gp1->board_dimensions - 1
                       && horiz + square_iterator < gp1->board_dimensions - 1)
                    {
                        for(int piece_2_index = 0;
                        	piece_2_index < gp1->number_of_pieces;
                            ++piece_2_index)
                        {
                            if(p1[piece_2_index].alive == true
                               && sa1[vert+square_iterator]
                               		 [horiz+square_iterator].x
                               == p1[piece_2_index].pos.x
                               && sa1[vert+square_iterator]
                               		 [horiz+square_iterator].y
                               == p1[piece_2_index].pos.y
                               && p1[piece_2_index].colour
                               == !p1[piece_index].colour)
                            {
                                int square_iterator_2 = 1;
                                while(1)
                                {
                                    if(vert + square_iterator
                                       + square_iterator_2
                                       < gp1->board_dimensions
                                       && horiz + square_iterator
                                       + square_iterator_2
                                       < gp1->board_dimensions)
                                    {
                                        sa1[vert+square_iterator
                                        	+square_iterator_2]
                                           [horiz+square_iterator
                                           	+square_iterator_2].can_move = true;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                ++square_iterator_2;
                                }
                            break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                    ++square_iterator;
                }
            }

            if(p1[piece_index].wtc.down_left == true)
            {
                int square_iterator = 1;
                while(1)
                {
                    if(vert + square_iterator < gp1->board_dimensions - 1
                       && horiz - square_iterator >= 1)
                    {
                        for(int piece_2_index = 0;
                        	piece_2_index < gp1->number_of_pieces;
                            ++piece_2_index)
                        {
                            if(p1[piece_2_index].alive == true
                               && sa1[vert+square_iterator]
                               		 [horiz-square_iterator].x
                               == p1[piece_2_index].pos.x
                               && sa1[vert+square_iterator]
                               		 [horiz-square_iterator].y
                               == p1[piece_2_index].pos.y
                               && p1[piece_2_index].colour
                               == !p1[piece_index].colour)
                            {
                                int square_iterator_2 = 1;
                                while(1)
                                {
                                    if(vert + square_iterator
                                       + square_iterator_2
                                       < gp1->board_dimensions
                                       && horiz - square_iterator
                                       - square_iterator_2 >= 0)
                                    {
                                        sa1[vert+square_iterator
                                        	+square_iterator_2]
                                           [horiz-square_iterator
                                           	-square_iterator_2].can_move = true;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                ++square_iterator_2;
                                }
                            break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                    ++square_iterator;
                }
            }
        }
    }
}

void check_putback_possibility(game_parameters *gp1,
                               square_attributes sa1[gp1->board_dimensions]
                                                    [gp1->board_dimensions],
                               selector *s1,
                               piece p1[gp1->number_of_pieces],
                               bool *can_not_move,
                               bool *other_under_appended)
{
    for(int vert = 0; vert < gp1->board_dimensions; ++vert)
    {
        for(int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
        {
            if(sa1[vert][horiz].x == s1->pos_x
               && sa1[vert][horiz].y == s1->pos_y
               && sa1[vert][horiz].can_move == false)
            {
                *can_not_move = true;
                break;
            }
        }
    }
    
    for(int piece_index = 0; piece_index < gp1->number_of_pieces;
        ++piece_index)
    {
        if(p1[piece_index].pos.x == s1->pos_x
           && p1[piece_index].pos.y == s1->pos_y
           && p1[piece_index].alive == true
           && p1[piece_index].if_append == false)
        {
            *other_under_appended = true;
            break;
        }
    }
}

void check_capture_possibility(game_parameters *gp1,
                               square_attributes sa1[gp1->board_dimensions]
                                                    [gp1->board_dimensions],
                               selector s1,
                               piece p1[gp1->number_of_pieces])
{
    for(int vert = 0; vert < gp1->board_dimensions; ++vert)
    {
        for (int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
        {
            for (int piece_index = 0; piece_index < gp1->number_of_pieces;
            	 ++piece_index)
            {   
                if(sa1[vert][horiz].x == p1[piece_index].pos.x
                   && sa1[vert][horiz].y == p1[piece_index].pos.y
                   && p1[piece_index].alive == true
                   && p1[piece_index].if_append == false
                   && p1[piece_index].colour == gp1->whose_turn)
                {
                    if(p1[piece_index].king == false)
                    {
                        CheckCapture("man", gp1, sa1, s1,
                        			 p1, piece_index, vert, horiz);
                    }
                    else
                    {
                        CheckCapture("king", gp1, sa1, s1,
                        			 p1, piece_index, vert, horiz);
                    }
                }
            }
        }
    }
}

void CheckCapture(char piece_type[],
                  game_parameters *gp1,
                  square_attributes sa1[gp1->board_dimensions]
                                       [gp1->board_dimensions],
                  selector s1,
                  piece p1[gp1->number_of_pieces],
                  int piece_index,
                  int vert,
                  int horiz)
{
    if(strcmp(piece_type, "man") == 0)
    {
        for(int piece_2_index = 0;
            piece_2_index < gp1->number_of_pieces; ++piece_2_index)
        {
            if(p1[piece_2_index].alive == true
               && sa1[vert-1][horiz+1].x == p1[piece_2_index].pos.x
               && sa1[vert-1][horiz+1].y == p1[piece_2_index].pos.y
               && p1[piece_2_index].colour == !p1[piece_index].colour
               && sa1[vert-2][horiz+2].empty == true)
            {
                if(vert > 1 && horiz < gp1->board_dimensions - 2)
                {
                    if(gp1->prev_game_status_indicator != gsi_capturing_mandatory)
                    {
                        gp1->game_status_indicator = gsi_capturing_mandatory;
                    }

                    p1[piece_index].wtc.up_right = true;
                    p1[piece_index].can_capture = true;
                }
            }
            
            if(p1[piece_2_index].alive == true
               && sa1[vert-1][horiz-1].x == p1[piece_2_index].pos.x
               && sa1[vert-1][horiz-1].y == p1[piece_2_index].pos.y
               && p1[piece_2_index].colour == !p1[piece_index].colour
               && sa1[vert-2][horiz-2].empty == true)
            {
                if(vert > 1 && horiz > 1)
                {
                    if(gp1->prev_game_status_indicator != gsi_capturing_mandatory)
                    {
                        gp1->game_status_indicator = gsi_capturing_mandatory;
                    }

                    p1[piece_index].wtc.up_left = true;
                    p1[piece_index].can_capture = true;
                }
            }
            
            if(p1[piece_2_index].alive == true 
               && sa1[vert+1][horiz+1].x == p1[piece_2_index].pos.x
               && sa1[vert+1][horiz+1].y == p1[piece_2_index].pos.y
               && p1[piece_2_index].colour == !p1[piece_index].colour
               && sa1[vert+2][horiz+2].empty == true)
            {
                if(vert < gp1->board_dimensions - 2 && horiz < gp1->board_dimensions - 2)
                {
                    if(gp1->prev_game_status_indicator != gsi_capturing_mandatory)
                    {
                        gp1->game_status_indicator = gsi_capturing_mandatory;
                    }

                    p1[piece_index].wtc.down_right = true;
                    p1[piece_index].can_capture = true;
                }
            }
            
            if(p1[piece_2_index].alive == true
               && sa1[vert+1][horiz-1].x == p1[piece_2_index].pos.x
               && sa1[vert+1][horiz-1].y == p1[piece_2_index].pos.y
               && p1[piece_2_index].colour == !p1[piece_index].colour
               && sa1[vert+2][horiz-2].empty == true)
            {
                if(vert < gp1->board_dimensions - 2 && horiz > 1)
                {
                    if(gp1->prev_game_status_indicator != gsi_capturing_mandatory)
                    {
                        gp1->game_status_indicator = gsi_capturing_mandatory;
                    }

                    p1[piece_index].wtc.down_left = true;
                    p1[piece_index].can_capture = true;
                }
            }
        }
    }

    if(strcmp(piece_type, "king") == 0)
    {
        int square_iterator = 1;
        while(1)
        {
            if(vert - square_iterator >= 1
                && horiz + square_iterator < gp1->board_dimensions - 1)
            {
                for(int piece_2_index = 0;
                    piece_2_index < gp1->number_of_pieces;
                    ++piece_2_index)
                {
                    if(p1[piece_2_index].alive == true
                       && sa1[vert-square_iterator][horiz+square_iterator].x
                       == p1[piece_2_index].pos.x
                       && sa1[vert-square_iterator][horiz+square_iterator].y
                       == p1[piece_2_index].pos.y)
                    {
                        if(p1[piece_2_index].colour == !p1[piece_index].colour)
                        {
                            int square_iterator_2 = 1;
                            while(1)
                            {
                                if(vert - square_iterator - square_iterator_2
                                   >= 0
                                   && horiz + square_iterator
                                   + square_iterator_2 < gp1->board_dimensions)
                                {
                                    if(sa1[vert-square_iterator
                                    	   -square_iterator_2]
                                          [horiz+square_iterator
                                           +square_iterator_2].empty == true)
                                    {
                                        if(gp1->prev_game_status_indicator 
                                           != gsi_capturing_mandatory)
                                        {
                                            gp1->game_status_indicator
                                            = gsi_capturing_mandatory;
                                        }

                                    p1[piece_index].wtc.up_right = true;
                                    p1[piece_index].can_capture = true;
                                    break;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                                ++square_iterator_2;
                            }
                        }
                        else
                        {
                            p1[piece_index].other_on_the_way = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                break;
            }
            ++square_iterator;

            if(p1[piece_index].other_on_the_way == true)
            {
                break;
            }
        }
            
        square_iterator = 1;
        while(1)
        {
            if(vert - square_iterator >= 1 && horiz - square_iterator >= 1)
            {
                for(int piece_2_index = 0;
                    piece_2_index < gp1->number_of_pieces;
                    ++piece_2_index)
                {
                    if(p1[piece_2_index].alive == true
                       && sa1[vert-square_iterator][horiz-square_iterator].x
                       == p1[piece_2_index].pos.x
                       && sa1[vert-square_iterator][horiz-square_iterator].y
                       == p1[piece_2_index].pos.y)
                    {
                        if(p1[piece_2_index].colour == !p1[piece_index].colour)
                        {
                            int square_iterator_2 = 1;
                            while(1)
                            {
                                if(vert - square_iterator - square_iterator_2
                                   >= 0
                                   && horiz - square_iterator
                                   - square_iterator_2 >= 0)
                                {
                                    if(sa1[vert-square_iterator
                                    	   -square_iterator_2]
                                          [horiz-square_iterator
                                           -square_iterator_2].empty == true)
                                    {
                                        if(gp1->prev_game_status_indicator
                                           != gsi_capturing_mandatory)
                                        {
                                            gp1->game_status_indicator
                                            = gsi_capturing_mandatory;
                                        }

                                    p1[piece_index].wtc.up_left = true;
                                    p1[piece_index].can_capture = true;
                                    break;
                                    }
                                }
                                else
                                {
                                    break;
                                }   
                                ++square_iterator_2;
                            }
                        }
                        else
                        {
                            p1[piece_index].other_on_the_way = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                break;
            }
            ++square_iterator;

            if(p1[piece_index].other_on_the_way == true)
            {
                break;
            }
        }
            
        square_iterator = 1;
        while(1)
        {
            if(vert + square_iterator < gp1->board_dimensions - 1
               && horiz + square_iterator < gp1->board_dimensions - 1)
            {
                for(int piece_2_index = 0;
                    piece_2_index < gp1->number_of_pieces;
                    ++piece_2_index)
                {
                    if(p1[piece_2_index].alive == true
                       && sa1[vert+square_iterator][horiz+square_iterator].x
                       == p1[piece_2_index].pos.x
                       && sa1[vert+square_iterator][horiz+square_iterator].y
                       == p1[piece_2_index].pos.y)
                    {
                        if(p1[piece_2_index].colour == !p1[piece_index].colour)
                        {
                            int square_iterator_2 = 1;
                            while(1)
                            {
                                if(vert + square_iterator + square_iterator_2
                                   < gp1->board_dimensions
                                   && horiz + square_iterator
                                   + square_iterator_2 < gp1->board_dimensions)
                                {
                                    if(sa1[vert+square_iterator
                                    	   +square_iterator_2]
                                          [horiz+square_iterator
                                           +square_iterator_2].empty == true)
                                    {
                                        if(gp1->prev_game_status_indicator
                                           != gsi_capturing_mandatory)
                                        {
                                            gp1->game_status_indicator
                                            = gsi_capturing_mandatory;
                                        }

                                    p1[piece_index].wtc.down_right = true;
                                    p1[piece_index].can_capture = true;
                                    break;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                                ++square_iterator_2;
                            }
                        }
                        else
                        {
                            p1[piece_index].other_on_the_way = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                break;
            }
            ++square_iterator;

            if(p1[piece_index].other_on_the_way == true)
            {
                break;
            }
        }
        
        square_iterator = 1;
        while(1)
        {
            if(vert + square_iterator < gp1->board_dimensions - 1
               && horiz - square_iterator >= 1)
            {
                for(int piece_2_index = 0;
                    piece_2_index < gp1->number_of_pieces;
                    ++piece_2_index)
                {
                    if(p1[piece_2_index].alive == true
                       && sa1[vert+square_iterator][horiz-square_iterator].x
                       == p1[piece_2_index].pos.x
                       && sa1[vert+square_iterator][horiz-square_iterator].y
                       == p1[piece_2_index].pos.y)
                    {
                        if(p1[piece_2_index].colour == !p1[piece_index].colour)
                        {
                            int square_iterator_2 = 1;
                            while(1)
                            {
                                if(vert + square_iterator + square_iterator_2
                                   < gp1->board_dimensions
                                   && horiz - square_iterator
                                   - square_iterator_2 >= 0)
                                {
                                    if(sa1[vert+square_iterator
                                    	   +square_iterator_2]
                                          [horiz-square_iterator
                                           -square_iterator_2].empty == true)
                                    {
                                        if(gp1->prev_game_status_indicator
                                           != gsi_capturing_mandatory)
                                        {
                                            gp1->game_status_indicator
                                            = gsi_capturing_mandatory;
                                        }

                                    p1[piece_index].wtc.down_left = true;
                                    p1[piece_index].can_capture = true;
                                    break;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                                ++square_iterator_2;
                            }
                        }
                        else
                        {
                            p1[piece_index].other_on_the_way = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                break;
            }
            ++square_iterator;

            if(p1[piece_index].other_on_the_way == true)
            {
                break;
            }
        }
    }
}

void capture(game_parameters *gp1,
             size_of_board sob1,
             square_attributes sa1[gp1->board_dimensions]
                                  [gp1->board_dimensions],
             piece p1[gp1->number_of_pieces])
{
    for (int vert = 0; vert < gp1->board_dimensions; ++vert)
    {
        for (int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
        {
            for (int piece_index = 0; piece_index < gp1->number_of_pieces;
            	 ++piece_index)
            {
                if(p1[piece_index].alive == true
                   && p1[piece_index].if_append == false
                   && sa1[vert][horiz].x == p1[piece_index].pos.x
                   && sa1[vert][horiz].y == p1[piece_index].pos.y
                   && p1[piece_index].pos.prev_x != p1[piece_index].pos.x
                   && p1[piece_index].pos.prev_y != p1[piece_index].pos.y
                   && p1[piece_index].can_capture == true)
                {
                    if(p1[piece_index].king == false)
                    {
                        CapturePiece("man", gp1, sob1, sa1,
                        			 p1, piece_index, vert, horiz);
                    }
                    else
                    {
                    	CapturePiece("king", gp1, sob1, sa1,
                    				 p1, piece_index, vert, horiz);
                    }
                }
            }
        }
    }
}

void CapturePiece(char piece_type[],
				  game_parameters *gp1,
				  size_of_board sob1,
				  square_attributes sa1[gp1->board_dimensions]
				                       [gp1->board_dimensions],
				  piece p1[gp1->number_of_pieces],
				  int piece_index,
				  int vert,
				  int horiz)
{
	if(strcmp(piece_type, "man") == 0)
	{
		for (int piece_2_index = 0; piece_2_index < gp1->number_of_pieces;
		     ++piece_2_index)
		{
		    if(p1[piece_2_index].alive == true
		       && p1[piece_index].pos.x - (p1[piece_index].pos.x - p1[piece_index].pos.prev_x) / 2
		       == p1[piece_2_index].pos.x
		       && p1[piece_index].pos.y - (p1[piece_index].pos.y - p1[piece_index].pos.prev_y) / 2
		       == p1[piece_2_index].pos.y)
		    {
		        p1[piece_2_index].alive = false;
		        p1[piece_index].captured = true;
		        gp1->game_status_indicator = gsi_nothing;

		        for(int piece_3_index = 0;
		        	piece_3_index < gp1->number_of_pieces; ++piece_3_index)
		        {
		            p1[piece_3_index].wtc.up_left = false;
		            p1[piece_3_index].wtc.up_right = false;
		            p1[piece_3_index].wtc.down_left = false;
		            p1[piece_3_index].wtc.down_right = false;
		            p1[piece_3_index].can_capture = false;
		        }

		        for(int vert = 0; vert < gp1->board_dimensions; ++vert)
		        {
		            for (int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
		            {
		                sa1[vert][horiz].can_move = false;
		            }
		        }
		    }
		}
	}
	
	if(strcmp(piece_type, "king") == 0)
	{
		int x = (p1[piece_index].pos.prev_x-p1[piece_index].pos.x)
				 / sob1.square_width,
		    y = (p1[piece_index].pos.prev_y-p1[piece_index].pos.y)
		    	 / sob1.square_height;
		for(int square_iterator = 1;
		    square_iterator < abs( (p1[piece_index].pos.prev_x
		    						- p1[piece_index].pos.x)
		    						/ sob1.square_width );
		    ++square_iterator)
		{
		    for(int piece_2_index = 0; piece_2_index < gp1->number_of_pieces;
		        ++piece_2_index)
		    {
		        if(p1[piece_2_index].alive == true
		           && sa1[vert+square_iterator*SIGN(y)]
		                 [horiz+square_iterator*SIGN(x)].x
		                 == p1[piece_2_index].pos.x
		           && sa1[vert+square_iterator*SIGN(y)]
		                 [horiz+square_iterator*SIGN(x)].y
		                 == p1[piece_2_index].pos.y)
		        {
		            p1[piece_2_index].alive = false;
		            p1[piece_index].captured = true;
		            gp1->game_status_indicator = gsi_nothing;

		            for(int piece_3_index = 0;
		            	piece_3_index < gp1->number_of_pieces; ++piece_3_index)
		            {
		                p1[piece_3_index].wtc.up_left = false;
		                p1[piece_3_index].wtc.up_right = false;
		                p1[piece_3_index].wtc.down_left = false;
		                p1[piece_3_index].wtc.down_right = false;
		                p1[piece_3_index].can_capture = false;
		                p1[piece_3_index].other_on_the_way = false;
		            }

		            for(int vert = 0; vert < gp1->board_dimensions; ++vert)
		            {
		                for (int horiz = 0; horiz < gp1->board_dimensions;
		                	 ++horiz)
		                {
		                    sa1[vert][horiz].can_move = false;
		                }
		            }
		        }
		    }
		}
	}
}

void check_for_kings(game_parameters gp1, 
                     size_of_board sob1,
                     piece p1[gp1.number_of_pieces])
{
    for (int piece_index = 0; piece_index < gp1.number_of_pieces;
        ++piece_index)
    {
        if(p1[piece_index].alive == true && p1[piece_index].king == false
           && p1[piece_index].if_append == false)
        {
            if(p1[piece_index].colour == b_WHITE
               && p1[piece_index].pos.y == sob1.border_y + 1
               && p1[piece_index].can_capture == false
               && p1[piece_index].pos.prev_x != p1[piece_index].pos.x
               && p1[piece_index].pos.prev_y != p1[piece_index].pos.y)
            {
                p1[piece_index].king = true;
            }
            else if(p1[piece_index].colour == b_BLACK
                    && p1[piece_index].pos.y == sob1.rows - sob1.square_height
                    && p1[piece_index].can_capture == false
                    && p1[piece_index].pos.prev_x != p1[piece_index].pos.x
                    && p1[piece_index].pos.prev_y != p1[piece_index].pos.y)
            {
                p1[piece_index].king = true;
            }
        }
    }
}

void change_turns(game_parameters *gp1,
                  piece p1[gp1->number_of_pieces])
{
    for(int piece_index = 0; piece_index < gp1->number_of_pieces;
        ++piece_index)
    {
        if((p1[piece_index].pos.x != p1[piece_index].pos.prev_x
            || p1[piece_index].pos.y != p1[piece_index].pos.prev_y)
            && p1[piece_index].if_append == false
            && p1[piece_index].can_capture == false)
        {       
            gp1->whose_turn = !gp1->whose_turn;
            gp1->turn_changed = true;
            p1[piece_index].captured = false;
            break;
        }
        else if((p1[piece_index].pos.x != p1[piece_index].pos.prev_x
                 || p1[piece_index].pos.y != p1[piece_index].pos.prev_y)
                 && p1[piece_index].if_append == false
                 && p1[piece_index].can_capture == true
                 && p1[piece_index].captured == false)
        {
            gp1->whose_turn = !gp1->whose_turn;
            gp1->turn_changed = true;
            p1[piece_index].captured = false;

            gp1->game_status_indicator = gsi_nothing;

            for(int piece_2_index=0; piece_2_index<gp1->number_of_pieces;
                ++piece_2_index)
            {
                p1[piece_2_index].wtc.up_left = false;
                p1[piece_2_index].wtc.up_right = false;
                p1[piece_2_index].wtc.down_left = false;
                p1[piece_2_index].wtc.down_right = false;
                p1[piece_2_index].can_capture = false;
                p1[piece_2_index].other_on_the_way = false;
            }

            break;
        }
    }
}

void check_end_of_game(game_parameters *gp1,
                       size_of_board sob1,
                       square_attributes sa1[gp1->board_dimensions]
                                            [gp1->board_dimensions],
                       piece p1[gp1->number_of_pieces])
{
    int number_of_white = 0, number_of_black = 0;
    for(int piece_index = 0; piece_index < gp1->number_of_pieces;
        ++piece_index)
    {
        if(p1[piece_index].alive == true)
        {
            switch(p1[piece_index].colour)
            {
                case b_WHITE:
                    ++number_of_white;
                    break;

                case b_BLACK:
                    ++number_of_black;
                    break;

                default:
                    break;
            }
        }
    }

    if(number_of_white == 0)
    {
        gp1->end_of_game = true;
        gp1->who_won = b_BLACK;
    }

    if(number_of_black == 0)
    {
    	gp1->end_of_game = true;
        gp1->who_won = b_WHITE;
    }
}

void end_of_game(game_parameters *gp1)
{
	char who_won[] = "Placeholder";

	if(gp1->who_won == b_WHITE)
	{
		strcpy(who_won, "White");
	}
	else
	{
		strcpy(who_won, "Black");
	}

	printf("\33c\e[3J");
	printf("%s won! Would you like to play once again?\n", who_won);
	printf("1) Yes\n");
	printf("2) No\n");

    char choice = '0';
    choice = getch();

    switch(choice)
    {
        case '1':
            gp1->back_to_menu = true;
            break;
        case '2':  
            break;
        default:
            break;
    }
}


// game save/read

void game_save(game_parameters *gp1,
               square_attributes sa1[gp1->board_dimensions]
                                    [gp1->board_dimensions],
               selector s1,
               piece p1[gp1->number_of_pieces])
{
    FILE *save = fopen("save.txt", "w");

    if(save == NULL)
    {
        gp1->sp1.save_error = true;
    }
    else
    {
        fprintf(save, "%d %d %d %d ",
                gp1->whose_turn,
                gp1->turn_changed,
                gp1->game_status_indicator,
                gp1->prev_game_status_indicator);

        for(int vert = 0; vert < gp1->board_dimensions; ++vert)
        {
            for (int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
            {
                fprintf(save, "%d %d %d %d ",
                        sa1[vert][horiz].can_move, sa1[vert][horiz].empty,
                        sa1[vert][horiz].x, sa1[vert][horiz].y);
            }
        }

        fprintf(save, "%d %d ", s1.pos_x, s1.pos_y);

        for(int piece_index = 0; piece_index < gp1->number_of_pieces;
            ++piece_index)
        {
            fprintf(save, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
                    p1[piece_index].alive, p1[piece_index].colour,
                    p1[piece_index].king, p1[piece_index].if_append,
                    p1[piece_index].can_capture, p1[piece_index].captured,
                    p1[piece_index].other_on_the_way, p1[piece_index].pos.x,
                    p1[piece_index].pos.y, p1[piece_index].pos.prev_x,
                    p1[piece_index].pos.prev_y, p1[piece_index].wtc.up_right,
                    p1[piece_index].wtc.up_left,
                    p1[piece_index].wtc.down_right,
                    p1[piece_index].wtc.down_left);
        }
    }

    fclose(save);
}

void load_game_save(game_parameters *gp1,
                    square_attributes sa1[gp1->board_dimensions]
                                         [gp1->board_dimensions],
                    selector *s1,
                    piece p1[gp1->number_of_pieces])
{
    FILE *save;
    save = fopen("save.txt", "r");

    // temporary variables for storing boolean data
    int gp1_temp1, gp1_temp2, gp1_temp3, gp1_temp4, gp1_temp5,
        gp1_temp6, gp1_temp7, gp1_temp8, gp1_temp9;
    int sa1_temp1, sa1_temp2, sa1_temp3, sa1_temp4;
    int s1_temp1, s1_temp2;
    int p1_temp1, p1_temp2, p1_temp3, p1_temp4, p1_temp5,
        p1_temp6, p1_temp7, p1_temp8, p1_temp9, p1_temp10,
        p1_temp11, p1_temp12, p1_temp13, p1_temp14, p1_temp15;

    if(save == NULL)
    {
        gp1->sp1.load_error = true;
    }
    else
    {
        fscanf(save, "%d %d %d %d ",
               &gp1_temp1, &gp1_temp2, &gp1_temp3, &gp1_temp4);

        gp1->whose_turn = gp1_temp1;
        gp1->turn_changed = gp1_temp2;
        gp1->game_status_indicator = gp1_temp3;
        gp1->prev_game_status_indicator = gp1_temp4;

        for(int vert = 0; vert < gp1->board_dimensions; ++vert)
        {
            for (int horiz = 0; horiz < gp1->board_dimensions; ++horiz)
            {
                fscanf(save, "%d %d %d %d ",
                       &sa1_temp1, &sa1_temp2, &sa1_temp3, &sa1_temp4);

                sa1[vert][horiz].can_move= sa1_temp1;
                sa1[vert][horiz].empty = sa1_temp2;
                sa1[vert][horiz].x = sa1_temp3;
                sa1[vert][horiz].y = sa1_temp4;
            }
        }

        fscanf(save, "%d %d ", &s1_temp1, &s1_temp2); 
            s1->pos_x = s1_temp1;
            s1->pos_y = s1_temp2;

        for(int piece_index = 0; piece_index < gp1->number_of_pieces;
            ++piece_index)
        {
            fscanf(save, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
                   &p1_temp1, &p1_temp2, &p1_temp3, &p1_temp4, &p1_temp5,
                   &p1_temp6, &p1_temp7, &p1_temp8, &p1_temp9, &p1_temp10,
                   &p1_temp11, &p1_temp12, &p1_temp13, &p1_temp14, &p1_temp15);

                    p1[piece_index].alive = p1_temp1;
                    p1[piece_index].colour = p1_temp2;
                    p1[piece_index].king = p1_temp3;
                    p1[piece_index].if_append = p1_temp4;
                    p1[piece_index].can_capture = p1_temp5;
                    p1[piece_index].captured = p1_temp6;
                    p1[piece_index].other_on_the_way = p1_temp7;
                    p1[piece_index].pos.x = p1_temp8;
                    p1[piece_index].pos.y = p1_temp9;
                    p1[piece_index].pos.prev_x = p1_temp10;
                    p1[piece_index].pos.prev_y = p1_temp11;
                    p1[piece_index].wtc.up_right = p1_temp12;
                    p1[piece_index].wtc.up_left = p1_temp13;
                    p1[piece_index].wtc.down_right = p1_temp14;
                    p1[piece_index].wtc.down_left = p1_temp15;
        }
    }

    fclose(save);
}


// useful functions

int append_check(game_parameters gp1,
                 piece p1[gp1.number_of_pieces])
{
    int append_checker = 0;
    for(int piece_index = 0; piece_index < gp1.number_of_pieces; ++piece_index)
    {
        if(p1[piece_index].alive == true && p1[piece_index].if_append == true)
        {
            append_checker = piece_index + 1;
            break;
        }
    }

    return append_checker;
}

void check_men_position(game_parameters gp1,
                        square_attributes sa1[gp1.board_dimensions]
                                             [gp1.board_dimensions],
                        piece p1[gp1.number_of_pieces])
{
    for(int vert = 0; vert < gp1.board_dimensions; ++vert)
    {
        for(int horiz = 0; horiz < gp1.board_dimensions; ++horiz)
        {
            sa1[vert][horiz].empty = true;
            for(int piece_index = 0; piece_index < gp1.number_of_pieces;
            	++piece_index)
            {
                if(p1[piece_index].pos.x == sa1[vert][horiz].x
                   && p1[piece_index].pos.y == sa1[vert][horiz].y
                   && p1[piece_index].alive == true)
                {
                    sa1[vert][horiz].empty = false;
                    break;
                }
            }
        }
    }
}

void restore_previous_position(game_parameters gp1,
                               piece p1[gp1.number_of_pieces])
{
    for (int piece_index = 0; piece_index < gp1.number_of_pieces;
         ++piece_index)
    {
        if(p1[piece_index].if_append == false)
        {
            p1[piece_index].pos.prev_x = p1[piece_index].pos.x;
            p1[piece_index].pos.prev_y = p1[piece_index].pos.y;
        }
    }
}