#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QPoint>

#include "chessboardmodel.h"


inline uint qHash(const QPoint &key) { return (key.x() << 16) + key.y(); }

class Piece : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isBlack READ isBlack CONSTANT)
    Q_PROPERTY(bool isWhite READ isWhite CONSTANT)
    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(ChessBoardModel::PieceColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString image READ image CONSTANT)

    ChessBoardModel* board_;
    bool hasMoved_;

    ChessBoardModel::PieceColor color_;
    QPoint position_;
    QSet<QPoint> validMoves_;

    QPoint m_position;

public:
    Piece(QString name, ChessBoardModel* board);

    virtual ~Piece();

    int index() const;
    QString image() const;

    Q_INVOKABLE bool isValidMove(const QPoint& a) const;
    Q_INVOKABLE bool isValidMove(int x, int y) const;

    QPoint position() const;
    void setPosition(const QPoint& p);

    void markMoved(bool moved);
    int hasMoved() const;

    ChessBoardModel* board();
    const ChessBoardModel* board() const;

    ChessBoardModel::PieceColor color() const;
    bool isBlack() const;
    bool isWhite() const;

public slots:
    bool moveTo(QPoint p);

    void setColor(ChessBoardModel::PieceColor color);

    void update();


protected:
    void projectMovement(int xStep, int yStep, QSet<QPoint>& validMoves);
    virtual QSet<QPoint> recalculateMoves() = 0;

signals:
    // not really needed, but qml complains
    void colorChanged(ChessBoardModel::PieceColor color);
    void positionChanged(QPoint position);
};

#endif // PIECE_H
