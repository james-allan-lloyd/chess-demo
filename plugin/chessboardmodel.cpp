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

bool ChessBoardModel::movePiece(int index, Piece* piece)
{
    qDebug() << "Move piece to " << index;
    cells_[piece->index()] = NULL;
    emit dataChanged(createIndex(piece->index(), 0), createIndex(piece->index(), 0));
    cells_[index] = piece;
    piece->setIndex(index);
    emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    return true;
}

Piece* ChessBoardModel::createPawn(int row, int col)
{
    if(row < 0 || col < 0 || row >= 8 || col >= 8)
    {
        // qDebug() << "invalid position" << row << "," << col;
        return NULL;
    }
    int index = row * 8 + col;
    if(cells_[index] != NULL)
    {
        // return NULL if cell is already occupied
        qDebug() << "cell occupied" << row << "," << col << (void*)cells_[index];
        return NULL;
    }
    Piece* piece = new Pawn(this);
    piece->setIndex(index);
    cells_[piece->index()] = piece;
    pieces_.insert(piece);
    return piece;
}


void ChessBoardModel::clearPieces()
{
    foreach(Piece* piece, pieces_)
    {
        cells_[piece->index()] = NULL;
        delete piece;
    }
    pieces_.clear();
    cells_.fill(NULL);
}
