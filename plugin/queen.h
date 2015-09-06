#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"


class Queen : public Piece
{
    QSet<QPoint> validMoves_;
public:
    Queen(ChessBoardModel* parent);

signals:

public slots:

    // Piece interface
public:
    bool isValidMove(QPoint a) const override;

protected:
    void recalculateMoves() override;
};

#endif // QUEEN_H
