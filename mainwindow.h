#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QResizeEvent>
#include <QWebEngineDownloadItem>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QWebEngineView>
#include <QFile>
#include <QTimer>
#include <functional>
#include <QSystemTrayIcon>
#include <QCloseEvent>


//#include <QWebEngineView>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

	bool getFile(QFileInfo);
	void download(QWebEngineDownloadItem*) const;


protected:
	void resizeEvent(QResizeEvent*) override;


private slots:
	void downloadRequested(QWebEngineDownloadItem*);
	void javaScript();

	void finished() const;
	void Progress(qint64, qint64) const;


    void on_loginInButton_clicked();

    void on_loginOutButton_clicked();

	void trayiconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
	//void closeEvent(QCloseEvent *);

private:
	Ui::MainWindow* ui;
	QWebEngineView* view;
	QFileInfo fileInfo;
	QString file;
	QUrl url;
	QSettings* ini;
	QTimer* timer;
	QString name;
	QString password;
	QString trueOrFalse;
	std::function<void(QVariant)> t;
	QTimer* timerOut;

	QSystemTrayIcon *trayIcon;
	QAction *minimizeAction;
	QAction *restoreAction;
	QAction *quitAction;
	QMenu   *trayIconMenu;

};

#endif // MAINWINDOW_H
