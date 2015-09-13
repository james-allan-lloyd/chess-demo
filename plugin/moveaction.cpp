#include "moveaction.h"

#include "piece.h"

#include <QDomDocument>

MoveAction::MoveAction(Piece* piece, int oldX, int oldY, bool hadMoved)
    : name_(piece->objectName())
    , oldX_(oldX)
    , oldY_(oldY)
    , newX_(piece->position().x())
    , newY_(piece->position().y())
    , color_(piece->color())
    , hadMoved_(hadMoved)
{}

MoveAction::MoveAction(const QString& name, int oldX, int oldY, int newX, int newY, ChessBoardModel::PieceColor color, bool hadMoved)
    : name_(name)
    , oldX_(oldX)
    , oldY_(oldY)
    , newX_(newX)
    , newY_(newY)
    , color_(color)
    , hadMoved_(hadMoved)
{}

void MoveAction::setTakenPiece(const QString& name, ChessBoardModel::PieceColor col, bool hasMoved)
{
    takenName_ = name;
    takenColor_ = col;
    takenHasMoved_ = hasMoved;
}

bool MoveAction::undo(ChessBoardModel* model)
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

bool MoveAction::redo(ChessBoardModel* model)
{
    // qDebug() << "Redo move " << name_ << newX_ << newY_;
    Piece* piece = model->cell(oldX_, oldY_);
    Q_ASSERT(piece);
    bool moved = piece->moveTo(QPoint(newX_, newY_));
    Q_ASSERT(moved);
    return true;
}

void MoveAction::write(QDomDocument& doc)
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
