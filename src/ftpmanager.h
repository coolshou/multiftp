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
private:
    QVector<QThread*> threads;
    QVector<FtpClient*> clients;
};

#endif // FTPMANAGER_H
