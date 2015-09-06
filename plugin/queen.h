#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

inline uint qHash(const QPoint &key) { return (key.x() << 16) + key.y(); }

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
    void positionUpdated() override;

private:
    void addRow(int xStep, int yStep);
};

#endif // QUEEN_H
