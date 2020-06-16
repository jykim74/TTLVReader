#include <QSettings>
#include "settings_mgr.h"

namespace  {
const char *kBehaviorGroup = "Behavior";
const char *kShowFullText = "showFullText";
const char *kKMIPHost = "KMIPHost";
const char *kKMIPPort = "KMIPPort";
}

SettingsMgr::SettingsMgr( QObject *parent ) : QObject(parent)
{

}

void SettingsMgr::setShowFullText(bool val)
{
    QSettings settings;

    settings.beginGroup(kBehaviorGroup);
    settings.setValue( kShowFullText, val );
    settings.endGroup();
}

bool SettingsMgr::showFullText()
{
    QSettings settings;

    bool val;

    settings.beginGroup(kBehaviorGroup);
    val = settings.value(kShowFullText, false).toBool();
    settings.endGroup();

    return val;
}

QString SettingsMgr::KMIPHost()
{
    QSettings settings;

    QString strHost;

    settings.beginGroup( kBehaviorGroup );
    strHost = settings.value(kKMIPHost, "127.0.0.1").toString();
    settings.endGroup();

    return strHost;
}

void SettingsMgr::setKMIPHost( const QString strHost )
{
    QSettings settings;

    settings.beginGroup( kBehaviorGroup );
    settings.setValue( kKMIPHost, strHost );
    settings.endGroup();
}

QString SettingsMgr::KMIPPort()
{
    QSettings settings;

    QString strPort;

    settings.beginGroup( kBehaviorGroup );
    strPort = settings.value( kKMIPPort, "5696" ).toString();
    settings.endGroup();

    return strPort;
}

void SettingsMgr::setKMIPPort( const QString strPort )
{
    QSettings settings;

    settings.beginGroup( kBehaviorGroup );
    settings.setValue( kKMIPPort, strPort );
    settings.endGroup();
}
