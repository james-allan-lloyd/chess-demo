#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "recorder.h"


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
    MoveAction(Piece* piece, int oldX, int oldY, bool hadMoved);


    MoveAction(const QString& name,
               int oldX,
               int oldY,
               int newX,
               int newY,
               ChessBoardModel::PieceColor color,
               bool hadMoved);

    void setTakenPiece(const QString& name, ChessBoardModel::PieceColor col, bool hasMoved);

    bool undo(ChessBoardModel* model) override;
    bool redo(ChessBoardModel* model) override;
    void write(QDomDocument& doc) override;
};

#endif // MOVEACTION_H
