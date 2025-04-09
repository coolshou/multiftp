#include "remotelist.h"
#include "ui_remotelist.h"

#include "comm.h"
// Define the static member variable
QList<QList<QString>> RemoteList::m_datas;
QList<QList<QString>> RemoteList::m_dirdatas;

RemoteList::RemoteList(QUrl url, QString username, QString passwd, int port,
                       QWidget *parent)
    : QDialog(parent), ui(new Ui::RemoteList),
    m_url(url), m_username(username), m_password(passwd), m_port(port)
{
    ui->setupUi(this);
    // connect(this, &RemoteList::datalist, this, &RemoteList::onDatalist);
    transfer = new CurlEasy(this);
    connect(transfer, &CurlEasy::done, this, &RemoteList::onTransferDone);
    transfer->setWriteFunction(this->writecallback);
    list();
}

RemoteList::~RemoteList()
{
    delete ui;
}

void RemoteList::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RemoteList::list(QUrl url, QString username, QString passwd, int port)
{
    clear();
    if (!url.isEmpty()){
        m_url = url;
    }
    setWindowTitle(m_url.toString());
    if (!username.isEmpty()){
        m_username = username;
    }
    if (!passwd.isEmpty()){
        m_password = passwd;
    }
    if (port){
        m_port = port;
    }
    transfer->set(CURLOPT_URL, m_url);
    transfer->set(CURLOPT_PORT, m_port);
    transfer->set(CURLOPT_USERNAME, m_username);
    transfer->set(CURLOPT_PASSWORD, m_password);
    // transfer->set(CURLOPT_DIRLISTONLY, 1L);
    transfer->perform();
}

size_t RemoteList::writecallback(char *data, size_t size)
{
    // qDebug() << "writecallback:" << data << " size:" << size;
    QString tmp(data);
    QStringList  ds = tmp.split("\n");
    foreach(QString d, ds){
        QStringList ls = d.split(" ");
        if (ls.size()>1){
            qDebug() << " data: " << ls.size() << "  => " << ls;
            QList<QString> l;
            l.append(ls.first());
            l.append(ls.last());
            if (ls.first().startsWith("-")){
                m_datas.append(l);
            }else{
                m_dirdatas.append(l);
            }
        }
    }
    return size;
}

void RemoteList::clear()
{
    m_dirdatas.clear();
    m_datas.clear();
    ui->tableWidget->clearContents();
}

QString RemoteList::getRemoteFileName()
{
    QList<QTableWidgetItem*> ds = ui->tableWidget->selectedItems();
    if (ds.count()>1){
        if (ds.first()->text().startsWith("-")){
            //TODO: also get the url path
            return ds.last()->text();
        }
    }
    // foreach(auto d, ds){
    //     qDebug() << "getRemoteFileName:" << d->text();
    // }
    return "";
}

void RemoteList::onTransferDone()
{
    if (transfer->result() != CURLE_OK) {
        QString msg = QString(" Transfer failed with curl error '%1'")
                          .arg(curl_easy_strerror(transfer->result()));
        emit errormsg( getDateTimeNow() + msg);
    }else{
        qDebug() << "perform list down";
        // ui->textEdit->append(m_datas.join(""));
        ui->tableWidget->setRowCount(m_dirdatas.count()+m_datas.count());
        int row=0;
        foreach (const QList<QString> &map, m_dirdatas){

            QTableWidgetItem *newItem = new QTableWidgetItem(map.first());
            ui->tableWidget->setItem(row, 0, newItem);
            QTableWidgetItem *newItem1 = new QTableWidgetItem(map.last());
            ui->tableWidget->setItem(row, 1, newItem1);
            row++;
        }
        foreach (const QList<QString> &map, m_datas){

            QTableWidgetItem *newItem = new QTableWidgetItem(map.first());
            ui->tableWidget->setItem(row, 0, newItem);
            QTableWidgetItem *newItem1 = new QTableWidgetItem(map.last());
            ui->tableWidget->setItem(row, 1, newItem1);
            row++;
        }
    }
}

