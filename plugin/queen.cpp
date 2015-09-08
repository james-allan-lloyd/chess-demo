#include "queen.h"

#include <QDebug>

Queen::Queen(ChessBoardModel* parent)
    : Piece("Queen", parent)
{

}

bool Queen::isValidMove(QPoint a) const
{
    return validMoves_.contains(a);
}


void Queen::recalculateMoves()
{
    validMoves_.clear();

    projectMovement(-1, 0, validMoves_);  // horizontal left
    projectMovement(1, 0, validMoves_);   // horizontal right

    projectMovement(0, -1, validMoves_);  // vertical up
    projectMovement(0, 1, validMoves_);   // vertical down

    projectMovement(-1, -1, validMoves_); // left, top
    projectMovement(-1, 1, validMoves_);  // left, bottom
    projectMovement(1, -1, validMoves_); // right, top
    projectMovement(1, 1, validMoves_);  // right, bottom

    Q_ASSERT(!validMoves_.contains(position()));
}
