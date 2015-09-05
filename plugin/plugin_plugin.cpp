#include "plugin_plugin.h"
#include "chessboardmodel.h"

#include <qqml.h>

void PluginPlugin::registerTypes(const char *uri)
{
    // @uri com.luxoft.Chess
    qmlRegisterType<ChessBoardModel>(uri, 1, 0, "BoardModel");
}


