#ifndef TORRENTSETTINGS_H
#define TORRENTSETTINGS_H

#include <QString>

class TorrentSettings
{
public:
	TorrentSettings();

	/* Setters */
	void setDownloadLocation(const QString &downloadLocation);
	void setStartImmediately(bool startImmediately);
	void setSkipHashCheck(bool skipHashCheck);

	/* Getters */
	const QString &downloadLocation() const;
	bool startImmediately() const;
	bool skipHashCheck() const;

private:
	QString m_downloadLocation;
	bool m_startImmediately;
	bool m_skipHashCheck;
};

#endif // TORRENTSETTINGS_H
