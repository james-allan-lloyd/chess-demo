#include "chessboardmodel.h"
#include "piece.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "king.h"

ChessBoardModel::ChessBoardModel(QObject* parent)
    : QAbstractListModel(parent)
    , cells_(64, NULL)
{
    pieceFactory_["queen"] = [this]() -> Piece* { return new Queen(this); };
    pieceFactory_["pawn"] = [this]() -> Piece* { return new Pawn(this); };
    pieceFactory_["rook"] = [this]() -> Piece* { return new Rook(this); };
    pieceFactory_["bishop"] = [this]() -> Piece* { return new Bishop(this); };
    pieceFactory_["knight"] = [this]() -> Piece* { return new Knight(this); };
    pieceFactory_["king"] = [this]() -> Piece* { return new King(this); };
}

ChessBoardModel::~ChessBoardModel()
{

}

Piece*ChessBoardModel::cell(QPoint p)
{
    if(!isValidPosition(p))
    {
        return NULL;
    }
    return cells_[p.x() + p.y() * 8];
}

const Piece* ChessBoardModel::cell(QPoint p) const
{
    if(!isValidPosition(p))
    {
        return NULL;
    }
    return cells_[p.x() + p.y() * 8];
}

void ChessBoardModel::removePiece(Piece* piece)
{
    int index = piece->index();
    Q_ASSERT(cells_[piece->index()] != NULL);
    cells_[piece->index()] = NULL;
    delete piece;
    pieces_.remove(piece);
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
}

int ChessBoardModel::rowCount(const QModelIndex&) const
{
    return 8 * 8;
}

QVariant ChessBoardModel::data(const QModelIndex& index, int) const
{
    QVariant result;
    result.setValue(cells_[index.row()]);
    return result;
}

QHash<int, QByteArray> ChessBoardModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ValidMoves] = "validMoves";
    return roles;
}

bool ChessBoardModel::movePiece(Piece* piece, QPoint position)
{
    if(piece == NULL)
    {
        qWarning() << "Attempt to move invalid piece to" << position;
        return false;
    }

    Q_ASSERT(piece != NULL);

    if(position.x() >= 8 || position.y() >= 8)
    {
        return false;
    }
    if(!piece->isValidMove(position))
    {
        return false;
    }
    int index = position.x() + position.y() * 8;
    // qDebug() << "Move piece to " << index;

    Piece* targetPiece = cell(position);
    Q_ASSERT(targetPiece != piece);
    if(targetPiece)
    {
        Q_ASSERT(targetPiece->color() != piece->color());
        emit pieceTaken(targetPiece->objectName(), targetPiece->isBlack() ? "black" : "white");
        removePiece(targetPiece);
        targetPiece = NULL; // deleted
    }
    Q_ASSERT(cell(position) == NULL);

    cells_[piece->index()] = NULL;
    emit dataChanged(createIndex(piece->index(), 0), createIndex(piece->index(), 0));

    cells_[index] = piece;
    piece->setPosition(position);
    foreach(Piece* otherPiece, pieces_)
    {
        otherPiece->recalculateMoves();
    }
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    return true;
}


Piece* ChessBoardModel::create(QString name, int x, int y, ChessBoardModel::PieceColor color)
{
    if(x < 0 || y < 0 || x >= 8 || y >= 8)
    {
        return NULL;
    }
    if(!pieceFactory_.contains(name.toLower()))
    {
        // unknown key type
        return NULL;
    }
    int index = x + y * 8;
    if(cells_[index] != NULL)
    {
        // return NULL if cell is already occupied
        // qDebug() << "cell occupied" << x << "," << y << (void*)cells_[index];
        return NULL;
    }
    Piece* piece = pieceFactory_.value(name.toLower())();
    piece->setColor(color);
    piece->setPosition(QPoint(x,y));
    cells_[piece->index()] = piece;
    foreach(Piece* otherPiece, pieces_)
    {
        otherPiece->recalculateMoves();
    }

    pieces_.insert(piece);
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    return piece;
}


void ChessBoardModel::clearPieces()
{
    foreach(Piece* piece, pieces_)
    {
        Q_ASSERT(cells_[piece->index()] != NULL);
        cells_[piece->index()] = NULL;
        delete piece;
    }
    pieces_.clear();
    cells_.fill(NULL);
    emit dataChanged(createIndex(0, 0), createIndex(63, 0));
}


bool ChessBoardModel::isValidPosition(const QPoint& p) const
{
    return p.x() >= 0 && p.y() >= 0 && p.x() < 8 && p.y() < 8;
}
