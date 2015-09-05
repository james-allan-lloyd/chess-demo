#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QPoint>
#include <QQmlListProperty>


class Piece : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(QList<QPoint> validMoves READ validMoves)
    Q_PROPERTY(QVariantList validMoves READ validMoves)
    Q_PROPERTY(int index READ index WRITE setIndex)
    Q_PROPERTY(QPoint currentPosition READ currentPosition)

    int m_index;

public:
    Piece(QString name, QObject* parent)
        : QObject(parent)
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
        return QPoint(m_index / 8, m_index % 8);
    }

public slots:
    void setIndex(int index)
    {
        m_index = index;
    }

    bool moveTo(QPoint p)
    {
        if(p.x() >= 8 || p.y() >= 8)
        {
            return false;
        }
        if(isValidMove(p))
        {
            setIndex(p.x() + p.y() * 8);
            return true;
        }
        return false;
    }
};

#endif // PIECE_H
