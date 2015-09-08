#include "pawn.h"
#include <QDebug>

Pawn::Pawn(ChessBoardModel* parent)
    : Piece("Pawn", parent)
{
}


bool Pawn::isValidMove(QPoint a) const
{
    return validMoves_.contains(a);
}



void Pawn::recalculateMoves()
{
    validMoves_.clear();

    int direction = 1;
    if(color() == ChessBoardModel::WHITE)
    {
        direction = -1;
    }

    QPoint oneMove = position() + QPoint(0, direction);
    if(board()->isValidPosition(oneMove) && !board()->cell(oneMove))
    {
        validMoves_.insert(oneMove);
        if(!hasMoved())
        {
            QPoint twoMove = position() + QPoint(0, 2*direction);
            if(board()->isValidPosition(twoMove) && !board()->cell(twoMove))
            {
                validMoves_.insert(twoMove);
            }
        }
    }

    QPoint leftAttack = position() + QPoint(1,direction*1);
    Piece* leftAttackPiece = board()->cell(leftAttack);
    if(leftAttackPiece && leftAttackPiece->color() != color())
    {
        validMoves_.insert(leftAttack);
    }

    QPoint rightAttack = position() + QPoint(-1,direction*1);
    Piece* rightAttackPiece = board()->cell(rightAttack);
    if(rightAttackPiece && rightAttackPiece->color() != color())
    {
        validMoves_.insert(rightAttack);
    }
}
