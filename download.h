#pragma once
#include <QUrl>
#include <QString>
#include <QFileInfo>
#include <Qdir>
#include <QStandardPaths>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QWidget>


class download
{


public:
	explicit download(QString  , QUrl );
	void getFileNamePath(QString);
	bool createFile();
	void request(QUrl);
	

	~download();



private:
	download *p_download;


	QString fileNamePath;
	QFileInfo fileInfo;
	QNetworkReply *reply;
	QFile *file;

private slots:
	void read();


};


