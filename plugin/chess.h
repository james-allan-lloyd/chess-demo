#ifndef CHESS_H
#define CHESS_H

#include <QQuickItem>

class Chess : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Chess)

public:
    Chess(QQuickItem *parent = 0);
    ~Chess();
};

#endif // CHESS_H

