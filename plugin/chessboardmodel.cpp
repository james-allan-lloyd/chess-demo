#include "chessboardmodel.h"
#include "piece.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "king.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QUrl>


class ChessBoardModel::Writer : public QObject
{
    QString filename_;
    QDomDocument doc_;
public:
    Writer(QObject* parent)
        : QObject(parent)
        , filename_("current_game.chess")
        , doc_("currentGame")
    {
        clear();
    }

    const QString filename() const { return filename_; }

    void clear()
    {
        QFile f(filename_);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qWarning() << "Unable to save to: " << filename_;
        }

        QXmlStreamWriter out(&f);
        out.setAutoFormatting(true);
        out.writeStartDocument();

        out.writeStartElement("chess");
        out.writeAttribute("version", "1.0");
        out.writeEndElement();

        out.writeEndDocument();
    }


    void loadDoc()
    {
        QFile file(filename_);
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc_.setContent(&file)) {
            file.close();
            return;
        }
        file.close();
    }


    void saveDoc()
    {
        QFile file(filename_);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }
        QTextStream textStream(&file);
        doc_.save(textStream, 0);
    }


    void writeCreate(const Piece& piece)
    {
        loadDoc();
        QDomElement createElement = doc_.createElement("create");
        createElement.setAttribute("name", piece.objectName());
        createElement.setAttribute("x", piece.position().x());
        createElement.setAttribute("y", piece.position().y());
        createElement.setAttribute("color", piece.isBlack() ? "black" : "white");
        doc_.documentElement().appendChild(createElement);
        saveDoc();
    }

    void writeMove(const Piece& piece, const QPoint& dest)
    {
        loadDoc();
        QDomElement createElement = doc_.createElement("move");
        createElement.setAttribute("name", piece.objectName());
        createElement.setAttribute("x", piece.position().x());
        createElement.setAttribute("y", piece.position().y());
        createElement.setAttribute("destX", dest.x());
        createElement.setAttribute("destY", dest.y());
        createElement.setAttribute("color", piece.isBlack() ? "black" : "white");
        doc_.documentElement().appendChild(createElement);
        saveDoc();
    }

};


ChessBoardModel::ChessBoardModel(QObject* parent)
    : QAbstractListModel(parent)
    , cells_(64, NULL)
    , writer_(new Writer(this))
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
        removePiece(targetPiece);
        targetPiece = NULL; // deleted
    }
    Q_ASSERT(cell(position) == NULL);

    cells_[piece->index()] = NULL;
    emit dataChanged(createIndex(piece->index(), 0), createIndex(piece->index(), 0));

    cells_[index] = piece;
    writer_->writeMove(*piece, position); // write before setting new position
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
    writer_->writeCreate(*piece);
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
    writer_->clear();
    cells_.fill(NULL);
    emit dataChanged(createIndex(0, 0), createIndex(63, 0));
}

bool ChessBoardModel::save(const QString& filename)
{
    QString fileNameFromUrl = QUrl(filename).toLocalFile();
    qDebug() << "Save: " << fileNameFromUrl;
    QFile::remove(fileNameFromUrl);
    bool result = QFile::copy(writer_->filename(), fileNameFromUrl);
    qDebug() << "Saved?" << result;
    return result;
}

bool ChessBoardModel::load(const QString& filename)
{
    QString fileNameFromUrl = QUrl(filename).toLocalFile();
    clearPieces();
    QFile f(fileNameFromUrl);
    if(!f.open(QIODevice::ReadOnly))
    {
        qWarning() << "Unable to load from: " << fileNameFromUrl;
        return false;
    }

    QXmlStreamReader xml(&f);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "chess")
        {
            return readEvents(xml);
        }
        else
        {
            qWarning() << "Invalid start element" << xml.name();
            return false;
        }
    }

    qWarning() << "No xml elements found";
    return false;
}


bool ChessBoardModel::isValidPosition(const QPoint& p) const
{
    return p.x() >= 0 && p.y() >= 0 && p.x() < 8 && p.y() < 8;
}

bool ChessBoardModel::readEvents(QXmlStreamReader& xml)
{
   while(xml.readNextStartElement())
   {
       if(xml.name() == "create")
       {
           readCreate(xml);
       }
       else if(xml.name() == "move")
       {
           readMove(xml);
       }
       else
       {
           xml.skipCurrentElement();
       }
   }
   return true;
}

bool ChessBoardModel::readCreate(QXmlStreamReader& xml)
{
    int x = xml.attributes().value("x").toInt();
    int y = xml.attributes().value("y").toInt();
    QString name = xml.attributes().value("name").toString();
    QString color = xml.attributes().value("color").toString();
    PieceColor col;
    if(color == "black")
    {
        col = BLACK;
    }
    else
    {
        col = WHITE;
    }
    create(name, x, y, col);
    xml.skipCurrentElement();
    return true;
}

bool ChessBoardModel::readMove(QXmlStreamReader& xml)
{
    int x = xml.attributes().value("x").toInt();
    int y = xml.attributes().value("y").toInt();
    int destX = xml.attributes().value("destX").toInt();
    int destY = xml.attributes().value("destY").toInt();
    QString name = xml.attributes().value("name").toString();
    QString color = xml.attributes().value("color").toString();
    PieceColor col;
    if(color == "black")
    {
        col = BLACK;
    }
    else
    {
        col = WHITE;
    }

    Piece* piece = cell(QPoint(x,y));
    Q_ASSERT(piece);
    bool moved = piece->moveTo(QPoint(destX, destY));
    Q_ASSERT(moved);
    xml.skipCurrentElement();
    return true;
}
