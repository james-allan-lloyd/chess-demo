#ifndef CHESSBOARDMODEL_H
#define CHESSBOARDMODEL_H

#include <QAbstractListModel>
#include <QPoint>
#include <QQmlListProperty>
#include <QDebug>
#include <functional>

class Piece;
class QXmlStreamReader;

class ChessBoardModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int pieceCount READ pieceCount NOTIFY pieceCountChanged)

    QVector<Piece*> cells_;
    QSet<Piece*> pieces_;

    typedef std::function<Piece*()> FactoryFunction;
    QMap<QString, FactoryFunction> pieceFactory_;

public:
    enum PieceRoles {
        ValidMoves = Qt::UserRole + 1,
    };

    /**
     * @brief The color of pieces
     *
     * I would have preferred this to be in piece, but because we're using
     * factory function create* here, and QML isn't great about sharing custom
     * enums between classes, it's expedient to just define it here. Still works
     * as a property of piece.
     */
    enum PieceColor {
        BLACK,
        WHITE
    };
    Q_ENUMS(PieceColor)

    ChessBoardModel(QObject *parent = 0);
    ~ChessBoardModel();

    Q_INVOKABLE Piece* cell(QPoint p);
    const Piece* cell(QPoint p) const;

    Q_INVOKABLE Piece* cell(int x, int y)
    {
        return cell(QPoint(x, y));
    }

    Q_INVOKABLE void removePiece(Piece* piece);

    // QAbstractItemModel methods
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex&) const;
    QVariant data(const QModelIndex &index, int /*role*/) const;

    int pieceCount() const { return pieces_.size(); }
    bool movePiece(Piece* piece, QPoint position);
    Q_INVOKABLE Piece* create(QString name, int x, int y, PieceColor color = BLACK);
    Q_INVOKABLE void clearPieces();

    bool isValidPosition(const QPoint& p) const;

private:
    bool readEvents(QXmlStreamReader& xml);
    bool readCreate(QXmlStreamReader& xml);
    bool readMove(QXmlStreamReader& xml);


signals:
    void pieceCountChanged(int newPieceCount);
};

#endif // CHESSBOARDMODEL_H
