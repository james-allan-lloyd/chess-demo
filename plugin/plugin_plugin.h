#ifndef PLUGIN_PLUGIN_H
#define PLUGIN_PLUGIN_H

#include <QQmlExtensionPlugin>

class PluginPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // PLUGIN_PLUGIN_H

