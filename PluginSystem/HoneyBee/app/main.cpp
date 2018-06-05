#include <QCoreApplication>
#include "../externsionsystem/pluginmanager.h"

using namespace ExtensionSystem;

int main(int argc, char *argv[])
{
    PluginManager  g_manager;
    g_manager.managerInit();

    QCoreApplication a(argc, argv);

    return a.exec();
}
