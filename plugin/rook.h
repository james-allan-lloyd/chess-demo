#ifndef ROOK_H
#define ROOK_H

#include "piece.h"


class Rook : public Piece
{
    QSet<QPoint> validMoves_;
public:
    Rook(ChessBoardModel* parent);

signals:

public slots:

    // Piece interface
public:
    bool isValidMove(QPoint a) const override;
    void recalculateMoves() override;
};

#endif // ROOK_H
