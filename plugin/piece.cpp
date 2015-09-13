#include "piece.h"

#include <QDebug>

Piece::Piece(QString name, ChessBoardModel* board)
    : QObject(board)
    , board_(board)
    , hasMoved_(false)
    , color_(ChessBoardModel::BLACK)
{
    setObjectName(name);
}

Piece::~Piece()
{
}

QString Piece::image() const
{
    QString colorString = color() == ChessBoardModel::BLACK ? "black" : "white";
    return "qrc:/images/games-chess-"+objectName().toLower()+"-"+colorString+".png";
}

bool Piece::isValidMove(const QPoint& a) const
{
    return validMoves_.contains(a);
}

bool Piece::isValidMove(int x, int y) const
{
    return isValidMove(QPoint(x, y));
}

int Piece::index() const
{
    return position_.x() + position_.y() * 8;
}

QPoint Piece::position() const
{
    return position_;
}

void Piece::setPosition(const QPoint& p)
{
    position_ = p;
    update();
    positionChanged(p);
}

void Piece::markMoved(bool moved)
{
    hasMoved_ = moved;
    update();
}

int Piece::hasMoved() const
{
    return hasMoved_;
}

ChessBoardModel*Piece::board()
{
    return board_;
}

const ChessBoardModel*Piece::board() const
{
    return board_;
}

ChessBoardModel::PieceColor Piece::color() const
{
    return color_;
}

bool Piece::isBlack() const
{
    return color_ == ChessBoardModel::BLACK;
}

bool Piece::isWhite() const
{
    return color_ == ChessBoardModel::WHITE;
}

bool Piece::moveTo(QPoint p)
{
    bool result = board_->movePiece(this, p);
    if(result)
    {
        hasMoved_ = true;
        update();  // need to recalc based on the fact we've now moved.
    }
    return result;
}

void Piece::setColor(ChessBoardModel::PieceColor color)
{
    if(color_ != color)
    {
        color_ = color;
        emit colorChanged(color_);
    }
}

void Piece::update()
{
    validMoves_ = recalculateMoves();
}

void Piece::projectMovement(int xStep, int yStep, QSet<QPoint>& validMoves)
{
    QPoint p = position();
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
