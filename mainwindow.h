#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

struct FileEntry
{
    QString fullname;
    QString name;
    QDateTime modified;
    qint64 size;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_restorePushButton_clicked();
    void on_timer();

    void on_startStopPushButton_clicked();

    void on_dstPushButton_clicked();

    void on_srcPushButton_clicked();

private:
    void updateTable();
    QMap<QString, FileEntry> findFiles();

    Ui::MainWindow *ui;
    QMap<QString, FileEntry> fileList;
    volatile bool backupEnabled;
};

#endif // MAINWINDOW_H
