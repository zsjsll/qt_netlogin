#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->label->setStyleSheet("color: red");
	ui->label->setAlignment(Qt::AlignCenter);
	QFont font("Microsoft YaHei", 13, 65);
	ui->label->setFont(font);
	ui->label->setText(QStringLiteral("登录中。。"));
	//ui->widget->setStyleSheet("background-color:black");


	//创建托盘图标
	QIcon icon = QIcon(":/new/icon/img/network_256px_542507_easyicon.net.png");
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(icon);
	trayIcon->setToolTip(QStringLiteral("上网登录器"));
	QString titlec = QStringLiteral("上网登录器");
	QString textc = QStringLiteral("点击关闭到托盘区");
	trayIcon->show();
	//弹出气泡提示
	trayIcon->showMessage(titlec, textc, QSystemTrayIcon::Information, 3000);

	minimizeAction = new QAction(QStringLiteral("最小化 (&I)"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
	restoreAction = new QAction(QStringLiteral("还原 (&R)"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	quitAction = new QAction(QStringLiteral("退出 (&Q)"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);
	trayIcon->setContextMenu(trayIconMenu);


	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	        this, SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));


	ini = new QSettings("config.ini", QSettings::IniFormat);
	view = new QWebEngineView(this);

	view->load(QUrl(ini->value("config/address", "http://222.197.192.153/eportal/index.jsp?s=911b6787de37291d0a0bac8d17f57fe6&mac=7837ecae23bfae2487a405c48f840199&vid=3e6544bc1848948f&port=5f4d19bc3742b47d&url=1c8aaf9da2c033d1072be8e29c46c695229ab1c8b1319b6e&userip=493a3b02c3231f2e060a84fc01983123").toString()));
	ini->setValue("config/address", ini->value("config/address", "http://222.197.192.153/eportal/index.jsp?s=911b6787de37291d0a0bac8d17f57fe6&mac=7837ecae23bfae2487a405c48f840199&vid=3e6544bc1848948f&port=5f4d19bc3742b47d&url=1c8aaf9da2c033d1072be8e29c46c695229ab1c8b1319b6e&userip=493a3b02c3231f2e060a84fc01983123").toString());
	//view->show();
	view->hide();

	name = ini->value("config/name", "919200002").toString();
	password = ini->value("config/password", "130910").toString();
	ini->setValue("config/name", name);
	ini->setValue("config/password", password);

	ui->progressBar->setValue(0);
	ui->progressBar->hide();

	timer = new QTimer(this);
	timer->start(1000);
	timerOut = new QTimer(this);


	ui->loginOutButton->setEnabled(false);

	connect(timer, SIGNAL(timeout()), this, SLOT(javaScript()));
	connect(timerOut, SIGNAL(timeout()), this, SLOT(on_loginOutButton_clicked()));


	//connect(this->ui->pushButton,SIGNAL(clicked()), this, SLOT(javaScript()));
	//connect(view, SIGNAL(loadFinished(bool)), this, SLOT(javaScript()));
	connect(view->page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
	        this, SLOT(downloadRequested(QWebEngineDownloadItem*)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
	//view->resize(this->size());
	view->resize(this->width(), this->height() - 50);
	auto y = this->height() - 10;
	ui->progressBar->move(0, y);
	ui->progressBar->setMinimumWidth(this->width());
}

bool MainWindow::getFile(QFileInfo fi)//fi�����룬f������
{
	QString path;

	path = ini->value("download/file", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).toString();
	QString defaultFileName = QFileInfo(path, fi.fileName()).absoluteFilePath();
	file = QFileDialog::getSaveFileName(this, "保存", defaultFileName);
	if (!file.isEmpty())
	{
		fi.setFile(file);
		ini->setValue("download/file", fi.path());
		return true;
	}
	return false;
}

void MainWindow::download(QWebEngineDownloadItem* d) const
{
	d->accept();
	connect(d, SIGNAL(finished()), this, SLOT(finished()));
	connect(d, SIGNAL(downloadProgress(qint64, qint64)), this,SLOT(Progress(qint64,qint64)));
}

void MainWindow::downloadRequested(QWebEngineDownloadItem* download)
{
	fileInfo.setFile(download->path());//ͨ��QWebEngineDownloadItem�ҵ�Ĭ�ϵ����ص�ַ,��fileInfo������
	url = download->url();

	if (fileInfo.absoluteDir().exists())//�жϾ���·������д����·��������
	{
		this->getFile(fileInfo);
		qDebug() << file;
		download->setPath(file);
	}


	if (download->state() == QWebEngineDownloadItem::DownloadRequested && !download->path().isEmpty())
	{
		this->download(download);
	}
}


void MainWindow::finished() const
{
	QThread::msleep(800);

	qDebug() << QStringLiteral("���ؽ�������");
	ui->progressBar->hide();
	ui->progressBar->setValue(0);
	ui->progressBar->setMaximum(0);
}

void MainWindow::Progress(qint64 down, qint64 total) const
{
	ui->progressBar->show();
	ui->progressBar->setMaximum(total);
	ui->progressBar->setValue(down);
}

void MainWindow::javaScript()
{
	auto code = QString("javascript:document.getElementsByName('username')[0].value = '%1';\
javascript:document.getElementsByName('pwd')[0].value ='%2';\
javascript:var oBtn =document.getElementById('loginLink_div');\
javascript:oBtn.click();").arg(name).arg(password);
	trueOrFalse = QString("javascript:document.getElementById('toLogOut').id=='toLogOut';");

	t = [=](const QVariant& v)
		{
			if (v.toBool())
			{
				qDebug() << "logined";
				ui->label->setText(QStringLiteral("登录成功"));
				timer->stop();
				ui->loginInButton->setEnabled(false);
				ui->loginOutButton->setEnabled(true);
			}
			else
			{
				ui->label->setText(QStringLiteral("登录中。。"));
				qDebug() << "logining";
				view->page()->runJavaScript(code);
			}
		};


	view->page()->runJavaScript(
		    trueOrFalse, t
	    );
}

void MainWindow::on_loginInButton_clicked()
{
	timer->start(1000);
	this->javaScript();
}

void MainWindow::on_loginOutButton_clicked()
{
	timerOut->start(5000);
	auto out = [=](const QVariant& v)
		{
			if (v.toBool())
			{
				qDebug() << "outed";
				view->page()->runJavaScript("javascript:document.getElementById('offlineDiv').click();");
				timerOut->stop();
				ui->loginInButton->setEnabled(true);
				ui->loginOutButton->setEnabled(false);
				ui->label->setText(QStringLiteral("下线成功"));
			}
			else
			{
				ui->label->setText(QStringLiteral("下线中"));
				qDebug() << "outing";
				view->page()->runJavaScript("javascript:document.getElementById('toLogOut').click();");
			}
		};

	//	offlineDiv
	auto test = "javascript:document.getElementById('offlineDiv').id=='offlineDiv';";


	view->page()->runJavaScript(test, out);
}

void MainWindow::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		//单击托盘图标
		break;
	case QSystemTrayIcon::DoubleClick:
		//双击托盘图标
		this->showNormal();
		this->raise();
		break;
	default:
		break;
	}
}


