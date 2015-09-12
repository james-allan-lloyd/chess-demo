#include "recorder.h"
#include "piece.h"


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
};


class MoveAction : public Recorder::Action
{
    QString name_;
    int oldX_;
    int oldY_;
    int newX_;
    int newY_;
    ChessBoardModel::PieceColor color_;
    ChessBoardModel::PieceColor takenColor_;
    QString takenName_;
public:
    MoveAction(Piece* piece, int oldX, int oldY)
        : name_(piece->objectName())
        , oldX_(oldX)
        , oldY_(oldY)
        , newX_(piece->position().x())
        , newY_(piece->position().y())
        , color_(piece->color())
    {}


    void setTakenPiece(const QString& name, ChessBoardModel::PieceColor col)
    {
        takenName_ = name;
        takenColor_ = col;
    }

    bool undo(ChessBoardModel* model) override
    {
        // qDebug() << "Undo move " << name_ << x_ << y_;
        Piece* piece = model->cell(newX_, newY_);
        model->removePiece(piece); // because pawn moves are not reversable
        piece = model->create(name_, oldX_, oldY_);
        Q_ASSERT(piece != NULL);

        if(!takenName_.isEmpty())
        {
            model->create(takenName_, newX_, newY_, takenColor_);
        }
        return true;
    }

    bool redo(ChessBoardModel* model) override
    {
        // qDebug() << "Redo creation" << name_ << x_ << y_;
        Piece* piece = model->cell(oldX_, oldY_);
        piece->moveTo(QPoint(newX_, newY_));
        return true;
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

Piece* Recorder::create(const QString& name, int x, int y, const QString& colorString)
{
    if(!model_)
    {
        return NULL;
    }
    if(nextUndo_ != actions_.size() - 1)
    {
        return NULL;
    }
    ChessBoardModel::PieceColor col = colorFromString(colorString);
    Piece* result = model_->create(name, x, y, col);
    if(result)
    {
        actions_.push_back(new CreateAction(name, x, y, col));
        nextUndo_ = actions_.size() - 1;
    }
    return result;
}

bool Recorder::move(Piece* piece, int x, int y)
{
    Piece* opposingPiece = model_->cell(x, y);
    bool takeMove = true;
    QString takenPiece;
    ChessBoardModel::PieceColor takenColor;
    if(opposingPiece)
    {
        takeMove = true;
        takenPiece = opposingPiece->objectName();
        takenColor = opposingPiece->color();
        Q_ASSERT(takenColor != piece->color());
    }

    int oldX = piece->position().x();
    int oldY = piece->position().y();
    if(piece->moveTo(QPoint(x, y)))
    {
        MoveAction* moveAction = new MoveAction(piece, oldX, oldY);
        moveAction->setTakenPiece(takenPiece, takenColor);
        actions_.push_back(moveAction);
        nextUndo_ = actions_.size() - 1;
        return true;
    }

    return false;
}

bool Recorder::undo()
{
    if(nextUndo_ >= 0)
    {
        actions_.at(nextUndo_)->undo(model_);
        --nextUndo_;
        return true;
    }

    return false;
}

bool Recorder::redo()
{
    if(nextUndo_ + 1 < actions_.size())
    {
        actions_.at(nextUndo_ + 1)->redo(model_);
        ++nextUndo_;
        return true;
    }
    return false;
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

