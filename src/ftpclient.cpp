#include "ftpclient.h"
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QEventLoop>
#include <QUrl>
#include <QIODevice>
#include <QThread>
#include <QFileInfo>
#include <QDir>

#include "comm.h"

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
    m_downloadsize = 0;
    // networkManager = new QNetworkAccessManager(this);
    transfer = new CurlEasy(this);
    connect(transfer, &CurlEasy::done, this, &FtpClient::onTransferDone);
    connect(transfer, &CurlEasy::aborted, this, &FtpClient::onTransferAborted);
    // connect(transfer, &CurlEasy::progress, this, &FtpClient::onTransferProgress);
    // m_timer = QTimer(this);
    m_timer.setInterval(3000);//3sec
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
    QFileInfo f(localFile);
    QDir d(f.absolutePath());
    if (!d.exists(f.absolutePath())){
        d.mkdir(f.absolutePath());
    }
    m_downloadFile = new QFile(localFile);
    if (!m_downloadFile->open(QIODevice::WriteOnly)) {
        QString msg = QString("Open file %1 to Write fail").arg(localFile);
        qDebug() << msg;
        emit errormsg(m_id, msg);
        delete m_downloadFile;
        m_downloadFile = nullptr;
        return;
    }
    QString url = QString("%1").arg(remoteFile);
    qDebug()  << "downloadFile: '" << url<< "' TO " << localFile ;

    QUrl ftpUrl(url);
    // Set a simple file writing function
    m_current_downloadsize = 0;
    transfer->setWriteFunction([this](char *data, size_t size)->size_t {
        qint64 bytesWritten = m_downloadFile->write(data, static_cast<qint64>(size));
        if (bytesWritten ==-1){
            qDebug() << "write to m_downloadFile Fail";
        }
        m_current_downloadsize = m_downloadFile->size();
        if ((m_downloadsize>0)&&(m_current_downloadsize>0)){
            int p = static_cast<int>((static_cast<double>(m_current_downloadsize) / m_downloadsize) * 100);
            // qDebug() << "write " << m_current_downloadsize << " / " << m_downloadsize << " - " << p << " %";
            emit progress(m_id, m_current_downloadsize , m_downloadsize, p);
        }

        return static_cast<size_t>(bytesWritten);
    });

    // Print headers to the transfer log box
    transfer->setHeaderFunction([this](char *data, size_t size)->size_t {
        QString msg = QString::fromUtf8(data, static_cast<int>(size));
        // log(msg);
        if (msg.contains("213")){
            QStringList ls = msg.split(" ");
            if (ls.size()==2){
                qDebug() << "File size:" << ls[1];
                bool ok;
                m_downloadsize = ls[1].toLongLong(&ok);
                if (!ok){
                    qDebug() << "convert "<< ls[1] <<" to int64 Fail";
                }
                emit progress(m_id, 0 , m_downloadsize, 0);
            }
        }
        return size;
    });

    transfer->set(CURLOPT_URL, ftpUrl);
    transfer->set(CURLOPT_PORT, m_port);
    transfer->set(CURLOPT_USERNAME, m_username);
    transfer->set(CURLOPT_PASSWORD, m_password);
    transfer->set(CURLOPT_FOLLOWLOCATION, long(1)); // Follow redirects
    transfer->set(CURLOPT_FAILONERROR, long(1)); // Do not return CURL_OK in case valid server responses reporting errors.
    log("downloadFile started.");
    transfer->perform();
}

void FtpClient::uploadFile(const QString &localFile, const QString &remoteFile) {
    m_uploadFile = new QFile(localFile);
    if (!m_uploadFile->open(QIODevice::ReadOnly)) {
        delete m_uploadFile;
        return;
    }
    QString url = QString("%1").arg(remoteFile);
    qDebug()  << "uploadFile: '" << localFile << "' TO " << url;
    QUrl ftpUrl(url);

    // Set a simple file reading function
    m_uploadsize = m_uploadFile->size();
    m_current_uploadsize=0;
    emit progress(m_id, m_current_uploadsize , m_uploadsize, 0);
    transfer->setReadFunction([this](char *data, size_t size) -> size_t {
        qint64 bytesRead = m_uploadFile->read(data, size);
        if (bytesRead == -1) {
            qWarning() << "Failed to read file";
            return CURL_READFUNC_ABORT;
        }
        //Return the number of bytes actually read
        m_current_uploadsize = m_current_uploadsize + bytesRead;
        if ((m_current_uploadsize>0)&&(m_uploadsize>0)){
            int p = static_cast<int>((static_cast<double>(m_current_uploadsize) / m_uploadsize) * 100);
            // qDebug() << "read " << m_current_uploadsize << " / " << m_uploadsize << " - " << p << " %";
            emit progress(m_id, m_current_uploadsize , m_uploadsize, p);
        }
        return static_cast<size_t>(bytesRead);
    });

    transfer->set(CURLOPT_URL, ftpUrl);
    transfer->set(CURLOPT_PORT, m_port);
    transfer->set(CURLOPT_USERNAME, m_username);
    transfer->set(CURLOPT_PASSWORD, m_password);
    transfer->set(CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR); // create dir if remote does not have it
    // Enable uploading
    transfer->set(CURLOPT_UPLOAD, 1L);
    transfer->perform();
}

void FtpClient::setStop()
{
    m_stop = true;
    emit stop(m_id);
}

void FtpClient::work()
{
    //the is qthread run
    m_runtimes++;
    qDebug() << "ID:" << m_id << " mode:" << m_ftpmode << " m_runtimes:" << QString::number(m_runtimes);
    if (m_ftpmode==FtpMode::Upload){
        uploadFile(m_localfile, m_remotefile);
    }else if (m_ftpmode==FtpMode::Download){
        downloadFile(m_remotefile, m_localfile);
    }else{
        emit errormsg(m_id , QString::number(m_id) + " No ftp mode set(upload/download)!!");
    }
}

void FtpClient::onTransferProgress(qint64 downloadTotal, qint64 downloadNow, qint64 uploadTotal, qint64 uploadNow)
{
    Q_UNUSED(uploadTotal);
    Q_UNUSED(uploadNow);
    int p =0;
    if (m_ftpmode==FtpClient::FtpMode::Download){
        if (downloadTotal > 0) {
            //calc progress
            p = static_cast<int>((static_cast<double>(downloadNow) / downloadTotal)*100);
            // qDebug() << m_id << " Download p: " << QString::number(p);
            emit progress(m_id, downloadNow , downloadTotal, p);
        } else {
            // ui->progressBar->setValue(0);
        }
    }else{
        if (uploadTotal >0){
            p = static_cast<int>((static_cast<double>(uploadNow) / uploadTotal)*100);
            // qDebug() << m_id << " Upload p: " << p;
            emit progress(m_id, uploadNow, uploadTotal, p);
        }
    }
}

void FtpClient::log(QString text)
{
    qDebug() << text;
}

void FtpClient::onTransferDone()
{
    // CurlEasy *transfer = qconst<CurlEasy*>(sender());
    if (transfer->result() != CURLE_OK) {
        QString msg = QString(" Transfer failed with curl error '%1'")
                          .arg(curl_easy_strerror(transfer->result()));
        log(msg);
        emit errormsg(m_id, getDateTimeNow() + msg);
        // m_downloadFile->remove();
        if (m_ftpmode == FtpMode::Download){
            emit downloadFinished(m_id);
        }else {
            emit uploadFinished(m_id);
        }
    } else {
        int tsize=0;
        if (m_ftpmode == FtpMode::Download){
            // qDebug() << "m_downloadsize: " << QString::number(m_downloadsize);
            tsize = m_downloadFile->size();
        }else{
            tsize = m_uploadFile->size();
        }
        QString msg = QString(" Transfer complete. %1 bytes Transfered.").arg(tsize);
        log(msg);
        emit errormsg(m_id, getDateTimeNow()+msg);

        if (m_ftpmode == FtpMode::Download){
            delete m_downloadFile;
            m_downloadFile = nullptr;
        }else{
            delete m_uploadFile;
            m_uploadFile = nullptr;
        }
        if ((m_runtimes < m_loops)||(m_loops==-1)){
            m_stop = false;
            if (m_loops){
                qDebug() << "m_loops: " << m_runtimes << " / " << m_loops;
            }
            emit errormsg(m_id, "Next Test in 3 sec:"+ getDateTimeNow(3));
            m_timer.singleShot(3000, this, &FtpClient::work);
        }else{
            emit stop(m_id);
        }
    }

}

void FtpClient::onTransferAborted()
{
    // log(QString("Transfer aborted. "));
    QString msg = getDateTimeNow() + " Transfer aborted.";
    if (transfer->result() != CURLE_OK) {
        msg = msg +"("+ curl_easy_strerror(transfer->result())+")";
    }
    log(msg);
    emit errormsg(m_id, msg);
}
