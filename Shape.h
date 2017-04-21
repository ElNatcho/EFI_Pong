/******************************
* Shape Class
* Author: Sven Gebhard
* Begin: 13.12.16
*/

#include<efi.h>
#include<efilib.h>

/////////////////////////
// -- Bound structure --
typedef struct Bound{
	UINTN top;    // x-Position
	UINTN left;   // y-Position
	UINTN width;  // width  of the object
	UINTN height; // height of the object
} _Bound;

/////////////////////////
// -- Vector structure --
typedef struct _Vector{
	UINTN x; // x-Value
	UINTN y; // y-Value
} Vector;

//////////////////////////////////////////////////////////////////// -- Paddle -- 

//////////////////////////
// -- Paddle structure --
typedef struct _Shape {
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL p_shape; // Color of the paddle
	_Bound bound; // Bounds of the paddle 
} Shape;