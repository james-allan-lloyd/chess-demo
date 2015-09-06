#ifndef PAWN_H
#define PAWN_H

#include "piece.h"


class Pawn : public Piece
{
    QList<QPoint> result;
    bool hasMoved_;
public:
    Pawn(ChessBoardModel* parent);

    bool isValidMove(QPoint a) const override;

signals:

public slots:

    // Piece interface
public:
    QVariantList validMoves() override;
};

#endif // PAWN_H
