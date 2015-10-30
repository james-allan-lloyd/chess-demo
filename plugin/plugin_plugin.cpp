#include "plugin_plugin.h"
#include "chessboardmodel.h"
#include "piece.h"
#include "recorder.h"

#include <qqml.h>

void PluginPlugin::registerTypes(const char *uri)
{
    // @uri com.vividcode.Chess
    qmlRegisterType<ChessBoardModel>(uri, 1, 0, "BoardModel");
    qmlRegisterType<Recorder>(uri, 1, 0, "Recorder");
    qmlRegisterInterface<Piece>("Piece");
}


