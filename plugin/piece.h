#ifndef PIECE_H
#define PIECE_H

#include <QObject>


class Piece : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> validMoves READ validMoves)
    Q_PROPERTY(int index READ index WRITE setIndex)

    QList<int> validMoves_;
    int m_index;

public:
    Piece(QString name, QObject* parent)
        : QObject(parent)
    {
        setObjectName(name);
        validMoves_.append(63);
    }

    QList<int> validMoves() const
    {
        return validMoves_;
    }

    int index() const
    {
        return m_index;
    }

public slots:
    void setIndex(int index)
    {
        m_index = index;
    }
};

#endif // PIECE_H
