#include "ip_cfg.h"
#include <QSettings>
#include <QFile>

#define DEF_FLAG    0xeb90
#define DEF_VENDOR  "CSJ"
#define DEF_NAME    "STAR"
#define DEF_DESC    "MY FIRST STAR"
#define DEF_VERSION "0.0.1"
#define DEF_PATH    ""

IP_Cfg::IP_Cfg(QObject *parent) : QObject(parent)
{
    m_path = "ip_cfg";
}

bool IP_Cfg::setCfgFile(const QString &path)
{
    if(!path.isEmpty())
    {
        if(!QFile::exists(path))
        {
            createDefCfg();
        }
        else    //检测配置文件的有效性
        {
            QSettings*settings=new QSettings(path,QSettings::IniFormat);
            QString s_key = QString::number(CFGFLAG,16);
            int cfgflag = settings->value(s_key).toInt();
            delete settings;

            if(cfgflag != DEF_FLAG)
                return false;
        }
    }
    m_path  = path;
    return true;
}

void IP_Cfg::setCFGValue(IP_Cfg::CFGKEYS key, const QVariant &value)
{
    QSettings*settings=new QSettings(m_path,QSettings::IniFormat);
    QString s_key = QString::number(key,16);
    settings->setValue(s_key,value);
    settings->sync();
    delete settings;
}

void IP_Cfg::createDefCfg()
{
    if(!m_path.isEmpty())
    {
        setCFGValue(CFGFLAG,DEF_FLAG);
        setCFGValue(VENDOR,DEF_VENDOR);
        setCFGValue(APPNAME,DEF_NAME);
        setCFGValue(DESCRIPTION,DEF_DESC);
        setCFGValue(VERISON,DEF_VERSION);
        setCFGValue(PLUGINSPATH,DEF_PATH);
    }
}
