#include "pawn.h"
#include <QDebug>

Pawn::Pawn(ChessBoardModel* parent)
    : Piece("Pawn", parent)
{
}


QSet<QPoint> Pawn::recalculateMoves()
{
    QSet<QPoint> result;

    int direction = 1;
    if(color() == ChessBoardModel::WHITE)
    {
        direction = -1;
    }

    QPoint oneMove = position() + QPoint(0, direction);
    if(board()->isValidPosition(oneMove) && !board()->cell(oneMove))
    {
        result.insert(oneMove);
        if(!hasMoved())
        {
            QPoint twoMove = position() + QPoint(0, 2*direction);
            if(board()->isValidPosition(twoMove) && !board()->cell(twoMove))
            {
                result.insert(twoMove);
            }
        }
    }

    QPoint leftAttack = position() + QPoint(1,direction*1);
    Piece* leftAttackPiece = board()->cell(leftAttack);
    if(leftAttackPiece && leftAttackPiece->color() != color())
    {
        result.insert(leftAttack);
    }

    QPoint rightAttack = position() + QPoint(-1,direction*1);
    Piece* rightAttackPiece = board()->cell(rightAttack);
    if(rightAttackPiece && rightAttackPiece->color() != color())
    {
        result.insert(rightAttack);
    }

    return result;
}
