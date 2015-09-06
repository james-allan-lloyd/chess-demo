#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece
{
    QSet<QPoint> validMoves_;
public:
    Bishop(ChessBoardModel* parent);

signals:

public slots:

    // Piece interface
public:
    bool isValidMove(QPoint a) const override;
    void recalculateMoves() override;
};

#endif // BISHOP_H
