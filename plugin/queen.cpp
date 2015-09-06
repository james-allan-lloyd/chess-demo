#include "queen.h"

Queen::Queen(ChessBoardModel* parent)
    : Piece("Queen", parent)
{

}

bool Queen::isValidMove(QPoint a) const
{
    return validMoves_.contains(a);
}


void Queen::positionUpdated()
{
    validMoves_.clear();
    for(int x=0; x<8; ++x)
    {
        QPoint p(x, currentPosition().y());
        if(p != currentPosition())
        {
            validMoves_.insert(p);
        }
    }
}
