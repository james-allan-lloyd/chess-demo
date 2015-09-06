#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"


class Knight : public Piece
{
    QSet<QPoint> validMoves_;
    QList<QPoint> knightVectors_;
public:
    Knight(ChessBoardModel* parent);

signals:

public slots:

    // Piece interface
public:
    bool isValidMove(QPoint a) const override;
    void recalculateMoves() override;
};

#endif // KNIGHT_H
