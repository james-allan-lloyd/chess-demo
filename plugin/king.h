#ifndef KING_H
#define KING_H

#include "piece.h"


class King : public Piece
{
    QSet<QPoint> validMoves_;
public:
    King(ChessBoardModel* parent);

signals:

public slots:

    // Piece interface
public:
    bool isValidMove(QPoint a) const override;
    void recalculateMoves() override;
};

#endif // KING_H
