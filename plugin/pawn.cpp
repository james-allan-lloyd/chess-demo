#include "pawn.h"
#include <QDebug>

Pawn::Pawn(ChessBoardModel* parent)
    : Piece("Pawn", parent)
{
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

    const Piece* occupied = board()->cell(a);
    if(occupied && occupied->color() == color())
    {
        return false;
    }

    // qDebug() << "valid move?" << currentPosition() << a;
    if(currentPosition() + QPoint(0,direction*1) == a)
        return true;

    if(!hasMoved() && currentPosition() + QPoint(0,direction*2) == a
            && !board()->cell(currentPosition() + QPoint(0, direction*1)))
        return true;

    if(currentPosition() + QPoint(1,direction*1) == a || currentPosition() + QPoint(-1,direction*1) == a)
    {
        // diagonal attack
        return board()->cell(a) != NULL && board()->cell(a)->color() != color();
        // return true;
    }

    return false;
}

