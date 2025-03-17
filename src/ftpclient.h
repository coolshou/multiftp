#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
// #include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QFile>
#include "CurlEasy.h"

class FtpClient : public QObject
{
    Q_OBJECT
public:
    enum FtpMode{ Upload, Download};
    Q_ENUM(FtpMode)

    explicit FtpClient(const QString &server, const QString &username, const QString &password,
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
    void downloadFinished();
    void uploadFinished();
    void progress(qint64 bytesCurrent, qint64 bytesTotal);
    void stop();

private slots:
    void onTransferProgress(qint64 downloadTotal, qint64 downloadNow, qint64 uploadTotal, qint64 uploadNow);
    void onTransferDone();
    void onTransferAborted();

    void onDownloadFinished();
    void onUploadFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadReadyRead();
    void onDownloadError(QNetworkReply::NetworkError code);
    void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void onUploadError(QNetworkReply::NetworkError code);
    // static size_t readUploadfile(char *buffer, size_t size, size_t nitems, void *stream);
    // static size_t writeDownloadfile(char *buffer, size_t size, size_t nmemb, void *userdata);


private:
    // size_t readUploadfile(char *buffer, size_t size) ;
    // size_t writeDownloadfile(char *buffer, size_t size) ;
    // QNetworkAccessManager *networkManager;
    void log(QString text);
    CurlEasy *transfer;
    QFile *m_downloadFile = nullptr;
    QFile *m_uploadFile = nullptr;
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
