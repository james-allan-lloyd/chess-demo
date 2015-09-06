#ifndef CHESSBOARDMODEL_H
#define CHESSBOARDMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QQmlListProperty>
#include <QDebug>

class Piece;

class ChessBoardModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int pieceCount READ pieceCount NOTIFY pieceCountChanged)

    QVector<QObject*> cells_;
    QSet<Piece*> pieces_;
public:
    enum PieceRoles {
        ValidMoves = Qt::UserRole + 1,
    };

    ChessBoardModel(QObject *parent = 0);
    ~ChessBoardModel();

    // QAbstractItemModel methods
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex&) const;
    QVariant data(const QModelIndex &index, int /*role*/) const;

    int pieceCount() const { return pieces_.size(); }
    bool movePiece(Piece* piece, QPoint position);
    Q_INVOKABLE Piece* createPawn(int row, int col);
    Q_INVOKABLE void clearPieces();

signals:
    void pieceCountChanged(int newPieceCount);
};

#endif // CHESSBOARDMODEL_H
