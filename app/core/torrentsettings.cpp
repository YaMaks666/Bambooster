#include "torrentsettings.h"

TorrentSettings::TorrentSettings()
{
}


void TorrentSettings::setDownloadLocation(const QString &downloadLocation)
{
	m_downloadLocation = downloadLocation;
}

void TorrentSettings::setStartImmediately(bool startImmediately)
{
	m_startImmediately = startImmediately;
}

void TorrentSettings::setSkipHashCheck(bool skipHashCheck)
{
	m_skipHashCheck = skipHashCheck;
}


const QString &TorrentSettings::downloadLocation() const
{
	return m_downloadLocation;
}

bool TorrentSettings::startImmediately() const
{
	return m_startImmediately;
}

bool TorrentSettings::skipHashCheck() const
{
	return m_skipHashCheck;
}
