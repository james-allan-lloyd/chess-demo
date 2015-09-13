#include "king.h"

King::King(ChessBoardModel* parent)
    : Piece("King", parent)
{

}

QSet<QPoint> King::recalculateMoves()
{
    QSet<QPoint> result;
    for(int y=-1; y<=1; ++y)
    {
        for(int x=-1; x<=1; ++x)
        {
            if(x != 0 || y != 0)
            {
                QPoint p = position() + QPoint(x, y);
                Piece* occupied = board()->cell(p);
                if(!occupied || occupied->color() != color())
                {
                    // don't add self move
                    result.insert(p);
                }
            }
        }
    }

    return result;
}
