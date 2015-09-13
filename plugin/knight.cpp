#include "knight.h"

Knight::Knight(ChessBoardModel* parent)
    : Piece("Knight", parent)
{
    knightVectors_.append(QPoint(-1, -2));
    knightVectors_.append(QPoint( 1, -2));
    knightVectors_.append(QPoint(-1,  2));
    knightVectors_.append(QPoint( 1,  2));
    knightVectors_.append(QPoint(-2, -1));
    knightVectors_.append(QPoint(-2,  1));
    knightVectors_.append(QPoint( 2, -1));
    knightVectors_.append(QPoint( 2,  1));
}

QSet<QPoint> Knight::recalculateMoves()
{
    QSet<QPoint> result;
    foreach(QPoint p, knightVectors_)
    {
        QPoint targetPoint = position() + p;
        if(targetPoint.x() < 0 || targetPoint.y() < 0 || targetPoint.x() > 7 || targetPoint.y() > 7)
        {
            continue;
        }

        Piece* occupied = board()->cell(targetPoint);
        if(occupied && occupied->color() == color())
        {
            continue;
        }

        result.insert(targetPoint);
    }

    return result;
}
