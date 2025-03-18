#include "ftpclient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QIODevice>

#include <QDebug>

FtpClient::FtpClient(int id, const QString &server, const QString &username,
                     const QString &password,
                     int port,int loops, QObject *parent)
    : QObject{parent}, m_id(id), m_server(server), m_username(username),
    m_password(password), m_port(port), m_loops(loops)
{
    m_runtimes = 0;
    m_errorcode = 0;
    m_stop = false;
    // networkManager = new QNetworkAccessManager(this);
    transfer = new CurlEasy(this);
    connect(transfer, &CurlEasy::done, this, &FtpClient::onTransferDone);
    connect(transfer, &CurlEasy::aborted, this, &FtpClient::onTransferAborted);
    connect(transfer, &CurlEasy::progress, this, &FtpClient::onTransferProgress);
}
FtpClient::~FtpClient() {
    // delete networkManager;
}

void FtpClient::setFtpMode(FtpMode mode, QString localfile, QString remotefile)
{
    m_ftpmode = mode;
    m_localfile = localfile;
    m_remotefile = remotefile;
}

void FtpClient::connectToFtp(const QString &server, const QString &username, const QString &password) {
    //TODO?? Implement connection logic here
    Q_UNUSED(server)
    Q_UNUSED(username)
    Q_UNUSED(password)
}

void FtpClient::downloadFile(const QString &remoteFile, const QString &localFile) {
    //TODO: remove exist local file
    m_downloadFile = new QFile(localFile);
    if (!m_downloadFile->open(QIODevice::WriteOnly)) {
        delete m_downloadFile;
        m_downloadFile = nullptr;
        return;
    }
    QString url = QString("ftp://%1/%2").arg(m_server, remoteFile);
    qDebug()  << "downloadFile: '" << url<< "' TO " << localFile ;

    QUrl ftpUrl(url);
    // // Set a simple file writing function
    // transfer->setWriteFunction([this](char *data, size_t size)->size_t {
    //     qint64 bytesWritten = m_downloadFile->write(data, static_cast<qint64>(size));
    //     return static_cast<size_t>(bytesWritten);
    // });

    // Print headers to the transfer log box
    transfer->setHeaderFunction([this](char *data, size_t size)->size_t {
        log(QString::fromUtf8(data, static_cast<int>(size)));
        return size;
    });

    // transfer->setWriteFunction(writeDownloadfile);
    transfer->setWriteFunction([this](char *data, size_t size) -> size_t {
        // if (!file->isOpen()) {
        //     if (!file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        //         qWarning() << "Failed to open file";
        //         return 0;
        //     }
        // }
        qint64 bytesWritten = m_downloadFile->write(data, size);
        if (bytesWritten == -1) {
            qWarning() << "Failed to write to file";
            return 0;
        }
        return static_cast<size_t>(bytesWritten);
    });
    transfer->set(CURLOPT_URL, ftpUrl);
    transfer->set(CURLOPT_FTPPORT, QString::number(m_port));
    transfer->set(CURLOPT_USERPWD, QString("%1:%2").arg(m_username,m_password));
    transfer->set(CURLOPT_FOLLOWLOCATION, long(1)); // Follow redirects
    transfer->set(CURLOPT_FAILONERROR, long(1)); // Do not return CURL_OK in case valid server responses reporting errors.
    transfer->set(CURLOPT_WRITEDATA, m_downloadFile);
    log("downloadFile started.");
    transfer->perform();

    // ftpUrl.setUserName(m_username);
    // ftpUrl.setPassword(m_password);
    // QNetworkRequest request(ftpUrl);
    // QNetworkReply *reply = networkManager->get(request);
    // connect(reply, &QNetworkReply::finished, this, &FtpClient::onDownloadFinished);
    // connect(reply, &QNetworkReply::downloadProgress, this, &FtpClient::onDownloadProgress);
    // connect(reply, &QNetworkReply::readyRead, this, &FtpClient::onDownloadReadyRead);
    // connect(reply, &QNetworkReply::errorOccurred, this , &FtpClient::onDownloadError);
}

void FtpClient::uploadFile(const QString &localFile, const QString &remoteFile) {
    m_uploadFile = new QFile(localFile);
    if (!m_uploadFile->open(QIODevice::ReadOnly)) {
        delete m_uploadFile;
        return;
    }
    QString url = QString("ftp://%1/%2").arg(m_server, remoteFile);
    qDebug()  << "uploadFile: '" << localFile << "' TO " << url;
    QUrl ftpUrl(url);

    // Set a simple file reading function
    // transfer->setReadFunction(readUploadfile);
    transfer->setReadFunction([this](char *data, size_t size) -> size_t {
        qint64 bytesRead = m_uploadFile->read(data, size);
        if (bytesRead == -1) {
            qWarning() << "Failed to read file";
            return CURL_READFUNC_ABORT;
        }
        //Return the number of bytes actually read
        return static_cast<size_t>(bytesRead);
    });
    transfer->set(CURLOPT_URL, ftpUrl);
    transfer->set(CURLOPT_FTPPORT, QString::number(m_port));
    transfer->set(CURLOPT_USERPWD, QString("%1:%2").arg(m_username,m_password));
    transfer->set(CURLOPT_READDATA, m_uploadFile);
    log("uploadFile started.");
    transfer->perform();

    // ftpUrl.setUserName(m_username);
    // ftpUrl.setPassword(m_password);
    //TODO: remove exist remote file
    // QNetworkRequest request(ftpUrl);
    // QNetworkReply *reply = networkManager->put(request, file->readAll());
    // connect(reply, &QNetworkReply::finished, this, &FtpClient::onUploadFinished);
    // connect(reply, &QNetworkReply::uploadProgress, this, &FtpClient::onUploadProgress);
    // connect(reply, &QNetworkReply::errorOccurred, this, &FtpClient::onUploadError);
}

void FtpClient::setStop()
{
    emit stop();
}

void FtpClient::work()
{
    //the is qthread run
    while ((m_loops == -1 )||(m_runtimes <= m_loops)){
        qDebug() << "mode:" << m_ftpmode << " m_runtimes:" << QString::number(m_runtimes);
        if (m_ftpmode==FtpMode::Upload){
            uploadFile(m_localfile, m_remotefile);
        }else if (m_ftpmode==FtpMode::Download){
            downloadFile(m_remotefile, m_localfile);
        }else{
            qDebug() << "No ftp mode set(upload/download)!!";
        }
        m_runtimes++;
        if (m_stop){
            break;
        }
    }
    emit stop();
}

void FtpClient::onTransferProgress(qint64 downloadTotal, qint64 downloadNow, qint64 uploadTotal, qint64 uploadNow)
{
    Q_UNUSED(uploadTotal);
    Q_UNUSED(uploadNow);

    if (m_ftpmode==FtpClient::FtpMode::Download){
        if (downloadTotal > 0) {
            emit progress(m_id, downloadNow , downloadTotal);
            if (downloadNow > downloadTotal){
                downloadNow = downloadTotal;
            }
            // qint64 progress = (downloadNow * ui->progressBar->maximum())/downloadTotal;
            // ui->progressBar->setValue(static_cast<int>(progress));
        } else {
            // ui->progressBar->setValue(0);
        }
    }else{
        if (uploadTotal >0){
            emit progress(m_id, uploadNow, uploadTotal);
            if (uploadNow > uploadTotal){
                uploadNow = uploadTotal;
            }
        }
    }
}

// void FtpClient::onDownloadFinished() {
//     // Handle download completion
//     if (m_file) {
//         m_file->close();
//         delete m_file;
//         m_file = nullptr;
//     }
//     //TODO: loop Download
//     // emit downloadFinished();
//     QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
//     if (reply){
//         reply->deleteLater();
//     }
// }

// void FtpClient::onUploadFinished() {
//     // Handle upload completion
//     //TODO: loop Upload
//     // emit uploadFinished();
//     QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
//     if (reply){
//         // if (reply->error() == QNetworkReply::NoError) {
//         reply->deleteLater();
//     }
// }

// void FtpClient::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
// {
//     qDebug() << "onDownloadProgress:" << QString::number(bytesReceived) << " / " << QString::number(bytesTotal);
//     emit progress(bytesReceived, bytesTotal);
// }

// void FtpClient::onDownloadReadyRead()
// {
//     QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
//     if (reply) {
//         qDebug() << "onDownloadReadyRead: " << reply;
//         if (m_file) {
//             m_file->write(reply->readAll());
//         }else{
//             qDebug() << "m_file not exist";
//         }
//     }else{
//         qDebug() << "QNetworkReply not exist";
//     }
// }

// void FtpClient::onDownloadError(QNetworkReply::NetworkError code)
// {
//     qDebug() << "onDownloadError:" << code ;//<< "(" << code.errorString() << ")";
// }

// void FtpClient::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
// {
//     qDebug() << "onUploadProgress:" << QString::number(bytesSent) << " / " << QString::number(bytesTotal);
//     emit progress(bytesSent, bytesTotal);
// }

// void FtpClient::onUploadError(QNetworkReply::NetworkError code)
// {
//     qDebug() << "onUploadError:" << code ;//<< "(" << code.errorString() << ")";
// }

// size_t FtpClient::readUploadfile(char *buffer, size_t size, size_t nitems, void *stream)
// const size_t FtpClient::readUploadfile(char *buffer, size_t size)
// {
//     // QFile *file = static_cast<QFile *>(stream);
//     QFile *file = static_cast<QFile *>(m_uploadFile);
//     if (!file->isOpen()) {
//         if (!file->open(QIODevice::ReadOnly)) {
//             qWarning() << "Failed to open file";
//             return CURL_READFUNC_ABORT;
//         }
//     }

//     qint64 bytesRead = file->read(buffer, size);
//     if (bytesRead == -1) {
//         qWarning() << "Failed to read file";
//         return CURL_READFUNC_ABORT;
//     }
//     //Return the number of bytes actually read
//     return static_cast<size_t>(bytesRead);
// }

// size_t FtpClient::writeDownloadfile(char *ptr, size_t size, size_t nmemb, void *userdata) {
// const size_t FtpClient::writeDownloadfile(char *buffer, size_t size) {
//     // QFile *file = static_cast<QFile *>(userdata);
//     // QFile *file = static_cast<QFile *>(m_downloadFile);
//     // if (!file->isOpen()) {
//     //     if (!file->open(QIODevice::WriteOnly | QIODevice::Append)) {
//     //         qWarning() << "Failed to open file";
//     //         return 0;
//     //     }
//     // }

//     qint64 bytesWritten = m_downloadFile->write(buffer, size );
//     if (bytesWritten == -1) {
//         qWarning() << "Failed to write data to file";
//         return 0;
//     }

//     return static_cast<size_t>(bytesWritten);
// }

void FtpClient::log(QString text)
{
    qDebug() << text;
}
void FtpClient::onTransferDone()
{
    // CurlEasy *transfer = qconst<CurlEasy*>(sender());
    if (transfer->result() != CURLE_OK) {
        log(QString("Transfer failed with curl error '%1'")
                .arg(curl_easy_strerror(transfer->result())));
        m_downloadFile->remove();
        if (m_ftpmode == FtpMode::Download){
            emit downloadFinished(m_id);
        }else {
            emit uploadFinished(m_id);
        }
    } else {
        int tsize=0;
        if (m_ftpmode == FtpMode::Download){
            tsize = m_downloadFile->size();
        }else{
            tsize = m_uploadFile->size();
        }
        log(QString("Transfer complete. %1 bytes Transfered.").arg(tsize));
        // ui->progressBar->setValue(ui->progressBar->maximum());
    }
    if (m_ftpmode == FtpMode::Download){
        delete m_downloadFile;
        m_downloadFile = nullptr;
    }else{
        delete m_uploadFile;
        m_uploadFile = nullptr;
    }
}

void FtpClient::onTransferAborted()
{
    log(QString("Transfer aborted. "));
    emit errormsg(m_id, "Transfer aborted.");

    // log(QString("Transfer aborted. %1 bytes downloaded.")
    //         .arg(m_downloadFile->size()));

    // m_downloadFile->remove();
    // delete m_downloadFile;
    // m_downloadFile = nullptr;
}
