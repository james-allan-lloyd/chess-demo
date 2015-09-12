#include "recorder.h"


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
        model->removePiece(model->cell(QPoint(x_, y_)));
        return true;
    }

    bool redo(ChessBoardModel* model) override
    {
        Piece* piece = model->create(name_, x_, y_, color_);
        return piece != NULL;
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
    ChessBoardModel::PieceColor col = colorFromString(colorString);
    Piece* result = model_->create(name, x, y, col);
    if(result)
    {
        actions_.push_back(new CreateAction(name, x, y, col));
        nextUndo_ = actions_.size() - 1;
    }
    return result;
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

