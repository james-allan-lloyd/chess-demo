#include "chessboardmodel.h"
#include "piece.h"


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
    Piece* piece = new Piece("Pawn", this);
    piece->setIndex(row * 8 + col);
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
}
