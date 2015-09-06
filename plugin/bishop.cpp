#include "bishop.h"

Bishop::Bishop(ChessBoardModel* parent)
    : Piece("Bishop", parent)
{

}

bool Bishop::isValidMove(QPoint a) const
{
    return validMoves_.contains(a);
}

void Bishop::recalculateMoves()
{
    validMoves_.clear();
    projectMovement(-1, -1, validMoves_); // left top
    projectMovement(-1, 1, validMoves_);  // left bottom
    projectMovement(1, -1, validMoves_);  // right top
    projectMovement(1, 1, validMoves_);   // right bottom
}
