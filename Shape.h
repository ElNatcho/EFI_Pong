/******************************
* Shape Class
* Author: Sven Gebhard
* Begin: 13.12.16
*/

#include<efi.h>
#include<efilib.h>

///////////////////////////
//// -- Bound structure --
//typedef struct Bound{
//	UINTN top;    // x-Position
//	UINTN left;   // y-Position
//	UINTN width;  // width  of the object
//	UINTN height; // height of the object
//} _Bound;
//
///////////////////////////
//// -- Vector structure --
//struct Vector{
//	UINTN x; // x-Value
//	UINTN y; // y-Value
//} _Vector;
//
////////////////////////////////////////////////////////////////////// -- Paddle -- 
//
////////////////////////////
//// -- Paddle structure --
//struct Shape {
//	EFI_GRAPHICS_OUTPUT_BLT_PIXEL p_shape; // Color of the paddle
//	_Bound bound; // Bounds of the paddle 
//} _Shape;