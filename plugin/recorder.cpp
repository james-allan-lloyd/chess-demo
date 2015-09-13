#include "recorder.h"
#include "piece.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QUrl>

namespace
{

    ChessBoardModel::PieceColor colorFromString(const QString& colorString)
    {
        QString lowerColorString = colorString.toLower();

        if(lowerColorString == "white")
        {
            return ChessBoardModel::WHITE;
        }
        else if(lowerColorString == "black")
        {
            return ChessBoardModel::BLACK;
        }

        qWarning() << "Unknown color" << colorString << " defaulting to black";
        return ChessBoardModel::BLACK;
    }

}

class CreateAction : public Recorder::Action
{
    QString name_;
    int x_;
    int y_;
    ChessBoardModel::PieceColor color_;
public:
    CreateAction(const QString& name, int x, int y, ChessBoardModel::PieceColor color)
        : name_(name)
        , x_(x)
        , y_(y)
        , color_(color)
    {
    }

    // Action interface
public:
    bool undo(ChessBoardModel* model) override
    {
        // qDebug() << "Undo creation " << name_ << x_ << y_;
        Piece* piece = model->cell(QPoint(x_, y_));
        Q_ASSERT(piece);
        model->removePiece(piece);
        Q_ASSERT(!model->cell(QPoint(x_, y_)));
        return true;
    }

    bool redo(ChessBoardModel* model) override
    {
        // qDebug() << "Redo creation" << name_ << x_ << y_;
        Piece* piece = model->create(name_, x_, y_, color_);
        return piece != NULL;
    }

    void write(QDomDocument& ) override
    {

    }
};


class MoveAction : public Recorder::Action
{
    const QString name_;
    const int oldX_;
    const int oldY_;
    const int newX_;
    const int newY_;
    const ChessBoardModel::PieceColor color_;
    ChessBoardModel::PieceColor takenColor_;
    QString takenName_;
    bool takenHasMoved_;
    const bool hadMoved_;

public:
    MoveAction(Piece* piece, int oldX, int oldY, bool hadMoved)
        : name_(piece->objectName())
        , oldX_(oldX)
        , oldY_(oldY)
        , newX_(piece->position().x())
        , newY_(piece->position().y())
        , color_(piece->color())
        , hadMoved_(hadMoved)
    {}


    MoveAction(const QString& name,
               int oldX,
               int oldY,
               int newX,
               int newY,
               ChessBoardModel::PieceColor color,
               bool hadMoved)
        : name_(name)
        , oldX_(oldX)
        , oldY_(oldY)
        , newX_(newX)
        , newY_(newY)
        , color_(color)
        , hadMoved_(hadMoved)
    {}


    void setTakenPiece(const QString& name, ChessBoardModel::PieceColor col, bool hasMoved)
    {
        takenName_ = name;
        takenColor_ = col;
        takenHasMoved_ = hasMoved;
    }

    bool undo(ChessBoardModel* model) override
    {
        // qDebug() << "Undo move " << name_ << newX_ << newY_;
        Piece* piece = model->cell(newX_, newY_);
        Q_ASSERT(piece != NULL);
        model->removePiece(piece); // because pawn moves are not reversable
        piece = model->create(name_, oldX_, oldY_, color_);
        piece->markMoved(hadMoved_);
        Q_ASSERT(piece != NULL);

        if(!takenName_.isEmpty())
        {
            Piece* piece = model->create(takenName_, newX_, newY_, takenColor_);
            piece->markMoved(takenHasMoved_);
        }
        return true;
    }

    bool redo(ChessBoardModel* model) override
    {
        // qDebug() << "Redo move " << name_ << newX_ << newY_;
        Piece* piece = model->cell(oldX_, oldY_);
        Q_ASSERT(piece);
        bool moved = piece->moveTo(QPoint(newX_, newY_));
        Q_ASSERT(moved);
        return true;
    }

    void write(QDomDocument& doc) override
    {
        QDomElement moveElement = doc.createElement("move");
        moveElement.setAttribute("name", name_);
        moveElement.setAttribute("x", oldX_);
        moveElement.setAttribute("y", oldY_);
        moveElement.setAttribute("destX", newX_);
        moveElement.setAttribute("destY", newY_);
        moveElement.setAttribute("color", color_ == ChessBoardModel::BLACK ? "black" : "white");
        moveElement.setAttribute("hadMoved", hadMoved_ ? "1" : "0");

        if(!takenName_.isEmpty())
        {
            QDomElement takeElement = doc.createElement("takes");
            takeElement.setAttribute("name", takenName_);
            takeElement.setAttribute("hadMoved", takenHasMoved_ ? "1" : "0");
            takeElement.setAttribute("color", takenColor_ == ChessBoardModel::BLACK ? "black" : "white");
            moveElement.appendChild(takeElement);
        }
        doc.documentElement().appendChild(moveElement);
    }
};



class Recorder::Writer : public QObject
{
    QString filename_;
    QDomDocument doc_;
public:
    Writer(const QString& filename)
        : filename_(filename)
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
            qWarning() << "Unable to load document from" << filename_;
        }
        file.close();
    }


    void saveDoc()
    {
        QFile file(filename_);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qWarning() << "Not able to save doc";
            return;
        }
        QTextStream textStream(&file);
        doc_.save(textStream, 4);
    }


    void write(const QList<Action*>& actions)
    {
        loadDoc();
        foreach(Action* action, actions)
        {
            action->write(doc_);
        }
        saveDoc();
    }

};


Recorder::Recorder(QObject* parent)
    : QObject(parent)
    , model_(NULL)
    , nextUndo_(-1)
{

}

Recorder::~Recorder()
{
    restart(); // clearing memory
}

ChessBoardModel* Recorder::model() const
{
    return model_;
}

Piece* Recorder::create(const QString& name, int x, int y, const QString& colorString)
{
    if(!model_)
    {
        return NULL;
    }
    ChessBoardModel::PieceColor col = colorFromString(colorString);
    Piece* result = model_->create(name, x, y, col);
    if(result)
    {
        queueAction(new CreateAction(name, x, y, col));
    }
    return result;
}

bool Recorder::move(Piece* piece, int x, int y)
{
    Piece* opposingPiece = model_->cell(x, y);
    bool takeMove = true;
    bool takenHadMoved = true;
    QString takenPiece;
    ChessBoardModel::PieceColor takenColor;
    if(opposingPiece)
    {
        takeMove = true;
        takenPiece = opposingPiece->objectName();
        takenColor = opposingPiece->color();
        takenHadMoved = opposingPiece->hasMoved();
    }

    int oldX = piece->position().x();
    int oldY = piece->position().y();
    bool hadMoved = piece->hasMoved();
    if(piece->moveTo(QPoint(x, y)))
    {
        Q_ASSERT(!takeMove || takenColor != piece->color());
        MoveAction* moveAction = new MoveAction(piece, oldX, oldY, hadMoved);
        if(takeMove)
            moveAction->setTakenPiece(takenPiece, takenColor, takenHadMoved);
        queueAction(moveAction);
        return true;
    }

    return false;
}

bool Recorder::undo()
{
    bool couldUndo = canUndo();
    bool couldRedo = canRedo();
    if(canUndo())
    {
        actions_.at(nextUndo_)->undo(model_);
        --nextUndo_;
        if(couldUndo != canUndo())
        {
            emit undoChanged(canUndo());
        }
        if(couldRedo != canRedo())
        {
            emit redoChanged(canRedo());
        }
        return true;
    }

    return false;
}

bool Recorder::redo()
{
    bool couldUndo = canUndo();
    bool couldRedo = canRedo();
    if(canRedo())
    {
        actions_.at(nextUndo_ + 1)->redo(model_);
        ++nextUndo_;
        if(couldUndo != canUndo())
        {
            emit undoChanged(canUndo());
        }
        if(couldRedo != canRedo())
        {
            emit redoChanged(canRedo());
        }
        return true;
    }
    return false;
}

bool Recorder::canRedo() const
{
    return nextUndo_ + 1 < actions_.size();
}

bool Recorder::canUndo() const
{
    return nextUndo_ >= 0;
}

void Recorder::restart()
{
    foreach(Action* action, actions_)
    {
       delete action;
    }
    actions_.clear();
    nextUndo_ = -1;
}

void Recorder::setModel(ChessBoardModel* model)
{
    if (model_ == model)
        return;

    model_ = model;
    emit modelChanged(model);
}

bool Recorder::save(const QString& filename)
{
    QString fileNameFromUrl = QUrl(filename).toLocalFile();
    qDebug() << "Save: " << fileNameFromUrl << ", originally:" << filename;
    Writer writer(fileNameFromUrl);
    writer.write(actions_);
    return true;
}


class Recorder::Reader
{
    QList<Recorder::Action*>& historyList_;
public:
    Reader(QList<Recorder::Action*>& historyList)
        : historyList_(historyList)
    {
    }

    bool read(QFile& input)
    {
        Q_ASSERT(input.isOpen());
        historyList_.clear();

        QXmlStreamReader xml(&input);
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

        qWarning() << "No XML elements";
        return false;
    }


    bool readEvents(QXmlStreamReader& xml)
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

    bool readCreate(QXmlStreamReader& xml)
    {
        int x = xml.attributes().value("x").toInt();
        int y = xml.attributes().value("y").toInt();
        QString name = xml.attributes().value("name").toString();
        QString color = xml.attributes().value("color").toString().toLower();
        ChessBoardModel::PieceColor col = colorFromString(color);
        historyList_.append(new CreateAction(name, x, y, col));
        xml.skipCurrentElement();
        return true;
    }

    bool readMove(QXmlStreamReader& xml)
    {
        int x = xml.attributes().value("x").toInt();
        int y = xml.attributes().value("y").toInt();
        int destX = xml.attributes().value("destX").toInt();
        int destY = xml.attributes().value("destY").toInt();
        QString name = xml.attributes().value("name").toString();
        QString color = xml.attributes().value("color").toString();
        bool hadMoved = xml.attributes().value("hasMoved").toInt() == 1;
        ChessBoardModel::PieceColor col = colorFromString(color);

        MoveAction* moveAction = new MoveAction(name, x, y, destX, destY, col, hadMoved);
        if(xml.readNextStartElement())
        {
            if(xml.name() == "takes")
            {
                QString takenName = xml.attributes().value("name").toString();
                QString color = xml.attributes().value("color").toString();
                bool takenHasMoved = xml.attributes().value("hasMoved").toInt() == 1;
                ChessBoardModel::PieceColor takenCol = colorFromString(color);
                moveAction->setTakenPiece(takenName, takenCol, takenHasMoved);
            }
            xml.skipCurrentElement();
        }
        historyList_.append(moveAction);
        xml.skipCurrentElement();
        return true;
    }
};

bool Recorder::load(const QString& filename)
{
    QString fileNameFromUrl = QUrl(filename).toLocalFile();
    QFile f(fileNameFromUrl);
    if(!f.open(QIODevice::ReadOnly))
    {
        qWarning() << "Unable to load from: " << fileNameFromUrl;
        return false;
    }
    Reader reader(actions_);
    reader.read(f);
    nextUndo_ = -1;
    emit undoChanged(false);
    emit redoChanged(actions_.size() > 0);
    return true;
}

void Recorder::queueAction(Recorder::Action* action)
{
    // clear to end of the queue for the new action
    while(nextUndo_ < actions_.size() - 1)
    {
        delete actions_.takeLast();
    }
    Q_ASSERT(nextUndo_ == actions_.size() - 1);
    actions_.push_back(action);
    nextUndo_ = actions_.size() - 1;
    emit undoChanged(true);
    emit redoChanged(false);
}
