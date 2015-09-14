#include "recorderwriter.h"

#include <QFile>
#include <QDebug>
#include <QXmlStreamWriter>

Recorder::Writer::Writer(const QString& filename)
    : filename_(filename)
    , doc_("currentGame")
{
}

const QString& Recorder::Writer::filename() const
{
    return filename_;
}

bool Recorder::Writer::write(const QList<Action*>& actions)
{
    doc_.clear();
    QDomElement chess = doc_.createElement("chess");
    chess.setAttribute("version", "1.0");
    doc_.appendChild(chess);
    foreach(Action* action, actions)
    {
        action->write(doc_);
    }

    QFile file(filename_);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Not able to save doc";
        return false;
    }
    QTextStream textStream(&file);
    doc_.save(textStream, 4);
    return true;
}
