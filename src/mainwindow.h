#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ftpmanager.h"
#include "ftpmodel.h"
#include "dirdelegate.h"

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

private slots:
    void onAdd(bool checked);
    void onClear(bool checked);
    void onStart(bool checked);
    void onStop(bool checked);
    void setRunStatus(bool start);

private:
    Ui::MainWindow *ui;
    FtpManager *m_ftpmanager;
    FtpModel *m_ftpmodel;
    DirDelegate *m_dirdelegate;
};
#endif // MAINWINDOW_H
