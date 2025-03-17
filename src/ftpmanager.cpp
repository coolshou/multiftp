#include "ftpmanager.h"

FtpManager::FtpManager(QObject *parent)
    : QObject{parent}
{}

FtpManager::~FtpManager() {
    for (QThread *thread : threads) {
        thread->quit();
        thread->wait();
        delete thread;
    }
    for (FtpClient *client : clients) {
        delete client;
    }
}

void FtpManager::addFtpClient(int id, const QString &server, int port,
                              const QString &username, const QString &password,
                              QString localfile, QString remotefile, FtpClient::FtpMode mode) {
    QThread *thread = new QThread;
    FtpClient *client = new FtpClient(id, server, username, password, port);
    client->setFtpMode(mode, localfile, remotefile);
    client->moveToThread(thread);

    connect(thread, &QThread::started, client, &FtpClient::work);

    connect(client, &FtpClient::downloadFinished, thread, &QThread::quit);
    connect(client, &FtpClient::uploadFinished, thread, &QThread::quit);
    connect(client, &FtpClient::stop, thread, &QThread::quit);

    threads.append(thread);
    clients.append(client);

    // thread->start();
}

void FtpManager::start()
{
    foreach(auto *th, threads){
        th->start();
    }
}

void FtpManager::stop()
{
    foreach(auto *cl, clients){
        cl->setStop();
    }
}

int FtpManager::count()
{
    return clients.size();
}


