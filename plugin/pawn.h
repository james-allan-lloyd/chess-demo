#ifndef PAWN_H
#define PAWN_H

#include "piece.h"


class Pawn : public Piece
{
    QSet<QPoint> validMoves_;
public:
    Pawn(ChessBoardModel* parent);

    bool isValidMove(QPoint a) const override;

signals:

public slots:


    // Piece interface
public:
    void recalculateMoves() override;
};

#endif // PAWN_H
