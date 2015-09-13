#ifndef RECORDERWRITER_H
#define RECORDERWRITER_H

#include <QObject>
#include <QDomDocument>

#include "recorder.h"


class Recorder::Writer : public QObject
{
    QString filename_;
    QDomDocument doc_;
public:
    Writer(const QString& filename);

    const QString filename() const;
    void write(const QList<Action*>& actions);

private:
    void clear();
    void loadDoc();
    void saveDoc();
};

#endif // RECORDERWRITER_H
