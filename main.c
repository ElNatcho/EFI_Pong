/***************************
*    PONG FOR UEFI
*	 Autor: Sven Gebhard
*    Begin_Date: 27.11.16 17:30
*/

#include<efi.h>
#include<efilib.h>

#include"Shapes.h"
#include"GameLogic.h"

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

/////////////////////
// -- MAIN_METHOD --
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	// -- VARS --
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_INPUT_KEY key;
	INT32 index = 0;

	BOOLEAN gameBegan = FALSE;

	struct Shape pad_1; // Paddle of the first player
	struct Shape pad_2; // Paddle of the second player
	struct Shape background; // Background

	struct Shape  ball;  // Ball
	struct Vector ballMoveVector; // Move vector of the ball
	ballMoveVector.x = 0;
	ballMoveVector.y = 0;

	createShape(&ball, 395, 295, 10, 10, 255, 0, 0); // Create the ball
	createShape(&pad_1, 10 , 10, 20, 200, 0, 255,   0); // Create the paddle of player 1
	createShape(&pad_2, 770, 150, 20, 200, 0, 0  , 255); // Create the paddle of player 2
	createShape(&background, 0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 128, 128, 128); // Create background

	InitializeLib(image, systab);

	systab->ConOut->ClearScreen(systab->ConOut);  // Clear the screen

	LibLocateProtocol(&GraphicsOutputProtocol, (VOID**)&gop);

	drawShape(&background, gop); // Draw the background of the screen
	drawShape(&pad_1, gop); // Draw the pad of the first player
	drawShape(&pad_2, gop); // Draw the pad of the second player
	drawShape(&ball, gop);  // Draw the ball

	// -- MAIN_PROGRAM_LOOP --
	while (1) {

		// -- CHECK FOR KEYBOARD EVENTS --
		//systab->BootServices->WaitForEvent(1, &systab->ConIn->WaitForKey, &index);
		systab->BootServices->CheckEvent(systab->ConIn->WaitForKey);
		systab->ConIn->ReadKeyStroke(systab->ConIn, &key);

		// CHECK FOR INPUT OF PLAYER_1 (pad_1) AND QUIT EVENT
		if (key.ScanCode == SCAN_UP && pad_1.bound.top > 0) { // If up is pressed move pad_1 up
			moveShape_r(&pad_1, &background, 0, -5, gop);
		} else if (key.ScanCode == SCAN_DOWN && (pad_1.bound.top + pad_1.bound.height) < 600) { // If down is pressed move pad_1 down
			moveShape_r(&pad_1, &background, 0, 5, gop);
		} else if (key.ScanCode == SCAN_ESC) { // If esc is pressed quit the game
			break;
		}
		// CHECK FOR INPUT OF PLAYER_2 (pad_2)
		if (key.UnicodeChar == 'w' || key.UnicodeChar == 'W' && pad_2.bound.top > 100) { // If w is pressed move pad_2 up
			moveShape_r(&pad_2, &background, 0, -5, gop);
		} else if (key.UnicodeChar == 's' || key.UnicodeChar == 'S' && (pad_1.bound.top + pad_1.bound.height) < 600) { // If s is pressed move pad_2 down
			moveShape_r(&pad_2, &background, 0, 5, gop);
		} else if (key.UnicodeChar == ' ' && gameBegan != TRUE) {
			gameBegan = FALSE;
			ballMoveVector.x = -1;
		}	

		//if (checkForCollision_b(&pad_1.bound, &ball.bound) == FALSE &&
		//	checkForCollision_b(&pad_2.bound, &ball.bound) == FALSE && 
		//	checkForCollision(ball.bound.left, ball.bound.top, ball.bound.width, ball.bound.height,
		//					  0, 0, SCREEN_SIZE_X - 10, SCREEN_SIZE_Y - 10) == TRUE) {
		//	moveShape_r(&ball, &background, ballMoveVector.x, ballMoveVector.y, gop); // MOVE THE BALL EVERY FRAME
		//}

		manageBallCol(&ball, &background, &pad_1, &pad_2, &ballMoveVector);
		moveShape_r(&ball, &background, ballMoveVector.x, ballMoveVector.y, gop);

		//drawShape(&background, gop);
		//drawShape(&pad_1, gop);
		//drawShape(&pad_2, gop);
	}

	systab->ConOut->ClearScreen(systab->ConOut);

	return EFI_SUCCESS;
}