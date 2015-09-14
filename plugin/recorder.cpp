#include "recorder.h"
#include "recorderwriter.h"
#include "recorderreader.h"
#include "moveaction.h"
#include "createaction.h"
#include "piece.h"

#include <QUrl>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>


Recorder::Recorder(QObject* parent)
    : QObject(parent)
    , model_(NULL)
    , nextUndo_(-1)
    // , lastDirectory_(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/chess")
    , lastDirectory_(qApp->applicationDirPath())
{
    // if(!QDir(lastDirectory_).exists())
    // {
    //     QDir().mkpath(lastDirectory_);
    // }
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
    bool takeMove = false;
    bool takenHadMoved = true;
    QString takenPiece;
    ChessBoardModel::PieceColor takenColor = ChessBoardModel::BLACK;
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
    Writer writer(fileNameFromUrl);
    writer.write(actions_);
    return true;
}



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

QString Recorder::lastDirectory() const
{
    return QUrl::fromLocalFile(lastDirectory_).toString();
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
