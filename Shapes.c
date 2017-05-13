#include"Shapes.h"

/////////////////////
// -- createShape --
void createShape(struct Shape *shape, UINTN xPos, UINTN yPos, UINTN width, UINTN height, UINTN p_green, UINTN p_blue, UINTN p_red) {
	shape->bound.top = yPos;        // Set y-Position of the shape
	shape->bound.left = xPos;       // Set x-Position of the shape
	shape->bound.width = width;     // Set the width of the shape
	shape->bound.height = height;   // Set the height of the shape
	shape->p_shape.Red = p_red;     // Set the red color value of the shape
	shape->p_shape.Blue = p_blue;   // Set the blue color value of the shape
	shape->p_shape.Green = p_green; // Set the green color value of the shape
};

///////////////////
// -- drawShape --
void drawShape(struct Shape *shape, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
	gop->Blt(gop, &shape->p_shape, EfiBltVideoFill, 0, 0, shape->bound.left, shape->bound.top, shape->bound.width, shape->bound.height, 0); // draw the paddle 
}

/////////////////////////
// -- setShapePosition --
void setShapePosition(struct Shape *shape, UINTN posX, UINTN posY) {
	shape->bound.top  = posY;
	shape->bound.left = posX;
}

///////////////////
// -- moveShape --
void moveShape(struct Shape *shape, INT32 amountX, INT32 amountY) {
	setShapePosition(shape, shape->bound.left + amountX, shape->bound.top + amountY);
}

////////////////////
// -- moveShape_r --
void moveShape_r(struct Shape *shape, struct Shape *background, INT32 amountX, INT32 amountY, EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
	gop->Blt(gop, &background->p_shape, EfiBltVideoFill, 0, 0, shape->bound.left, shape->bound.top, shape->bound.width, shape->bound.height, 0);
	setShapePosition(shape, shape->bound.left + amountX, shape->bound.top + amountY);
	drawShape(shape, gop);
}

//////////////////////////
// -- checkForCollision --
BOOLEAN checkForCollision_b(struct Bound *b1, struct Bound *b2) {
	return checkForCollision(b1->left, b1->top, b1->width, b1->height,
		b2->left, b2->top, b2->width, b2->height);
}

//////////////////////////
// -- checkForCollision --
BOOLEAN checkForCollision(UINTN x1, UINTN y1, UINTN w1, UINTN h1,
					      UINTN x2, UINTN y2, UINTN w2, UINTN h2) {
	if (x1 < x2 + w2 &&
		x1 + w1 > x2 &&
		y1 < y2 + h2 &&
		y1 + h1 > y2) {
		return TRUE;
	} else {
		return FALSE;
	}
}