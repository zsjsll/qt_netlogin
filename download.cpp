#include "download.h"
#include <QFileDialog>


download::download(QString filePath, QUrl url)
{
	getFileNamePath(filePath);
	createFile();
	request(url);
	read();
}

void download::getFileNamePath(QString filePath)
{
	fileInfo.setFile(filePath);
	if (fileInfo.absoluteDir().exists())
	{
		auto location= QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
		auto fileName=fileInfo.fileName();
		auto newFileNamePath=QFileInfo(location, fileName).absoluteFilePath();
		fileNamePath = QFileDialog::getSaveFileName(this, QStringLiteral("±£´æ"), newFileNamePath);
		return ;

	}
}

bool download::createFile()
{
	file = new QFile(fileNamePath);
	file->open(QIODevice::WriteOnly);

	if (!file->open(QIODevice::WriteOnly))
	{
		
		delete file;
		file = nullptr;
		return false;
	}
	return true;
}

void download::request( QUrl url)
{
	
	QNetworkAccessManager *accessManger = new QNetworkAccessManager(this);
	
	reply = accessManger->get(QNetworkRequest(url));

	connect(reply, SIGNAL(readyRead()), this, SLOT(read()));
}



void download::read()
{
	if (file)
	{
		file->write(reply->readAll());
	}
}

