#include "plugin_plugin.h"
#include "chess.h"

#include <qqml.h>

void PluginPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterType<Chess>(uri, 1, 0, "Chess");
}


