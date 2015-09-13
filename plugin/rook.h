#ifndef ROOK_H
#define ROOK_H

#include "piece.h"


class Rook : public Piece
{
public:
    Rook(ChessBoardModel* parent);

protected:
    QSet<QPoint> recalculateMoves() override;
};

#endif // ROOK_H
