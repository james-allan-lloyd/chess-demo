#ifndef RECORDERREADER_H
#define RECORDERREADER_H

#include "recorder.h"

class QXmlStreamReader;
class QFile;

class Recorder::Reader
{
    QList<Recorder::Action*>& historyList_;
public:
    Reader(QList<Recorder::Action*>& historyList);

    bool read(QFile& input);

private:
    bool readEvents(QXmlStreamReader& xml);
    bool readCreate(QXmlStreamReader& xml);
    bool readMove(QXmlStreamReader& xml);
};

#endif // RECORDERREADER_H
