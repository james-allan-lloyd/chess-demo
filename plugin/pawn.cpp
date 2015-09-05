#include "pawn.h"
#include <QDebug>

Pawn::Pawn(QObject* parent)
    : Piece("Pawn", parent)
{
    result.append(QPoint(1, 0));
}

QVariantList Pawn::validMoves()
{
    QVariantList result;
    result.push_back(QPoint(1, 0));
    return result;
}


bool Pawn::isValidMove(QPoint a) const
{
    if(a.x() >= 8 || a.y() >= 8)
        return false;

    if(currentPosition() + QPoint(1, 0) == a)
        return true;
    return false;
}

