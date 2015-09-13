#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>
#include <QUrl>
#include "chessboardmodel.h"

class QDomDocument;

class Recorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ChessBoardModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY redoChanged)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY undoChanged)
    Q_PROPERTY(QString lastDirectory READ lastDirectory NOTIFY lastDirectoryChanged)

    class Writer;
    class Reader;
public:
    class Action
    {
    public:
        virtual ~Action() {}
        virtual bool undo(ChessBoardModel* model) = 0;
        virtual bool redo(ChessBoardModel* model) = 0;
        virtual void write(QDomDocument& doc) = 0;
    };

    explicit Recorder(QObject* parent = 0);
    ~Recorder();

    ChessBoardModel* model() const;

    Q_INVOKABLE Piece* create(const QString& name, int x, int y, const QString& colorString = QString("black"));
    Q_INVOKABLE bool move(Piece* piece, int x, int y);

    Q_INVOKABLE bool undo();
    Q_INVOKABLE bool redo();

    bool canRedo() const;
    bool canUndo() const;

    Q_INVOKABLE bool save(const QString& filename);
    Q_INVOKABLE bool load(const QString& filename);

    QString lastDirectory() const;

signals:
    void modelChanged(ChessBoardModel* model);

    void redoChanged(bool canRedo);
    void undoChanged(bool canUndo);

    void lastDirectoryChanged(QString lastDirectory);

public slots:
    void restart();

    void setModel(ChessBoardModel* model);


private:
    void queueAction(Action* action);

    ChessBoardModel* model_;
    QList<Action*> actions_;
    int nextUndo_;
    QString lastDirectory_;
};

#endif // RECORDER_H
