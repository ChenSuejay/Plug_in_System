#ifndef IP_CFG_H
#define IP_CFG_H

#include <QObject>
#include <QString>

class IP_Cfg : public QObject
{
    Q_OBJECT
public:
    enum CFGKEYS{CFGFLAG,VENDOR,APPNAME,DESCRIPTION,VERISON,PLUGINSPATH,END};

    explicit IP_Cfg(QObject *parent = nullptr);
    bool setCfgFile(const QString &path);
    void setCFGValue(CFGKEYS key, const QVariant &value);
signals:

public slots:

private:
    void createDefCfg();
    QString m_path;
};

#endif // IP_CFG_H
