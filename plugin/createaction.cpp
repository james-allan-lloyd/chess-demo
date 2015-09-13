#include "createaction.h"


CreateAction::CreateAction(const QString& name, int x, int y, ChessBoardModel::PieceColor color)
    : name_(name)
    , x_(x)
    , y_(y)
    , color_(color)
{
}

bool CreateAction::undo(ChessBoardModel* model)
{
    // qDebug() << "Undo creation " << name_ << x_ << y_;
    Piece* piece = model->cell(QPoint(x_, y_));
    Q_ASSERT(piece);
    model->removePiece(piece);
    Q_ASSERT(!model->cell(QPoint(x_, y_)));
    return true;
}

bool CreateAction::redo(ChessBoardModel* model)
{
    // qDebug() << "Redo creation" << name_ << x_ << y_;
    Piece* piece = model->create(name_, x_, y_, color_);
    return piece != NULL;
}

void CreateAction::write(QDomDocument&)
{
    // TODO:
}
