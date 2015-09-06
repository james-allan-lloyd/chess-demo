#include "pawn.h"
#include <QDebug>

Pawn::Pawn(ChessBoardModel* parent)
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
    if(a.x() < 0 || a.y() < 0 || a.x() >= 8 || a.y() >= 8)
        return false;

    // qDebug() << "valid move?" << currentPosition() << a;
    if(currentPosition() + QPoint(0,1) == a)
        return true;

    if(!hasMoved() && currentPosition() + QPoint(0,2) == a)
        return true;

    return false;
}
