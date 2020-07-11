#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

/* Fomat size
 * Accepts: size in number of bytes
 * Returns: formatted size (converted to KiB, MiB, etc.) */
QString formatSize(qint64 size);

/* Returns percent-encoded version of the argument */
QByteArray percentEncode(const QByteArray &data);

#endif // GLOBAL_H
