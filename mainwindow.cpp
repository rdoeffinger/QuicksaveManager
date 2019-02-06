#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    backupEnabled(false)
{
    ui->setupUi(this);
    on_timer();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList{tr("Screenshot"), tr("Filename")});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable()
{
    ui->tableWidget->clearContents();
    QDirIterator it(ui->dstText->text(), QDir::Dirs);
    int i = 0;
    while (it.hasNext())
    {
        QString name = it.next();
        if (!QFile(name + ".png").exists()) continue;

        ui->tableWidget->setRowCount(i + 1);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QPixmap(name + ".png"));
        ui->tableWidget->setItem(i, 0, item);
        item = new QTableWidgetItem(it.fileInfo().fileName());
        ui->tableWidget->setItem(i, 1, item);
        ++i;
    }
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void MainWindow::on_restorePushButton_clicked()
{
    bool oldEnabled = backupEnabled;
    backupEnabled = false;
    QMessageBox::critical(this, tr("Missing"), tr("Not yet implemented!"));
    fileList = findFiles();
    backupEnabled = oldEnabled;
    QTimer::singleShot(5000, Qt::VeryCoarseTimer, this, SLOT(on_timer()));
}

QMap<QString, FileEntry> MainWindow::findFiles()
{
    QDirIterator it(ui->srcText->text(), QStringList(ui->patternEdit->text()),
                    QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    QMap<QString, FileEntry> files;
    while (it.hasNext())
    {
        FileEntry f;
        f.fullname = it.next();
        QFileInfo info = it.fileInfo();
        f.name = info.fileName();
        f.modified = info.lastModified();
        f.size = info.size();
        files.insert(f.fullname, f);
    }
    return files;
}

static bool compareLists(const QMap<QString, FileEntry> &a, const QMap<QString, FileEntry> &b)
{
    if (a.size() != b.size()) return false;
    for (const FileEntry &e : a)
    {
        if (b.count(e.fullname) != 1) return false;
        const FileEntry &o = b[e.fullname];
        if (e.size != o.size) return false;
        if (e.modified != o.modified) return false;
    }
    return true;
}

void MainWindow::on_timer()
{
    if (!backupEnabled) return;
    QTimer::singleShot(5000, Qt::VeryCoarseTimer, this, SLOT(on_timer()));

    QMap<QString, FileEntry> newFileList = findFiles();
    if (compareLists(fileList, newFileList)) return;
    QString dst = "qsm-" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd-HH-mm-ss");
    if (!QDir(ui->dstText->text()).mkdir(dst)) return;
    dst = ui->dstText->text() + QDir::separator() + dst;

    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap capture = screen->grabWindow(0);
    capture.save(dst + ".png");

    dst += QDir::separator();
    QString src = ui->srcText->text() + QDir::separator();
    for (const FileEntry &e : newFileList)
    {
        QFile::copy(e.fullname, dst + e.name);
    }
    fileList = newFileList;
    updateTable();
}

void MainWindow::on_startStopPushButton_clicked()
{
    if (!backupEnabled)
    {
        QString dst(ui->dstText->text());
        if (dst.isEmpty() || !QDir(dst).exists())
        {
            QMessageBox::critical(this, tr("Bad destination"), tr("Specified backup destination directory does not exist!"));
            return;
        }
        QString src(ui->srcText->text());
        if (src.isEmpty() || !QDir(src).exists())
        {
            QMessageBox::critical(this, tr("Bad source"), tr("Specified source directory does not exist!"));
            return;
        }
        fileList = findFiles();
        if (fileList.size() == 0)
        {
            QMessageBox::critical(this, tr("Bad pattern"), tr("Specified source directory does not contain files matching the pattern!"));
            return;
        }
    }
    backupEnabled = !backupEnabled;
    ui->startStopPushButton->setText(backupEnabled ? tr("Stop") : tr("Start"));
    ui->srcPushButton->setEnabled(!backupEnabled);
    ui->dstPushButton->setEnabled(!backupEnabled);
    ui->patternEdit->setEnabled(!backupEnabled);
    QTimer::singleShot(5000, Qt::VeryCoarseTimer, this, SLOT(on_timer()));
}

void MainWindow::on_dstPushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select backup directory"));
    if (dir.isEmpty()) return;
    ui->dstText->setText(dir);
    updateTable();
}

void MainWindow::on_srcPushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select savegame location"));
    ui->srcText->setText(dir);
}