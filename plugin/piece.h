#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QPoint>
#include <QQmlListProperty>

#include "chessboardmodel.h"


inline uint qHash(const QPoint &key) { return (key.x() << 16) + key.y(); }

class Piece : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(int index READ index WRITE setIndex)
    Q_PROPERTY(bool isBlack READ isBlack CONSTANT)
    Q_PROPERTY(bool isWhite READ isWhite CONSTANT)
    Q_PROPERTY(QPoint position READ position)
    Q_PROPERTY(ChessBoardModel::PieceColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString image READ image CONSTANT)

    ChessBoardModel* board_;
    // int m_index;
    bool hasMoved_;

    ChessBoardModel::PieceColor color_;
    QPoint position_;

public:
    Piece(QString name, ChessBoardModel* board)
        : QObject(board)
        , board_(board)
        , hasMoved_(false)
        , color_(ChessBoardModel::BLACK)
    {
        setObjectName(name);
    }

    Q_INVOKABLE virtual bool isValidMove(QPoint a) const = 0;

    int index() const
    {
        return position_.x() + position_.y() * 8;
    }

    QPoint position() const
    {
        return position_;
    }

    void setPosition(const QPoint& p)
    {
        position_ = p;
        recalculateMoves();
    }

    int hasMoved() const { return hasMoved_; }
    ChessBoardModel* board() { return board_; }
    const ChessBoardModel* board() const { return board_; }

    ChessBoardModel::PieceColor color() const
    {
        return color_;
    }

    bool isBlack() const
    {
        return color_ == ChessBoardModel::BLACK;
    }

    bool isWhite() const
    {
        return color_ == ChessBoardModel::WHITE;
    }

public slots:
    // void setIndex(int index)
    // {
    //     m_index = index;
    // }

    bool moveTo(QPoint p)
    {
        bool result = board_->movePiece(this, p);
        if(result)
        {
            hasMoved_ = true;
            recalculateMoves();  // need to recalc based on the fact we've now moved.
        }
        return result;
    }

    void setColor(ChessBoardModel::PieceColor color)
    {
        if(color_ != color)
        {
            color_ = color;
            emit colorChanged(color_);
        }
    }

public:
    // FIXME: make pure rather than empty
    virtual void recalculateMoves();

    QString image() const;

protected:
    void projectMovement(int xStep, int yStep, QSet<QPoint>& validMoves);

signals:
    // not really needed, but qml complains
    void colorChanged(ChessBoardModel::PieceColor color);
};

#endif // PIECE_H
