#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QSet>
#include <QStringList>
#include <QObject>
#include <QScopedPointer>
#include <QReadWriteLock>
#include <QSettings>
#include <QTimer>
#include <QEventLoop>
#include <QTextStream>

#include "pluginspec.h"
namespace ExtensionSystem {

/**
 * Start def class PluginManagerPrivate
 */
class PluginManager;

namespace Internal {

class PluginSpecPrivate;

class EXTENSIONSYSTEM_EXPORT PluginManagerPrivate : public QObject
{
    Q_OBJECT
public:
    PluginManagerPrivate(PluginManager *pluginManager);
    virtual ~PluginManagerPrivate();

    // Object pool operations
    void addObject(QObject *obj);
    void removeObject(QObject *obj);

    // Plugin operations
    void loadPlugins();
    void shutdown();
    void setPluginPaths(const QStringList &paths);
    QList<PluginSpec *> loadQueue();
    void loadPlugin(PluginSpec *spec, PluginSpec::State destState);
    void resolveDependencies();
    void enableDependenciesIndirectly();
    void initProfiling();
    void profilingSummary() const;
    void profilingReport(const char *what, const PluginSpec *spec = 0);
    void setSettings(QSettings *settings);
    void setGlobalSettings(QSettings *settings);
    void readSettings();
    void writeSettings();

    QHash<QString, QList<PluginSpec *>> pluginCategories;
    QList<PluginSpec *> pluginSpecs;

    QStringList pluginPaths;
    QString pluginIID;
    QList<QObject *> allObjects; // ### make this a QList<QPointer<QObject> > > ?
    QStringList defaultDisabledPlugins; // Plugins/Ignored from install settings
    QStringList defaultEnabledPlugins; // Plugins/ForceEnabled from install settings
    QStringList disabledPlugins;
    QStringList forceEnabledPlugins;
    // delayed initialization
    QTimer *delayedInitializeTimer;
    QList<PluginSpec *> delayedInitializeQueue;
    // ansynchronous shutdown
    QList<PluginSpec *> asynchronousPlugins; // plugins that have requested async shutdown
    QEventLoop *shutdownEventLoop; // used for async shutdown

    QStringList arguments;
   // QScopedPointer<QTime> m_profileTimer;
    QHash<const PluginSpec *, int> m_profileTotal;
    int m_profileElapsedMS;
    unsigned m_profilingVerbosity;
    QSettings *settings;
    QSettings *globalSettings;

    // Look in argument descriptions of the specs for the option.
    PluginSpec *pluginForOption(const QString &option, bool *requiresArgument) const;
    PluginSpec *pluginByName(const QString &name) const;

    // used by tests
    static PluginSpec *createSpec();
    static PluginSpecPrivate *privateSpec(PluginSpec *spec);

    mutable QReadWriteLock m_lock;

    bool m_isInitializationDone = false;

private:
    PluginManager *q;

    void nextDelayedInitialize();
    void asyncShutdownFinished();

    void readPluginPaths();
    bool loadQueue(PluginSpec *spec,
            QList<PluginSpec *> &queue,
            QList<PluginSpec *> &circularityCheckQueue);
    void stopAll();
    void deleteAll();

};
} // namespace Internal

/**
 * Start def class PluginManager
 */

class IPlugin;
class PluginSpec;

class EXTENSIONSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT

public:
    static PluginManager *instance();

    PluginManager();
    ~PluginManager();

    bool managerInit();
    // Object pool operations
    static void addObject(QObject *obj);
    static void removeObject(QObject *obj);
    static QList<QObject *> allObjects();
    static QReadWriteLock *listLock();
    template <typename T> static QList<T *> getObjects()
    {
        QReadLocker lock(listLock());
        QList<T *> results;
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            T *result = qobject_cast<T *>(obj);
            if (result)
                results += result;
        }
        return results;
    }
    template <typename T, typename Predicate>
    static QList<T *> getObjects(Predicate predicate)
    {
        QReadLocker lock(listLock());
        QList<T *> results;
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            T *result = qobject_cast<T *>(obj);
            if (result && predicate(result))
                results += result;
        }
        return results;
    }

    // This is useful for soft dependencies using pure interfaces.
    template <typename T> static T *getObject()
    {
        QReadLocker lock(listLock());
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            if (T *result = qobject_cast<T *>(obj))
                return result;
        }
        return 0;
    }
    template <typename T, typename Predicate> static T *getObject(Predicate predicate)
    {
        QReadLocker lock(listLock());
        QList<QObject *> all = allObjects();
        foreach (QObject *obj, all) {
            if (T *result = qobject_cast<T *>(obj))
                if (predicate(result))
                    return result;
        }
        return 0;
    }

    static QObject *getObjectByName(const QString &name);
    static QObject *getObjectByClassName(const QString &className);

    // Plugin operations
    static QList<PluginSpec *> loadQueue();
    static void loadPlugins();
    static QStringList pluginPaths();
    static void setPluginPaths(const QStringList &paths);
    static QString pluginIID();
    static void setPluginIID(const QString &iid);
    static const QList<PluginSpec *> plugins();
    static QHash<QString, QList<PluginSpec *>> pluginCollections();
    static bool hasError();
    static QSet<PluginSpec *> pluginsRequiringPlugin(PluginSpec *spec);
    static QSet<PluginSpec *> pluginsRequiredByPlugin(PluginSpec *spec);

    // Settings
    static void setSettings(QSettings *settings);
    static QSettings *settings();
    static void setGlobalSettings(QSettings *settings);
    static QSettings *globalSettings();
    static void writeSettings();

    // command line arguments
    static QStringList arguments();
    static bool parseOptions(const QStringList &args,
        const QMap<QString, bool> &appOptions,
        QMap<QString, QString> *foundAppOptions,
        QString *errorString);
    static void formatOptions(QTextStream &str, int optionIndentation, int descriptionIndentation);
    static void formatPluginOptions(QTextStream &str, int optionIndentation, int descriptionIndentation);
    static void formatPluginVersions(QTextStream &str);

    static QString serializedArguments();

    static bool testRunRequested();

    static void profilingReport(const char *what, const PluginSpec *spec = 0);

    static QString platformName();

    static bool isInitializationDone();

    void remoteArguments(const QString &serializedArguments, QObject *socket);
    void shutdown();

    QString systemInformation() const;

signals:
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);

    void pluginsChanged();
    void initializationDone();
    void testsFinished(int failedTests);

    friend class Internal::PluginManagerPrivate;
};

} // namespace ExtensionSystem


#endif // PLUGINMANAGER_H
