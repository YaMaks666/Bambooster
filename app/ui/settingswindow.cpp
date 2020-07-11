#include "settingswindow.h"
#include "qtorrent.h"
#include "core/torrentserver.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QSettings>
#include <QMessageBox>
#include <QGuiApplication>

SettingsWindow::SettingsWindow(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

	m_serverStartPort = new QLineEdit;
	m_serverEndPort = new QLineEdit;
	m_serverStartPort->setValidator(new QIntValidator(0, 49151));
	m_serverEndPort->setValidator(new QIntValidator(0, 49151));
	m_serverStartPort->setMaximumWidth(200);
	m_serverEndPort->setMaximumWidth(200);

	QHBoxLayout *serverPortLayout = new QHBoxLayout;
	serverPortLayout->addWidget(new QLabel(tr("Server port range: ")));
	serverPortLayout->addWidget(m_serverStartPort);
	serverPortLayout->addWidget(new QLabel(tr(" to ")));
	serverPortLayout->addWidget(m_serverEndPort);
	serverPortLayout->addStretch();

	mainLayout->addLayout(serverPortLayout);

	QPushButton *applyButton = new QPushButton(tr("Apply"));
	QPushButton *cancelButton = new QPushButton(tr("Cancel"));
	applyButton->setMaximumWidth(200);
	cancelButton->setMaximumWidth(200);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(applyButton);
	buttonsLayout->addWidget(cancelButton);
	buttonsLayout->addStretch();

	mainLayout->addLayout(buttonsLayout);

	mainLayout->addStretch();

	connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::apply);
	connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::reset);

	reset();
}

void SettingsWindow::apply()
{
	bool ok;

	quint16 serverStartPort = m_serverStartPort->text().toInt(&ok);
	if (!ok) {
		QMessageBox::warning(this, QGuiApplication::applicationDisplayName(), tr("Please enter a valid start port"));
		return;
	}

	quint16 serverEndPort = m_serverEndPort->text().toInt(&ok);
	if (!ok) {
		QMessageBox::warning(this, QGuiApplication::applicationDisplayName(), tr("Please enter a valid end port"));
		return;
	}

	QSettings settings;
	settings.setValue("ServerStartPort", serverStartPort);
	settings.setValue("ServerEndPort", serverEndPort);

	// Restart the server
	QTorrent::instance()->server()->startServer();
}

void SettingsWindow::reset()
{
	QSettings settings;
	m_serverStartPort->setText(settings.value("ServerStartPort").toString());
	m_serverEndPort->setText(settings.value("ServerEndPort").toString());
}
