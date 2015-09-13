#ifndef KING_H
#define KING_H

#include "piece.h"


class King : public Piece
{
public:
    King(ChessBoardModel* parent);

protected:
    QSet<QPoint> recalculateMoves() override;
};

#endif // KING_H
