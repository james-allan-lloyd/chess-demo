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

    addRow(-1, 0);  // horizontal left
    addRow(1, 0);   // horizontal right

    addRow(0, -1);  // vertical up
    addRow(0, 1);   // vertical down

    addRow(-1, -1); // left, top
    addRow(-1, 1);  // left, bottom
    addRow(1, -1); // right, top
    addRow(1, 1);  // right, bottom

    Q_ASSERT(!validMoves_.contains(currentPosition()));
}

void Queen::addRow(int xStep, int yStep)
{
    QPoint p = currentPosition();
    p += QPoint(xStep, yStep);
    while(p.x() >= 0 && p.y() >= 0 && p.x() <= 7 && p.y() <= 7)
    {
        validMoves_.insert(p);
        p += QPoint(xStep, yStep);
    }
}
