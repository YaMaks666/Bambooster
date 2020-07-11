#include "settingsutil.h"

void saveParameter(const QString &key, const QVariant &value, const QString &group)
{
    QSettings S;
    S.beginGroup(group);
    S.setValue(key, value);
    S.endGroup();
}

QVariant loadParameter(const QString &key, const QString &group,
                     const QVariant &defaultValue)
{
    QSettings S;
    S.beginGroup(group);
    QVariant v = S.value(key, defaultValue);
    S.endGroup();
    return v;
}
