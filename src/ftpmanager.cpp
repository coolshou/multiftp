#include "ftpmanager.h"

FtpManager::FtpManager(QObject *parent)
    : QObject{parent}
{}

FtpManager::~FtpManager()
{
    clear();
}

void FtpManager::onAddFtpClient(int id, const QString &server, int port,
                              const QString &username, const QString &password,
                              QString localfile, QString remotefile, FtpClient::FtpMode mode,
                              int loop) {
    QThread *thread = new QThread;
    FtpClient *client = new FtpClient(id, server, username, password, port, loop);
    client->setFtpMode(mode, localfile, remotefile);
    client->moveToThread(thread);

    connect(thread, &QThread::started, client, &FtpClient::work);

    connect(client, &FtpClient::downloadFinished, this, &FtpManager::onDownloadFinished);
    connect(client, &FtpClient::uploadFinished, this, &FtpManager::onUploadFinished);
    connect(client, &FtpClient::progress, this, &FtpManager::onProgress);
    connect(client, &FtpClient::throughput, this, &FtpManager::onThroughput);
    // connect(client, &FtpClient::stop, thread, &QThread::quit);
    connect(client, &FtpClient::stop, this, &FtpManager::onStop);
    connect(client, &FtpClient::errormsg, this, &FtpManager::onErrorMsg);

    threads.append(thread);
    clients.append(client);

    // thread->start();
}

void FtpManager::start()
{
    foreach(auto *th, threads){
        qDebug() << "start thread:" << th;
        th->start();
        int idx = threads.indexOf(th);
        runnings.append(idx);
    }
    emit started();
}

void FtpManager::stop()
{
    foreach(auto *cl, clients){
        cl->setStop();
    }
    emit stoped();
}

int FtpManager::count()
{
    return clients.size();
}

void FtpManager::clear()
{
    for (FtpClient *client : clients) {
        client->setStop();
        delete client;
    }
    clients.clear();
    for (QThread *thread : threads) {
        thread->quit();
        thread->wait();
        delete thread;
    }
    threads.clear();
}

void FtpManager::onDownloadFinished(int id)
{
    qDebug() << id << " Download Finished";
}

void FtpManager::onUploadFinished(int id)
{
    qDebug() << id << " Upload Finished";
}

void FtpManager::onProgress(int id, qint64 bytesCurrent, qint64 bytesTotal, int percentage)
{
    emit progress(id, bytesCurrent, bytesTotal, percentage);
}

void FtpManager::onThroughput(int id, double value)
{
    emit throughput(id, value);
}

void FtpManager::onErrorMsg(int id, QString msg)
{
    emit errormsg(id, msg);
}

void FtpManager::onStop(int id)
{
    QThread *t = threads.at(id);
    t->quit();
    t->wait();
    if (runnings.contains(id)){
        int idx = runnings.indexOf(id);
        runnings.remove(idx);
    }
    if (runnings.size()<=0){
        emit stoped();
    }
}


