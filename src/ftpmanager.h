#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QString>

#include "ftpclient.h"

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = nullptr);
    ~FtpManager();
    void addFtpClient(int id, const QString &server, int port,
                      const QString &username, const QString &password,
                      QString localfile, QString remotefile, FtpClient::FtpMode mode);
    void start();
    void stop();
    int count();
    void clear();
private slots:
    void onDownloadFinished(int id);
    void onUploadFinished(int id);
    void onProgress(int id, qint64 bytesCurrent, qint64 bytesTotal);
    void onErrorMsg(int id, QString msg);
signals:
    void progress(int id, qint64 bytesCurrent, qint64 bytesTotal);
    void errormsg(int id, QString msg);

private:
    QVector<QThread*> threads;
    QVector<FtpClient*> clients;
};

#endif // FTPMANAGER_H
