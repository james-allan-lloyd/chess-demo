#ifndef PAWN_H
#define PAWN_H

#include "piece.h"


class Pawn : public Piece
{
public:
    Pawn(ChessBoardModel* parent);

    bool isValidMove(QPoint a) const override;

signals:

public slots:

};

#endif // PAWN_H
