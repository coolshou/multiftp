#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QNetworkReply>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include "CurlEasy.h"

class FtpClient : public QObject
{
    Q_OBJECT
public:
    enum FtpMode{ Upload, Download};
    Q_ENUM(FtpMode)

    explicit FtpClient(int id, const QString &server, const QString &username, const QString &password,
                       int port=21, int loop=0, QObject *parent = nullptr);
    ~FtpClient();
    void setFtpMode(FtpMode mode, QString localfile, QString remotefile);
    void connectToFtp(const QString &server, const QString &username, const QString &password);
    void downloadFile(const QString &remoteFile, const QString &localFile);
    void uploadFile(const QString &localFile, const QString &remoteFile);
    void setStop();

public slots:
    void work();

signals:
    void downloadFinished(int id);
    void uploadFinished(int id);
    void progress(int id, qint64 bytesCurrent, qint64 bytesTotal, int percentage);
    void stop(int id);
    void errormsg(int id, QString msg);

private slots:
    void onTransferProgress(qint64 downloadTotal, qint64 downloadNow, qint64 uploadTotal, qint64 uploadNow);
    void onTransferDone();
    void onTransferAborted();

private:
    // QNetworkAccessManager *networkManager;
    void log(QString text);
    QTimer m_timer;
    int m_id;
    CurlEasy *transfer;
    QFile *m_downloadFile = nullptr;
    qint64 m_downloadsize;
    qint64 m_current_downloadsize;
    QFile *m_uploadFile = nullptr;
    qint64 m_uploadsize;
    qint64 m_current_uploadsize;
    QString m_server;
    QString m_username;
    QString m_password;
    int m_port;
    int m_loops; //-1: loops forever, 0: no loops, >0: loops times;
    int m_runtimes;
    FtpMode m_ftpmode;
    QString m_localfile;
    QString m_remotefile;
    QFile *m_file;
    int m_errorcode; // error code
    QString m_error; // error message
    bool m_stop;
};

#endif // FTPCLIENT_H
