#include "rook.h"

Rook::Rook(ChessBoardModel* parent)
    : Piece("Rook", parent)
{

}

QSet<QPoint> Rook::recalculateMoves()
{
    QSet<QPoint> result;
    projectMovement(-1, 0, result);
    projectMovement(1, 0, result);  // horizontal
    projectMovement(0, -1, result);
    projectMovement(0, 1, result);  // vertical
    return result;
}

