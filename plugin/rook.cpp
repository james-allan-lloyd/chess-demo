#include "rook.h"

Rook::Rook(ChessBoardModel* parent)
    : Piece("Rook", parent)
{

}

bool Rook::isValidMove(QPoint a) const
{
    return validMoves_.contains(a);
}

void Rook::recalculateMoves()
{
    validMoves_.clear();
    projectMovement(-1, 0, validMoves_);
    projectMovement(1, 0, validMoves_);  // horizontal
    projectMovement(0, -1, validMoves_);
    projectMovement(0, 1, validMoves_);  // vertical
}

