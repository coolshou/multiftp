#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "ftpmanager.h"
#include "ftpmodel.h"
#include "ftpoption.h"
#include "dirdelegate.h"
#include "remotelist.h"
#include "comm.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void decompressFile(const QString &sourcePath, const QString &destinationPath);

private slots:
    void loadcfg();
    void savecfg();
    void onShowAbout(bool checked);
    void onQuit(bool checked);
    void onAdd(bool checked);
    void onClear(bool checked);
    void onStart(bool checked);
    void onStop(bool checked);
    void setRunStatus(bool start);
    void onStarted();
    void onStoped();

private:
    Ui::MainWindow *ui;
    FtpManager *m_ftpmanager;
    FtpModel *m_ftpmodel;
    FtpOption *m_ftpoption;
    DirDelegate *m_dirdelegate;
    QString apppath;
    QSettings *cfg;
    RemoteList *m_remotelist;
};
#endif // MAINWINDOW_H
