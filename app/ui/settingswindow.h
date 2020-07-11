#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class QLineEdit;

class SettingsWindow : public QWidget
{
	Q_OBJECT

public:
	SettingsWindow(QWidget *parent = nullptr);

private slots:
	void apply();
	void reset();

private:
	QLineEdit *m_serverStartPort;
	QLineEdit *m_serverEndPort;
};

#endif // SETTINGSWINDOW_H
