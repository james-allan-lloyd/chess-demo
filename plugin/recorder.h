#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include "chessboardmodel.h"

class Recorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ChessBoardModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    class Action
    {
    public:
        virtual ~Action() {}
        virtual bool undo(ChessBoardModel* model) = 0;
        virtual bool redo(ChessBoardModel* model) = 0;
    };

private:
    ChessBoardModel* model_;
    QList<Action*> actions_;
    int nextUndo_;

public:
    explicit Recorder(QObject* parent = 0);
    ~Recorder();

    ChessBoardModel* model() const;

    Q_INVOKABLE Piece* create(const QString& name, int x, int y, const QString& colorString = QString("black"));

    Q_INVOKABLE bool undo();
    Q_INVOKABLE bool redo();

signals:
    void modelChanged(ChessBoardModel* model);

public slots:
    void restart();

    void setModel(ChessBoardModel* model);
};

#endif // RECORDER_H
