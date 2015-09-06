#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QPoint>
#include <QQmlListProperty>

#include "chessboardmodel.h"


class Piece : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(QList<QPoint> validMoves READ validMoves)
    Q_PROPERTY(QVariantList validMoves READ validMoves)
    Q_PROPERTY(int index READ index WRITE setIndex)
    Q_PROPERTY(QPoint currentPosition READ currentPosition)

    ChessBoardModel* board_;
    int m_index;
    bool hasMoved_;

public:
    Piece(QString name, ChessBoardModel* board)
        : QObject(board)
        , board_(board)
        , hasMoved_(false)
    {
        setObjectName(name);
    }

    virtual QVariantList validMoves() = 0;

    Q_INVOKABLE virtual bool isValidMove(QPoint a) const = 0;

    int index() const
    {
        return m_index;
    }

    QPoint currentPosition() const
    {
        return QPoint(m_index % 8, m_index / 8);
    }

    int hasMoved() const { return hasMoved_; }
    ChessBoardModel* board() { return board_; }

public slots:
    void setIndex(int index)
    {
        m_index = index;
    }

    bool moveTo(QPoint p)
    {
        bool result = board_->movePiece(this, p);
        if(result)
        {
            hasMoved_ = true;
        }
        return result;
    }
};

#endif // PIECE_H
