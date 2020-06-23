#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <ctime>

using namespace std;

array<int, 9> board {
	0,0,0,0,0,0,0,0,0
};
bool my_turn = true;

int position = 0;

void draw_board( ) {
	system( "cls" );
	for ( int n = 0; n < 9; n++ ) {
		if ( my_turn ) {
			if ( position == n ) printf( "*" );
			else if ( board.at( n ) == 0 ) printf( "-" );
			else if ( board.at( n ) == 1 ) printf( "o" );
			else printf( "x" );
			if ( n % 3 == 2 ) printf( "\n" );
		}
		else {
			if ( board.at( n ) == 0 ) printf( "-" );
			else if ( board.at( n ) == 1 ) printf( "o" );
			else printf( "x" );
			if ( n % 3 == 2 ) printf( "\n" );
		}
	}
};


void process_input( ) {
	if ( my_turn ) {
		if ( GetAsyncKeyState( VK_LEFT ) & 1 ) position--;
		else if ( GetAsyncKeyState( VK_RIGHT ) & 1 ) position++;
		position = clamp( position, 0, 8 );
		if ( GetAsyncKeyState( VK_RETURN ) & 1 && board.at(position)==0 ) {
			board [ position ] = 1;
			my_turn = false;
		}
	}
	else {
		vector<int> free_spots { };

		for ( int n = 0; n < 9;n++ ) {
			if ( board [ n ] == 0 ) {
				free_spots.push_back( n );
			}
		}
		srand( time(0) );
		const auto random = rand( );
		if(free_spots.size()) board [ free_spots [ random % free_spots.size( ) ] ] = 2;
		my_turn = true;
	}
	draw_board( );
};

int check_win( ) {
	vector<int> free_spots { };

	for ( int n = 0; n < 9; n++ ) {
		if ( board [ n ] == 0 ) {
			free_spots.push_back( n );
		}
	}

	if ( !free_spots.size( ) )
		return -1;

	for ( int n = 0; n < 3; n++ ) {
		int first = board.at(n * 3 + 0);
		int second = board.at(n * 3 + 1);
		int third = board.at(n * 3 + 2);
		if ( ( first + second + third ) % 3 == 0 && first != 0 && second != 0 && third != 0 ) return my_turn + 1;
	}
	for ( int n = 0; n < 3; n++ ) {
		int first = board.at( n );
		int second = board.at( n + 3 );
		int third = board.at( n + 6 );
		if ( ( first + second + third ) % 3 == 0 && first != 0 && second != 0 && third != 0 ) return my_turn + 1;
	}
	if ( ( board.at( 0 ) + board.at( 4 ) + board.at( 8 ) ) % 3 == 0 && board.at( 0 ) != 0 && board.at( 4 ) != 0 && board.at( 8 ) != 0 ) return my_turn + 1;
	if ( ( board.at( 2 ) + board.at( 4 ) + board.at( 6 ) ) % 3 == 0 && board.at( 2 ) != 0 && board.at( 4 ) != 0 && board.at( 6 ) != 0 ) return my_turn + 1;
	return 0;
}

/*
player always goes first => my_turn = true;
check for win => check_win();
if check_win() == true, then print out winner on screen (my_turn)
if check_win == false, then myturn = !myturn
if my_turn == true, then let player input
if my_turn == flase, then bot chooses random square that isn't taken and set it to x

*/

int main() {
	draw_board( );
	int who_won = 0;
	while ( true ) {
		process_input( );
		who_won = check_win( );
		if ( who_won != 0 ) break;
	}
	if ( who_won == -1 ) printf( "Tie!" );
	else if ( who_won == 1 ) printf( "Player has won!" );
	else printf( "Bot has won!" );
	system( "pause" );
}