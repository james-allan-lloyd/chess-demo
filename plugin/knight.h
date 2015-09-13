#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"


class Knight : public Piece
{
    QList<QPoint> knightVectors_;
public:
    Knight(ChessBoardModel* parent);

protected:
    QSet<QPoint> recalculateMoves() override;
};

#endif // KNIGHT_H
