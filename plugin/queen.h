#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"


class Queen : public Piece
{
public:
    Queen(ChessBoardModel* parent);

protected:
    QSet<QPoint> recalculateMoves() override;
};

#endif // QUEEN_H
