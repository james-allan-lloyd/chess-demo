#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece
{
public:
    Bishop(ChessBoardModel* parent);

protected:
    QSet<QPoint> recalculateMoves() override;
};

#endif // BISHOP_H
