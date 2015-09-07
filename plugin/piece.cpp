#include "piece.h"

#include <QDebug>

void Piece::recalculateMoves()
{
}

QString Piece::image() const
{
    QString colorString = color() == ChessBoardModel::BLACK ? "black" : "white";
    return "qrc:/images/games-chess-"+objectName().toLower()+"-"+colorString+".png";
}

void Piece::projectMovement(int xStep, int yStep, QSet<QPoint>& validMoves)
{
    QPoint p = currentPosition();
    p += QPoint(xStep, yStep);
    while(p.x() >= 0 && p.y() >= 0 && p.x() <= 7 && p.y() <= 7)
    {
        Piece* occupied = board()->cell(p);
        if(!occupied || occupied->color() != color())
        {
            validMoves.insert(p);
            p += QPoint(xStep, yStep);
            if(occupied)
                break;
        }
        else
        {
            break;
        }
    }
}
