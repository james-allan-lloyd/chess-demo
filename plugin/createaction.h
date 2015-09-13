#ifndef CREATEACTION_H
#define CREATEACTION_H

#include "recorder.h"


class CreateAction : public Recorder::Action
{
    QString name_;
    int x_;
    int y_;
    ChessBoardModel::PieceColor color_;
public:
    CreateAction(const QString& name, int x, int y, ChessBoardModel::PieceColor color);

    // Action interface
public:
    bool undo(ChessBoardModel* model) override;
    bool redo(ChessBoardModel* model) override;
    void write(QDomDocument& ) override;
};

#endif // CREATEACTION_H
