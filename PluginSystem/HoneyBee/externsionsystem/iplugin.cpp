#include "iplugin.h"
#include "pluginmanager.h"
#include "pluginspec.h"
#include "externsionsystem_global.h"
using namespace ExtensionSystem;


IPlugin::IPlugin()
    : d(new Internal::IPluginPrivate())
{
}

IPlugin::~IPlugin()
{
    foreach (QObject *obj, d->addedObjectsInReverseOrder)
        PluginManager::removeObject(obj);
    qDeleteAll(d->addedObjectsInReverseOrder);
    d->addedObjectsInReverseOrder.clear();
    delete d;
    d = 0;
}

/*!
    \fn PluginSpec *IPlugin::pluginSpec() const
    Returns the PluginSpec corresponding to this plugin.
    This is not available in the constructor.
*/
PluginSpec *IPlugin::pluginSpec() const
{
    return d->pluginSpec;
}

void IPlugin::addObject(QObject *obj)
{
    PluginManager::addObject(obj);
}

void IPlugin::addAutoReleasedObject(QObject *obj)
{
    d->addedObjectsInReverseOrder.prepend(obj);
    PluginManager::addObject(obj);
}

/*!
    \fn void IPlugin::removeObject(QObject *obj)
    Convenience function that unregisters \a obj from the plugin manager's
    plugin pool by just calling PluginManager::removeObject().
*/
void IPlugin::removeObject(QObject *obj)
{
    PluginManager::removeObject(obj);
}
