#include "bishop.h"

Bishop::Bishop(ChessBoardModel* parent)
    : Piece("Bishop", parent)
{
}

QSet<QPoint> Bishop::recalculateMoves()
{
    QSet<QPoint> result;
    projectMovement(-1, -1, result); // left top
    projectMovement(-1, 1, result);  // left bottom
    projectMovement(1, -1, result);  // right top
    projectMovement(1, 1, result);   // right bottom
    return result;
}
