#ifndef SHAPES_H
#define SHAPES_H

/******************************
* Shape Class
* Author: Sven Gebhard
* Begin: 13.12.16
*/

#include<efi.h>
#include<efilib.h>

/////////////////////////
// -- Bound structure --
struct Bound {
	UINTN top;    // x-Position
	UINTN left;   // y-Position
	UINTN width;  // width  of the object
	UINTN height; // height of the object
};

/////////////////////////
// -- Vector structure --
typedef struct Vector {
	UINTN x; // x-Value
	UINTN y; // y-Value
};

//////////////////////////
// -- Shape structure --
typedef struct Shape {
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL p_shape; // Color of the paddle
	struct Bound bound; // Bounds of the paddle 
};

/////////////////////
// -- createShape --
// Methode mit der man einen Shape erzeugen kann
// @param shape: Shape dessen Eigenschaften verändert werden sollen
// @param %etc%: Eigenschaften des Shapes
//
void createShape(struct Shape *shape, UINTN xPos, UINTN yPos, UINTN width, UINTN height, UINTN p_green, UINTN p_blue, UINTN p_red);

///////////////////
// -- drawShape --
// Methode mit der ein Shape gezeichnet werden kann
//
void drawShape(struct Shape *shape, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);

//////////////////////////
// -- setShapePosition --
// Methode mit der man die Position eines Shapes verändern kann (Änderungen sind nach dem Aufruf dieser Funktion nicht sichtbar,
// da der Shape nicht ernuet gezeichnet wird)
//
void setShapePosition(struct Shape *shape, UINTN posX, UINTN posY);

///////////////////
// -- moveShape --
// Methode mit der man einen Shape bewegen kann (Änderungen sind nach dem Aufruf dieser Funktion nicht sichtbar,
// da der Shape nicht ernuet gezeichnet wird)
//
void moveShape(struct Shape *shape, INT32 amountX, INT32 amountY);

////////////////////
// -- moveShape_r --
// Methode mit der man einen Shape bewegen kann UND diesen Shape gleichzeitig erneut zeichnen kann
//
void moveShape_r(struct Shape *shape, struct Shape *background, INT32 amountX, INT32 amountY, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);

//////////////////////////
// -- checkForCollision --
// Methode prüft ob b1 und b2 kollidieren
//
BOOLEAN checkForCollision_b(struct Bound *b1, struct Bound *b2);

//////////////////////////
// -- checkForCollision --
// Methode prüft ob die beiden Rechtecke kollidieren
//
BOOLEAN checkForCollision(UINTN x1, UINTN y1, UINTN w1, UINTN h1,
						  UINTN x2, UINTN y2, UINTN w2, UINTN h2);

#endif