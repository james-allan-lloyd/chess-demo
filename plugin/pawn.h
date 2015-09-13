#ifndef PAWN_H
#define PAWN_H

#include "piece.h"


class Pawn : public Piece
{
public:
    Pawn(ChessBoardModel* parent);

protected:
    QSet<QPoint> recalculateMoves() override;
};

#endif // PAWN_H
