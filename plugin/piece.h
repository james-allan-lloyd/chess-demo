#ifndef PIECE_H
#define PIECE_H

#include <QObject>


class Piece : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QList<int> validMoves READ validMoves)
    Q_PROPERTY(int index READ index WRITE setIndex)

    QString name_;
    QList<int> validMoves_;
    int m_index;

public:
    Piece(QString name)
    {
        name_ = name;
        validMoves_.append(63);
    }

    QString name() const { return name_; }

    QList<int> validMoves() const
    {
        return validMoves_;
    }

    int index() const
    {
        return m_index;
    }

public slots:
    void setName(const QString& name) { name_ = name; emit nameChanged(name); }

    void setIndex(int index)
    {
        m_index = index;
    }

signals:
    void nameChanged(QString name);
};

#endif // PIECE_H
