#ifndef PAWN_H
#define PAWN_H

#include "piece.h"


class Pawn : public Piece
{
    QList<QPoint> result;
public:
    Pawn(QObject* parent);

    bool isValidMove(QPoint a) const override;

signals:

public slots:

    // Piece interface
public:
    QVariantList validMoves() override;
};

#endif // PAWN_H
