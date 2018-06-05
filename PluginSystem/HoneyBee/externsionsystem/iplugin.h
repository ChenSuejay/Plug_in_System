#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include <QString>
#include "externsionsystem_global.h"
namespace ExtensionSystem {

class PluginManager;
class PluginSpec;


namespace Internal {
    class PluginSpecPrivate;

    class IPluginPrivate
    {
    public:
        PluginSpec *pluginSpec;
        QList<QObject *> addedObjectsInReverseOrder;
    };
}

class EXTENSIONSYSTEM_EXPORT IPlugin : public QObject
{
    Q_OBJECT

public:
    enum ShutdownFlag {
        SynchronousShutdown,
        AsynchronousShutdown
    };

    IPlugin();
    virtual ~IPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) = 0;
    virtual void extensionsInitialized() = 0;
    virtual bool delayedInitialize() { return false; }
    virtual ShutdownFlag aboutToShutdown() { return SynchronousShutdown; }
    virtual QObject *remoteCommand(const QStringList & /* options */,
                                   const QString & /* workingDirectory */,
                                   const QStringList & /* arguments */) { return 0; }

    PluginSpec *pluginSpec() const;

    void addObject(QObject *obj);
    void addAutoReleasedObject(QObject *obj);
    void removeObject(QObject *obj);

signals:
    void asynchronousShutdownFinished();

private:
    Internal::IPluginPrivate *d;

    friend class Internal::PluginSpecPrivate;
};

} // namespace ExtensionSystem


#endif // IPLUGIN_H
