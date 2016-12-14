/***************************
*    PONG FOR UEFI
*	 Autor: Sven Gebhard
*    Begin_Date: 27.11.16 17:30
*/

#include<efi.h>
#include<efilib.h>

/////////////////////////
// -- Bound structure --
typedef struct {
	UINTN top;    // x-Position
	UINTN left;   // y-Position
	UINTN width;  // width  of the object
	UINTN height; // height of the object
} Bound;

/////////////////////////
// -- Vector structure --
typedef struct {
	UINTN x; // x-Value
	UINTN y; // y-Value
} Vector;

//////////////////////////////////////////////////////////////////// -- Paddle -- 

//////////////////////////
// -- Paddle structure --
typedef struct {
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL p_shape; // Color of the paddle
	Bound bound; // Bounds of the paddle 
} Shape;

///////////////////
// create the paddle
void createShape(Shape *shape, UINTN xPos, UINTN yPos, UINTN width, UINTN height, UINTN p_green, UINTN p_blue, UINTN p_red) {
	shape->bound.top = yPos;        // Set y-Position of the paddle
	shape->bound.left = xPos;       // Set x-Position of the paddle
	shape->bound.width = width;     // Set the width of the paddle
	shape->bound.height = height;   // Set the height of the paddle
	shape->p_shape.Red = p_red;     // Set the red color value of the paddle
	shape->p_shape.Blue = p_blue;   // Set the blue color value of the paddle
	shape->p_shape.Green = p_green; // Set the green color value of the paddle
}

///////////////////
// draw a shape
void drawShape(Shape *shape, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
	gop->Blt(gop, &shape->p_shape, EfiBltVideoFill, 0, 0, shape->bound.left, shape->bound.top, shape->bound.width, shape->bound.height, 0); // draw the paddle 
}

//////////////////////////////
// set of a shape
void setShapePosition(Shape *shape, UINTN posX, UINTN posY) {
	shape->bound.top  = posY;
	shape->bound.left = posX;
}

///////////////////
// move a shape
void moveShape(Shape *shape, INT32 amountX, INT32 amountY) {
	setShapePosition(shape, shape->bound.left + amountX, shape->bound.top + amountY);
}

//////////////////////////////
// move and redraw a shape
void moveShape_r(Shape *shape, Shape *background, INT32 amountX, INT32 amountY, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
	gop->Blt(gop, &background->p_shape, EfiBltVideoFill, 0, 0, shape->bound.left, shape->bound.top, shape->bound.width, shape->bound.height, 0);
	setShapePosition(shape, shape->bound.left + amountX, shape->bound.top + amountY);
	drawShape(shape, gop);
}

////////////////////////////////////////////////////////////////////

/////////////////////
// -- MAIN_METHOD --
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	// -- VARS --
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	EFI_INPUT_KEY key;
	INT32 index = 0;

	BOOLEAN gameBegan = FALSE;

	Shape pad_1; // Paddle of the first player
	Shape pad_2; // Paddle of the second player
	Shape background; // Background

	Shape ball;  // Ball
	Vector ballMoveVector; // Move vector of the ball
	ballMoveVector.x = 0;
	ballMoveVector.y = 0;

	createShape(&ball, 395, 295, 10, 10, 255, 0, 0); // Create the ball
	createShape(&pad_1, 10 , 10, 20, 200, 0, 255,   0); // Create the paddle of player 1
	createShape(&pad_2, 770, 10, 20, 200, 0, 0  , 255); // Create the paddle of player 2
	createShape(&background, 0, 0, 800, 600, 128, 128, 128); // Create background

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

		if (key.ScanCode != SCAN_NULL) { // CHECK FOR INPUT OF PLAYER_1 (pad_1) AND QUIT EVENT
			if (key.ScanCode == SCAN_UP && pad_1.bound.top > 0) { // If up is pressed move pad_1 up
				moveShape_r(&pad_1, &background, 0, -5, gop);
			} else if (key.ScanCode == SCAN_DOWN && (pad_1.bound.top + pad_1.bound.height) < 600) { // If down is pressed move pad_1 down
				moveShape_r(&pad_1, &background, 0, 5, gop);
			} else if (key.ScanCode == SCAN_ESC) { // If esc is pressed quit the game
				break;
			}
		} else { // CHECK FOR INPUT OF PLAYER_2 (pad_2)
			if (key.UnicodeChar == 'w' || key.UnicodeChar == 'W' && pad_2.bound.top > 0) { // If w is pressed move pad_2 up
				moveShape_r(&pad_2, &background, 0, -5, gop);
			} else if (key.UnicodeChar == 's' || key.UnicodeChar == 'S' && pad_1.bound.top + pad_1.bound.height < 600) { // If s is pressed move pad_2 down
				moveShape_r(&pad_2, &background, 0, 5, gop);
			} else if (key.UnicodeChar == ' ' && gameBegan != TRUE) {
				gameBegan = FALSE;
				ballMoveVector.x = 1;
			}
		}

		if (ballMoveVector.x != 0 || ballMoveVector.y != 0) {
			moveShape_r(&ball, &background, ballMoveVector.x, ballMoveVector.y, gop); // MOVE THE BALL EVERY FRAME
		}


		//drawShape(&background, gop);
		//drawShape(&pad_1, gop);
		//drawShape(&pad_2, gop);
	}

	systab->ConOut->ClearScreen(systab->ConOut);

	return EFI_SUCCESS;
}