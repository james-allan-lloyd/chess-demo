#include "queen.h"

#include <QDebug>

Queen::Queen(ChessBoardModel* parent)
    : Piece("Queen", parent)
{
}


QSet<QPoint> Queen::recalculateMoves()
{
    QSet<QPoint> result;
    result.clear();

    projectMovement(-1, 0, result);  // horizontal left
    projectMovement(1, 0, result);   // horizontal right

    projectMovement(0, -1, result);  // vertical up
    projectMovement(0, 1, result);   // vertical down

    projectMovement(-1, -1, result); // left, top
    projectMovement(-1, 1, result);  // left, bottom
    projectMovement(1, -1, result); // right, top
    projectMovement(1, 1, result);  // right, bottom

    Q_ASSERT(!result.contains(position()));
    return result;
}
