#ifndef GAMELOGIC_H
#define GAMELOGIC_H

/******************************
* Klasse für die Spiele-Logik
* Autor: Sven Gebhard
* Start: 4.3.17
*/

#include<efi.h>
#include<efilib.h>

#include"Shape.h"

#define BALL_MOVE_SPEED_X 10
#define BALL_MOVE_SPEED_Y 10

/////////////
// Collider werden dazu verwendet, um zu prüfen ob sich der Ball an der Decke oder 
// dem Boden des Spielfelds befindet oder um zu prüfen ob ein Spieler den Ball
// durchgelassen hat
//
_Bound topCollider, bottomCollider, leftCollider, rightCollider;

/////////////
// Aktuell erzielte Scores der beiden Spieler
UINTN p1_score, p2_score;

//////////////////
// -- setupGame --
// Setup-Methode des Spiels
//
void setupGame(Shape *background);

///////////////////////
// -- setupColliders --
// Methode die die Werte der Collider setzt
//
void setupCollider(Shape *background);

//////////////////////
// -- manageBallCol --
// Methode die die Kollision des Balls abfängt und dessen Bewegungsvektor neu setzt
//
void manageBallCol(Shape *ball, Shape *background, Shape *pad_1, Shape *pad_2, Vector *ballMovVec);

////////////////////////
// -- managePaddleCol --
// Methode die die Kollision des Balls mit einem Paddle abfängt und dessen Bewegungs-
// vektor neu setzt
// @param xdir: -1 = left
//		  ydir:  1 = right
//
BOOLEAN managePaddleCol(Shape *pad, Shape *ball, Vector *ballMovVec, UINTN xdir);

//////////////////
// -- resetBall --
// Methode setzt den Ball auf die Anfangsposition zurück
//
void resetBall(Shape background, Shape *ball, Vector *moveVec);

#endif