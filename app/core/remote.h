#ifndef REMOTE_H
#define REMOTE_H

#include <QObject>

class QLocalSocket;
class QLocalServer;

/* This class is used, so that the application won't be started twice
 * It uses QLocalServer and QLocalSocket. When the application is
 * started for the second time, it sends a message to the first one
 * to show the main window.
 * It could be used for remote access to the client if modified a little
 */
class Remote : public QObject
{
	Q_OBJECT

public:
	Remote();
	~Remote();

	bool start();

	void sendShowWindow();
	void showWindow();

public slots:
	void newConnection();
	void disconnected();
	void readyRead();

private:
	QLocalServer *m_server;
	QLocalSocket *m_socket;
	QByteArray m_buffer;

	void readMessages();
};

#endif // REMOTE_H
