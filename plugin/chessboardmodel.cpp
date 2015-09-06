#include "chessboardmodel.h"
#include "piece.h"
#include "pawn.h"


ChessBoardModel::ChessBoardModel(QObject* parent)
    : QAbstractListModel(parent)
    , cells_(64, NULL)
{
    // cells_[0] = new Piece("Pawn");
}

ChessBoardModel::~ChessBoardModel()
{

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
        return NULL;
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
    cells_[piece->index()] = NULL;
    emit dataChanged(createIndex(piece->index(), 0), createIndex(piece->index(), 0));
    cells_[index] = piece;
    piece->setIndex(index);
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    return true;
}

Piece* ChessBoardModel::createPawn(int x, int y, PieceColor color)
{
    if(x < 0 || y < 0 || x >= 8 || y >= 8)
    {
        return NULL;
    }
    int index = x + y * 8;
    if(cells_[index] != NULL)
    {
        // return NULL if cell is already occupied
        // qDebug() << "cell occupied" << x << "," << y << (void*)cells_[index];
        return NULL;
    }
    Piece* piece = new Pawn(this);
    piece->setColor(color);
    piece->setIndex(index);
    cells_[piece->index()] = piece;
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
}
