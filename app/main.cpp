#include "qtorrent.h"
#include "core/remote.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    app.setOrganizationName("Bambooster");
    app.setOrganizationDomain("bambooster.com");
    app.setApplicationName("Bambooster");

	Remote remote;
	if (!remote.start()) {
		qDebug() << "Already running";
		return 0;
	}

	QTorrent qTorrent;

	app.exec();
	qTorrent.shutDown();
	return 0;
}
