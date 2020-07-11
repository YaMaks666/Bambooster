#ifndef SETTINGSUTIL_H
#define SETTINGSUTIL_H

#include <QSettings>
#include <QString>
#include <QVariant>

#define SGROUP_WINDOW_GEOMETRY "WindowGeometry"
#define SGROUP_PLAYLIST "Playlist"
#define SGROUP_PLAYER "Player"

#define SKEY_WINDOW_POS "WindowPos"
#define SKEY_WINDOW_SIZE "WindowSize"
#define SKEY_CURR_PLAYLIST_ITEM "CurrPlaylistItem"
#define SKEY_PLAYER_POS "PlayerPosition"
#define SKEY_IS_PLAYER_PAUSED "IsPlayerPaused"
#define SKEY_VOLUME_VALUE "50"

void saveParameter(const QString &key, const QVariant &value,
                   const QString &group);
QVariant loadParameter(const QString &key,  const QString &group,
                       const QVariant &defaultValue);

#endif // SETTINGSUTIL_H
