#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>	
#include <allegro5\allegro_native_dialog.h> 
#include "logic.h"

// Turn uses enum for easy global access
enum Turn { 
	X_TURN = 0, 
	O_TURN = 1
};

Turn turn = X_TURN; // Start with X's turn (player)

void set_graphics_x_o(int x, int y, logic &game_logic);
void draw_board();
void draw_x(int x, int y);
void draw_o(int x, int y);
void game_message(bool &gameover, logic &game_logic);
void turn_xo(int x, int y, int boardx, int boardy, logic  &game_logic);
bool computer_move_o(logic& game_logic);

int main(void)
{
	logic  game_logic;
	int posX = 0, posY = 0;
	bool gameover = false;
	ALLEGRO_DISPLAY *Screen = NULL;
	int width = 640, height = 480;

	if (!al_init())
	{
		al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}


	Screen = al_create_display(width, height);
	if (Screen == NULL)
	{
		al_show_native_message_box(Screen, "Error!", "Failed to create the display.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	if (!al_install_mouse()) {
		al_show_native_message_box(Screen, "Error!", "Failed to initialize the mouse!\n.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	bool draw = false, done = false;;

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	event_queue = al_create_event_queue();


	al_register_event_source(event_queue, al_get_display_event_source(Screen));
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_register_event_source(event_queue, al_get_mouse_event_source());

	game_logic.setup();
	draw_board();
	game_message(gameover, game_logic);

	al_flip_display();
	while (!done && !gameover)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				posX = ev.mouse.x;
				posY = ev.mouse.y;

				draw = true;
			}
		}
		draw_board();
		game_message(gameover, game_logic);

		// If player's turn, set X on the board
		if (draw && turn == X_TURN)
		{

			set_graphics_x_o(posX, posY, game_logic);

			draw = false;
		}

		// Let computer play when it's O's turn
		if (turn == O_TURN && !gameover) {
			if (computer_move_o(game_logic)) {
				turn = X_TURN;
				al_flip_display();
			}
		}

		al_flip_display();
	}
	al_rest(5.0);
	al_destroy_event_queue(event_queue);
	al_destroy_display(Screen);						//destroy our display object

	return 0;
}


/*
* Computer will check for available rows staring with top left row
* It will know what rows are taken but it has no startegy to win
* returns true if computer drew an O
*/
bool computer_move_o(logic& game_logic) {
	// Search for an empty cell to place O
	for (int row = 0; row < 3; ++row) {
		for (int col = 0; col < 3; ++col) {
			// If the cell is already occupied, skip it
			if (game_logic.set_o(row, col)) {
				int draw_x = 106 + 213 * col; // center x + cell width * column number
				int draw_y = 62 + 125 * row; // center y + cell height * row number
				draw_o(draw_x, draw_y);
				return true;
			}
		}
	}
	return false;
}

void draw_board()
{
	al_draw_line(0, 375, 640, 375, al_map_rgb(255, 0, 0), 2);
	al_draw_filled_rectangle(0, 376, 640, 480, al_map_rgb(200, 200, 200));

	al_draw_line(0, 125, 640, 125, al_map_rgb(255, 255, 255), 2);
	al_draw_line(0, 250, 640, 250, al_map_rgb(255, 255, 255), 2);
	al_draw_line(213, 0, 213, 375, al_map_rgb(255, 255, 255), 2);
	al_draw_line(426, 0, 426, 375, al_map_rgb(255, 255, 255), 2);


}
void draw_x(int x, int y)
{
	al_draw_line(x - 106, y - 62, x + 106, y + 62, al_map_rgb(255, 0, 0), 2);
	al_draw_line(x - 106, y + 62, x + 106, y - 62, al_map_rgb(255, 0, 0), 2);
}
void draw_o(int x, int y)
{
	al_draw_circle(x, y, 62, al_map_rgb(255, 255, 0), 4);

}
void turn_xo(int x, int y, int boardx, int boardy, logic  &game_logic)
{
	ALLEGRO_FONT *font = al_load_font("GROBOLD.ttf", 24, 0);
	if (turn == 0)
	{
		if (game_logic.set_x(boardx, boardy) == true)
		{
			draw_x(x, y);
			turn = O_TURN;
		}
	}
	else
	{
		if (game_logic.set_o(boardx, boardy) == true)
		{
			draw_o(x, y);
			turn = X_TURN;
		}
	}
}
void set_graphics_x_o(int x, int y, logic &game_logic)
{

	if ((x<213) && (y<125))
	{
		turn_xo(106, 62, 0, 0, game_logic);
	}
	else if ((x>213) && (x<426) && (y<125))
	{
		turn_xo(319, 62, 0, 1, game_logic);
	}
	else if ((x>426) && (y<125))
	{
		turn_xo(533, 62, 0, 2, game_logic);
	}
	else if ((x<213) && (y>125) && (y<250))
	{
		turn_xo(106, 186, 1, 0, game_logic);

	}
	else if ((x>213) && (x<426) && (y>125) && (y<250))
	{
		turn_xo(319, 186, 1, 1, game_logic);
	}
	else if ((x>426) && (y>125) && (y<250))
	{
		turn_xo(533, 186, 1, 2, game_logic);
	}
	else if ((x<213) && (y>250) && (y<375))
	{
		turn_xo(106, 314, 2, 0, game_logic);

	}
	else if ((x>213) && (x<426) && (y>250) && (y<375))
	{
		turn_xo(319, 314, 2, 1, game_logic);
	}
	else if ((x>426) && (y>250) && (y<375))
	{
		turn_xo(533, 314, 2, 2, game_logic);
	}
}

void game_message(bool &gameover, logic &game_logic)
{
	bool xwon = false, owon = false, tie = false;
	game_logic.done(tie, xwon, owon);
	ALLEGRO_FONT *font = al_load_font("GROBOLD.ttf", 24, 0);

	if (tie == true)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "The game is a tie -- screen will close shortly");
		gameover = true;
	}
	else if (xwon == true)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "X won the game-- screen will close shortly");
		gameover = true;
	}
	else if (owon == true)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "O won the game-- screen will close shortly");
		gameover = true;
	}
	else
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, 400, ALLEGRO_ALIGN_LEFT, "Pick a Square");

}


