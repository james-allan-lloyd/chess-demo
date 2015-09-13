#include "recorderreader.h"
#include "moveaction.h"
#include "createaction.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>


Recorder::Reader::Reader(QList<Recorder::Action*>& historyList)
    : historyList_(historyList)
{
}

bool Recorder::Reader::read(QFile& input)
{
    Q_ASSERT(input.isOpen());
    historyList_.clear();

    QXmlStreamReader xml(&input);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "chess")
        {
            return readEvents(xml);
        }
        else
        {
            qWarning() << "Invalid start element" << xml.name();
            return false;
        }
    }

    qWarning() << "No XML elements";
    return false;
}

bool Recorder::Reader::readEvents(QXmlStreamReader& xml)
{
    while(xml.readNextStartElement())
    {
        if(xml.name() == "create")
        {
            readCreate(xml);
        }
        else if(xml.name() == "move")
        {
            readMove(xml);
        }
        else
        {
            xml.skipCurrentElement();
        }
    }
    return true;
}

bool Recorder::Reader::readCreate(QXmlStreamReader& xml)
{
    int x = xml.attributes().value("x").toInt();
    int y = xml.attributes().value("y").toInt();
    QString name = xml.attributes().value("name").toString();
    QString color = xml.attributes().value("color").toString().toLower();
    ChessBoardModel::PieceColor col = colorFromString(color);
    historyList_.append(new CreateAction(name, x, y, col));
    xml.skipCurrentElement();
    return true;
}

bool Recorder::Reader::readMove(QXmlStreamReader& xml)
{
    int x = xml.attributes().value("x").toInt();
    int y = xml.attributes().value("y").toInt();
    int destX = xml.attributes().value("destX").toInt();
    int destY = xml.attributes().value("destY").toInt();
    QString name = xml.attributes().value("name").toString();
    QString color = xml.attributes().value("color").toString();
    bool hadMoved = xml.attributes().value("hasMoved").toInt() == 1;
    ChessBoardModel::PieceColor col = colorFromString(color);

    MoveAction* moveAction = new MoveAction(name, x, y, destX, destY, col, hadMoved);
    while(xml.readNextStartElement())
    {
        if(xml.name() == "takes")
        {
            QString takenName = xml.attributes().value("name").toString();
            QString color = xml.attributes().value("color").toString();
            bool takenHasMoved = xml.attributes().value("hasMoved").toInt() == 1;
            ChessBoardModel::PieceColor takenCol = colorFromString(color);
            moveAction->setTakenPiece(takenName, takenCol, takenHasMoved);
        }

        xml.skipCurrentElement(); //  don't recurse any deeper thanks.
    }

    historyList_.append(moveAction);
    return true;
}
