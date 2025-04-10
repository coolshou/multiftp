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

    void start();
    void stop();
    int count();
    void clear();
public slots:
    void onAddFtpClient(int id, const QString &server, int port,
                      const QString &username, const QString &password,
                      QString localfile, QString remotefile, FtpClient::FtpMode mode,
                      int loop=0);
private slots:
    void onDownloadFinished(int id);
    void onUploadFinished(int id);
    void onProgress(int id, qint64 bytesCurrent, qint64 bytesTotal, int percentage);
    void onThroughput(int id, double value);
    void onErrorMsg(int id, QString msg);
    void onStop(int id);
signals:
    void progress(int id, qint64 bytesCurrent, qint64 bytesTotal, int percentage);
    void throughput(int id, double value);
    void errormsg(int id, QString msg);
    void started();
    void stoped();

private:
    QVector<QThread*> threads;
    QVector<FtpClient*> clients;
    QVector<int> runnings;
};

#endif // FTPMANAGER_H
