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

    int direction = 1;
    if(color() == ChessBoardModel::WHITE)
    {
        direction = -1;
    }

    // qDebug() << "valid move?" << currentPosition() << a;
    if(currentPosition() + QPoint(0,direction*1) == a)
        return true;

    if(!hasMoved() && currentPosition() + QPoint(0,direction*2) == a)
        return true;

    if(currentPosition() + QPoint(1,direction*1) == a || currentPosition() + QPoint(-1,direction*1) == a)
    {
        // diagonal attack
        return board()->cell(a) != NULL && board()->cell(a)->color() != color();
        // return true;
    }

    return false;
}

