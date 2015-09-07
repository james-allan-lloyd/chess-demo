#include "king.h"

King::King(ChessBoardModel* parent)
    : Piece("King", parent)
{

}

bool King::isValidMove(QPoint a) const
{
    return validMoves_.contains(a);
}

void King::recalculateMoves()
{
    validMoves_.clear();
    for(int y=-1; y<=1; ++y)
    {
        for(int x=-1; x<=1; ++x)
        {
            if(x != 0 || y != 0)
            {
                QPoint p = currentPosition() + QPoint(x, y);
                Piece* occupied = board()->cell(p);
                if(!occupied || occupied->color() != color())
                {
                    // don't add self move
                    validMoves_.insert(p);
                }
            }
        }
    }
}
