#include "pluginmanager.h"
#include <QDir>
#include <QFile>
#include <QLibrary>
#include <QMutex>
namespace ExtensionSystem {

using namespace Internal;

static Internal::PluginManagerPrivate *d = NULL;
static PluginManager *m_instance = NULL;

/*!
    Gets the unique plugin manager instance.
*/
PluginManager *PluginManager::instance()
{
    static QMutex mutex;
    if (!m_instance)
    {
        QMutexLocker locker(&mutex);
        if (!m_instance)
        {
            m_instance = new PluginManager();
        }
    }
    return m_instance;
}

/*!
    Creates a plugin manager. Should be done only once per application.
*/
PluginManager::PluginManager()
{
    m_instance = this;
    d = new PluginManagerPrivate(this);
}

/*!
    \internal
*/
PluginManager::~PluginManager()
{
    delete d;
    d = NULL;
}

bool PluginManager::managerInit()
{
    /** load plugin desc,by Dependency */

    /** load plugin **/

    /** start plugin**/
    return true;
}

void PluginManager::addObject(QObject *obj)
{
    d->addObject(obj);
}

void PluginManager::removeObject(QObject *obj)
{
    d->removeObject(obj);
}

QList<QObject *> PluginManager::allObjects()
{
    return d->allObjects;
}

QReadWriteLock *PluginManager::listLock()
{
    return &d->m_lock;
}

void PluginManager::loadPlugins()
{
    d->loadPlugins();
}

/*!
    Returns true if any plugin has errors even though it is enabled.
    Most useful to call after loadPlugins().
*/
bool PluginManager::hasError()
{

}

/*!
    Returns all plugins that require \a spec to be loaded. Recurses into dependencies.
 */
QSet<PluginSpec *> PluginManager::pluginsRequiringPlugin(PluginSpec *spec)
{

}

/*!
    Returns all plugins that \a spec requires to be loaded. Recurses into dependencies.
 */
QSet<PluginSpec *> PluginManager::pluginsRequiredByPlugin(PluginSpec *spec)
{

}

/*!
    Shuts down and deletes all plugins.
*/
void PluginManager::shutdown()
{
    d->shutdown();
}

static QString filled(const QString &s, int min)
{
    return s + QString(qMax(0, min - s.size()), ' ');
}

QString PluginManager::systemInformation() const
{

}

QStringList PluginManager::pluginPaths()
{
    return d->pluginPaths;
}

void PluginManager::setPluginPaths(const QStringList &paths)
{
    d->setPluginPaths(paths);
}

/*!
    The IID that valid plugins must have.

    \sa setPluginIID()
*/
QString PluginManager::pluginIID()
{
    return d->pluginIID;
}

/*!
    Sets the IID that valid plugins must have. Only plugins with this IID are loaded, others are
    silently ignored.

    At the moment this must be called before setPluginPaths() is called.
    // ### TODO let this + setPluginPaths read the plugin meta data lazyly whenever loadPlugins() or plugins() is called.
*/
void PluginManager::setPluginIID(const QString &iid)
{
    d->pluginIID = iid;
}

/*!
    Defines the user specific settings to use for information about enabled and
    disabled plugins.
    Needs to be set before the plugin search path is set with setPluginPaths().
*/
void PluginManager::setSettings(QSettings *settings)
{
    d->setSettings(settings);
}

/*!
    Defines the global (user-independent) settings to use for information about
    default disabled plugins.
    Needs to be set before the plugin search path is set with setPluginPaths().
*/
void PluginManager::setGlobalSettings(QSettings *settings)
{
    d->setGlobalSettings(settings);
}

/*!
    Returns the user specific settings used for information about enabled and
    disabled plugins.
*/
QSettings *PluginManager::settings()
{
    return d->settings;
}

/*!
    Returns the global (user-independent) settings used for information about default disabled plugins.
*/
QSettings *PluginManager::globalSettings()
{
    return d->globalSettings;
}

void PluginManager::writeSettings()
{
    d->writeSettings();
}

/*!
    The arguments left over after parsing (that were neither startup nor plugin
    arguments). Typically, this will be the list of files to open.
*/
QStringList PluginManager::arguments()
{
    return d->arguments;
}

/*!
    List of all plugin specifications that have been found in the plugin search paths.
    This list is valid directly after the setPluginPaths() call.
    The plugin specifications contain the information from the plugins' xml description files
    and the current state of the plugins. If a plugin's library has been already successfully loaded,
    the plugin specification has a reference to the created plugin instance as well.

    \sa setPluginPaths()
*/
const QList<PluginSpec *> PluginManager::plugins()
{
    return d->pluginSpecs;
}

QHash<QString, QList<PluginSpec *> > PluginManager::pluginCollections()
{
    return d->pluginCategories;
}

static const char argumentKeywordC[] = ":arguments";
static const char pwdKeywordC[] = ":pwd";

/*!
    Serializes plugin options and arguments for sending in a single string
    via QtSingleApplication:
    ":myplugin|-option1|-option2|:arguments|argument1|argument2",
    as a list of lists started by a keyword with a colon. Arguments are last.

    \sa setPluginPaths()
*/
QString PluginManager::serializedArguments()
{
    const QChar separator = QLatin1Char('|');
    QString rc;

    return rc;
}

/* Extract a sublist from the serialized arguments
 * indicated by a keyword starting with a colon indicator:
 * ":a,i1,i2,:b:i3,i4" with ":a" -> "i1,i2"
 */
static QStringList subList(const QStringList &in, const QString &key)
{
    QStringList rc;
    // Find keyword and copy arguments until end or next keyword
    const QStringList::const_iterator inEnd = in.constEnd();
    QStringList::const_iterator it = std::find(in.constBegin(), inEnd, key);
    if (it != inEnd) {
        const QChar nextIndicator = QLatin1Char(':');
        for (++it; it != inEnd && !it->startsWith(nextIndicator); ++it)
            rc.append(*it);
    }
    return rc;
}

/*!
    Parses the options encoded by serializedArguments() const
    and passes them on to the respective plugins along with the arguments.

    \a socket is passed for disconnecting the peer when the operation is done (for example,
    document is closed) for supporting the -block flag.
*/

void PluginManager::remoteArguments(const QString &serializedArgument, QObject *socket)
{

}


bool PluginManager::parseOptions(const QStringList &args,
    const QMap<QString, bool> &appOptions,
    QMap<QString, QString> *foundAppOptions,
    QString *errorString)
{

}



static inline void indent(QTextStream &str, int indent)
{
    str << QString(indent, ' ');
}

static inline void formatOption(QTextStream &str,
                                const QString &opt, const QString &parm, const QString &description,
                                int optionIndentation, int descriptionIndentation)
{
    int remainingIndent = descriptionIndentation - optionIndentation - opt.size();
    indent(str, optionIndentation);
    str << opt;
    if (!parm.isEmpty()) {
        str << " <" << parm << '>';
        remainingIndent -= 3 + parm.size();
    }
    if (remainingIndent >= 1) {
        indent(str, remainingIndent);
    } else {
        str << '\n';
        indent(str, descriptionIndentation);
    }
    str << description << '\n';
}

/*!
    Formats the startup options of the plugin manager for command line help.
*/

void PluginManager::formatOptions(QTextStream &str, int optionIndentation, int descriptionIndentation)
{

}

/*!
    Formats the plugin options of the plugin specs for command line help.
*/

void PluginManager::formatPluginOptions(QTextStream &str, int optionIndentation, int descriptionIndentation)
{

}

/*!
    Formats the version of the plugin specs for command line help.
*/
void PluginManager::formatPluginVersions(QTextStream &str)
{

}

/*!
 * \internal
 */
bool PluginManager::testRunRequested()
{

}

/*!
    Creates a profiling entry showing the elapsed time if profiling is
    activated.
*/

void PluginManager::profilingReport(const char *what, const PluginSpec *spec)
{
    d->profilingReport(what, spec);
}


/*!
    Returns a list of plugins in load order.
*/
QList<PluginSpec *> PluginManager::loadQueue()
{
    return d->loadQueue();
}

//============PluginManagerPrivate===========

/*!
    \internal
*/
PluginSpec *PluginManagerPrivate::createSpec()
{
    return new PluginSpec();
}

/*!
    \internal
*/
void PluginManagerPrivate::setSettings(QSettings *s)
{
    if (settings)
        delete settings;
    settings = s;
    if (settings)
        settings->setParent(this);
}

/*!
    \internal
*/
void PluginManagerPrivate::setGlobalSettings(QSettings *s)
{
    if (globalSettings)
        delete globalSettings;
    globalSettings = s;
    if (globalSettings)
        globalSettings->setParent(this);
}

/*!
    \internal
*/
PluginSpecPrivate *PluginManagerPrivate::privateSpec(PluginSpec *spec)
{
    return spec->d;
}

void PluginManagerPrivate::nextDelayedInitialize()
{

}

/*!
    \internal
*/
PluginManagerPrivate::PluginManagerPrivate(PluginManager *pluginManager) :
    delayedInitializeTimer(0),
    shutdownEventLoop(0),
    m_profileElapsedMS(0),
    m_profilingVerbosity(0),
    settings(0),
    globalSettings(0),
    q(pluginManager)
{
}


/*!
    \internal
*/
PluginManagerPrivate::~PluginManagerPrivate()
{
    qDeleteAll(pluginSpecs);
}


/*!
    \internal
*/
void PluginManagerPrivate::writeSettings()
{
    if (!settings)
        return;

}

/*!
    \internal
*/
void PluginManagerPrivate::readSettings()
{

}

/*!
    \internal
*/
void PluginManagerPrivate::stopAll()
{
    if (delayedInitializeTimer && delayedInitializeTimer->isActive()) {
        delayedInitializeTimer->stop();
        delete delayedInitializeTimer;
        delayedInitializeTimer = 0;
    }
    QList<PluginSpec *> queue = loadQueue();
    foreach (PluginSpec *spec, queue) {
        loadPlugin(spec, PluginSpec::Stopped);
    }
}

/*!
    \internal
*/
void PluginManagerPrivate::deleteAll()
{

}

/*!
    \internal
*/
void PluginManagerPrivate::addObject(QObject *obj)
{

}

/*!
    \internal
*/
void PluginManagerPrivate::removeObject(QObject *obj)
{

}

/*!
    \internal
*/
void PluginManagerPrivate::loadPlugins()
{

}

/*!
    \internal
*/
void PluginManagerPrivate::shutdown()
{
    stopAll();
    if (!asynchronousPlugins.isEmpty()) {
        shutdownEventLoop = new QEventLoop;
        shutdownEventLoop->exec();
    }
    deleteAll();
    if (!allObjects.isEmpty()) {
       // qDebug() << "There are" << allObjects.size() << "objects left in the plugin manager pool.";
        // Intentionally split debug info here, since in case the list contains
        // already deleted object we get at least the info about the number of objects;
       // qDebug() << "The following objects left in the plugin manager pool:" << allObjects;
    }
}

/*!
    \internal
*/
void PluginManagerPrivate::asyncShutdownFinished()
{

}

/*!
    \internal
*/
QList<PluginSpec *> PluginManagerPrivate::loadQueue()
{
    QList<PluginSpec *> queue;
    foreach (PluginSpec *spec, pluginSpecs) {
        QList<PluginSpec *> circularityCheckQueue;
        loadQueue(spec, queue, circularityCheckQueue);
    }
    return queue;
}

/*!
    \internal
*/
bool PluginManagerPrivate::loadQueue(PluginSpec *spec, QList<PluginSpec *> &queue,
        QList<PluginSpec *> &circularityCheckQueue)
{

    return true;
}

/*!
    \internal
*/
void PluginManagerPrivate::loadPlugin(PluginSpec *spec, PluginSpec::State destState)
{

}

/*!
    \internal
*/
void PluginManagerPrivate::setPluginPaths(const QStringList &paths)
{
    pluginPaths = paths;
    readSettings();
    readPluginPaths();
}

static QStringList pluginFiles(const QStringList &pluginPaths)
{
    QStringList pluginFiles;
    QStringList searchPaths = pluginPaths;

    return pluginFiles;
}

/*!
    \internal
*/
void PluginManagerPrivate::readPluginPaths()
{

}

void PluginManagerPrivate::resolveDependencies()
{

}

void PluginManagerPrivate::enableDependenciesIndirectly()
{

}

// Look in argument descriptions of the specs for the option.
PluginSpec *PluginManagerPrivate::pluginForOption(const QString &option, bool *requiresArgument) const
{

}

PluginSpec *PluginManagerPrivate::pluginByName(const QString &name) const
{

}

void PluginManagerPrivate::initProfiling()
{

}

void PluginManagerPrivate::profilingReport(const char *what, const PluginSpec *spec /* = 0 */)
{

}

void PluginManagerPrivate::profilingSummary() const
{
    //if (!m_profileTimer.isNull()) {
        QMultiMap<int, const PluginSpec *> sorter;
        int total = 0;

        auto totalEnd = m_profileTotal.constEnd();
        for (auto it = m_profileTotal.constBegin(); it != totalEnd; ++it) {
            sorter.insert(it.value(), it.key());
            total += it.value();
        }

        auto sorterEnd = sorter.constEnd();
        for (auto it = sorter.constBegin(); it != sorterEnd; ++it)
            qDebug("%-22s %8dms   ( %5.2f%% )", qPrintable(it.value()->name()),
                it.key(), 100.0 * it.key() / total);
         qDebug("Total: %8dms", total);

    //}
}

static inline QString getPlatformName()
{

}

QString PluginManager::platformName()
{
    static const QString result = getPlatformName();
    return result;
}

bool PluginManager::isInitializationDone()
{
    return d->m_isInitializationDone;
}

/*!
    Retrieves one object with \a name from the object pool.
    \sa addObject()
*/

QObject *PluginManager::getObjectByName(const QString &name)
{

}

/*!
    Retrieves one object inheriting a class with \a className from the object
    pool.
    \sa addObject()
*/

QObject *PluginManager::getObjectByClassName(const QString &className)
{

}

} // ExtensionSystem
