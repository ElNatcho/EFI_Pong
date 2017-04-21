#include"GameLogic.h"

//////////////////
// -- setupGame --
//
void setupGame(Shape *background) {
	setupCollider(background);
	p1_score = 0;
	p2_score = 0;
}


//////////////////////
// -- setupCollider --
void setupCollider(Shape *background) {
	// Collider des linken Spielers aufsetzen
	leftCollider.left   = background->bound.left;
	leftCollider.top    = background->bound.top;
	leftCollider.width  = 10;
	leftCollider.height = background->bound.height;
	// Collider des rechten Spielers aufsetzen
	rightCollider.left   = background->bound.width - 10;
	rightCollider.top    = background->bound.top;
	rightCollider.width  = 10;
	rightCollider.height = background->bound.height;
	// Boden-Collider aufsetzen
	bottomCollider.left   = background->bound.left;
	bottomCollider.top    = background->bound.height - 10;
	bottomCollider.width  = background->bound.width;
	bottomCollider.height = 10;
	// Decken-Collider aufsetzen
	bottomCollider.left   = background->bound.left;
	bottomCollider.top    = background->bound.top;
	bottomCollider.width  = background->bound.width;
	bottomCollider.height = 10;
}

//////////////////////
// -- manageBallCol --
void manageBallCol(Shape *ball, Shape *background, Shape *pad_1, Shape *pad_2, Vector *ballMovVec) {
	// Testen ob der Ball noch auf dem Spielfeld ist
	if (checkForCollision(ball->bound.left, ball->bound.top, ball->bound.width, ball->bound.height,
						0, 0, background->bound.width - 10, background->bound.top - 10) == TRUE) {
		// Testen ob der Ball mit dem ersten Paddle kollidiert ist
		if (managePaddleCol(pad_1, ball, ballMovVec, 1)) {
			return;
		} // Testen ob der Ball mit dem zweiten Paddle kollidiert ist
		else if (managePaddleCol(pad_2, ball, ballMovVec, -1)) {
			return;
		} // Testen ob der Ball mit dem oberen oder unteren Collider kollidiert
		else if (checkForCollision_b(ball, bottomCollider) ||
			checkForCollision_b(ball, topCollider)) {
			ballMovVec->y *= -1;
			return;
		} 
		// -- Testen ob ein Spieler den Ball durchgelassen hat --
		// Testen ob der erste Spieler den Ball durchgelassen hat
		if (checkForCollision_b(&leftCollider, &ball)) {
			resetBall(*background, ball, ballMovVec);
			p2_score++;
		} // Testen ob der zweite Spieler den Ball durchgelassen hat
		else if (checkForCollision_b(&rightCollider, &ball)) {
			resetBall(*background, ball, ballMovVec);
			p1_score++;
		}


	}
}

////////////////////////
// -- managePaddleCol --
BOOLEAN managePaddleCol(Shape *pad, Shape *ball, Vector *ballMovVec, UINTN xdir) {
	UINTN tempH = pad->bound.height / 5;
	// Testen ob der Ball mit dem obersten Bereich des Paddles kollidiert
	if (checkForCollision(pad->bound.left, pad->bound.top, pad->bound.width, tempH,
		ball->bound.left, ball->bound.top, ball->bound.width, ball->bound.height)) {
		ballMovVec->x =  BALL_MOVE_SPEED_X * xdir;
		ballMovVec->y = -BALL_MOVE_SPEED_Y / 2;
		return TRUE;
	} // Testen ob der Ball mit dem zweitobersten Bereich des Paddles kollidiert
	else if (checkForCollision(pad->bound.left, pad->bound.top + tempH, pad->bound.width, tempH,
		     ball->bound.left, ball->bound.top, ball->bound.width, ball->bound.height)) {
		ballMovVec->x =  BALL_MOVE_SPEED_X * xdir;
		ballMovVec->y = -BALL_MOVE_SPEED_Y / 4;
		return TRUE;
	} // Testen ob der Ball mit dem mittleren Bereich des Paddles kollidiert
	else if (checkForCollision(pad->bound.left, pad->bound.top + 2 * tempH, pad->bound.width, tempH,
			 ball->bound.left, ball->bound.top, ball->bound.width, ball->bound.height)) {
		ballMovVec->x = BALL_MOVE_SPEED_X * xdir;
		ballMovVec->y = 0;
		return TRUE;
	} // Testen ob der Ball mit dem zweituntersten Bereich des Paddles kollidiert
	else if (checkForCollision(pad->bound.left, pad->bound.top + 3 * tempH, pad->bound.width, tempH,
			 ball->bound.left, ball->bound.top, ball->bound.width, ball->bound.height)) {
		ballMovVec->x = BALL_MOVE_SPEED_X * xdir;
		ballMovVec->y = BALL_MOVE_SPEED_Y / 4;
		return TRUE;
	} // Testen ob der Ball mit dem untersten Bereich des Paddles kollidiert
	else if (checkForCollision(pad->bound.left, pad->bound.top + 4 * tempH, pad->bound.width, tempH,
		ball->bound.left, ball->bound.top, ball->bound.width, ball->bound.height)) {
		ballMovVec->x = BALL_MOVE_SPEED_X * xdir;
		ballMovVec->y = -BALL_MOVE_SPEED_Y / 2;
		return TRUE;
	} else {
		return FALSE;
	}
}

//////////////////
// -- resetBall --
void resetBall(Shape background, Shape *ball, Vector *moveVec) {
	moveVec->x = 0;
	moveVec->y = 0;
	ball->bound.left   = background.bound.width  / 2 - ball->bound.width  / 2;
	ball->bound.height = background.bound.height / 2 - ball->bound.height / 2;
}