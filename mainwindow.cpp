#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"
#include "imusicservice.h"
#include "cmusicservicespotify.h"

#include <QMessageBox>
#include <QDebug>

#define QTPLAYLISTDATA_ID   1000
#define QTPLAYLISTDATA_URL  1001
#define QTPLAYLISTDATA_STAT 1002

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("QTPlaylist");
    QCoreApplication::setOrganizationDomain("github.com/renatoriolino");
    QCoreApplication::setApplicationName("Renato Riolino");

    actionServicesConnect = new QAction("&Connect", this);
    connect(actionServicesConnect, &QAction::triggered, this, &MainWindow::on_menuServicesConnect);
    actionServicesSettings = new QAction("&Settings", this);
    connect(actionServicesSettings, &QAction::triggered, this, &MainWindow::on_menuServicesSettings);
    menuServices = menuBar()->addMenu("&Services");
    menuServices->addAction(actionServicesConnect);
    menuServices->addSeparator();
    menuServices->addAction(actionServicesSettings);

    musicService = new cMusicServiceSpotify();

    connect(musicService, SIGNAL(searchResult(const QList<iMusicService::S_TRACK_DATA>&)), this, SLOT(on_searchResult(const QList<iMusicService::S_TRACK_DATA>&)));

    player = new QMediaPlayer;

    m_PlayIcon.addFile(":/images/play1.png", QSize(16, 16));
}

MainWindow::~MainWindow()
{
    delete player;
    delete musicService;
    delete menuServices;
    delete actionServicesSettings;
    delete ui;
}


void MainWindow::on_txtSearch_returnPressed()
{
    ui->listResult->clear();
    //ui->listResult->addItem(ui->txtSearch->text());
    if (!musicService->Search(ui->txtSearch->text()))
        qDebug() << "Search failed";
}

void MainWindow::on_listResult_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listPlaylist->addItem(item->clone());
}

void MainWindow::on_listPlaylist_itemDoubleClicked(QListWidgetItem *item)
{
    int row=0;
    for (; row<ui->listPlaylist->count(); row++)
        if (ui->listPlaylist->item(row) == item)
            break;

    qDebug() << "Playing " << item->text() << " at " << item->data(QTPLAYLISTDATA_URL).toString() << "...";
    setCurrentTrack(row);
    playCurrentTrack();
}

void MainWindow::on_btDelete_clicked()
{
    QListWidgetItem *item = ui->listPlaylist->currentItem();
    if (item == nullptr)
        return;

    qDebug() << "Removing " << item->text() << "from playlist...";
    delete item;
}

void MainWindow::on_btMoveUp_clicked()
{
    int row = ui->listPlaylist->currentRow();
    if (row == 0)
        return;

    QListWidgetItem *item = ui->listPlaylist->takeItem(row);
    ui->listPlaylist->insertItem(row-1, item);
    ui->listPlaylist->setCurrentRow(row-1);
}

void MainWindow::on_btMoveDown_clicked()
{
    int row = ui->listPlaylist->currentRow();
    if (row == ui->listPlaylist->count()-1)
        return;

    QListWidgetItem *item = ui->listPlaylist->takeItem(row);
    ui->listPlaylist->insertItem(row+1, item);
    ui->listPlaylist->setCurrentRow(row+1);
}

void MainWindow::on_menuServicesConnect()
{
    musicService->Connect();
    connect(musicService, SIGNAL(connected()), this, SLOT(connected()));
}

void MainWindow::on_menuServicesSettings()
{
    qDebug() << "settings";
    settingsWindow settingsWin;
    settingsWin.setModal(true);
    settingsWin.exec();

}

void MainWindow::connected()
{
    qDebug() << "Connected!!!";
}

void MainWindow::on_searchResult(const QList<iMusicService::S_TRACK_DATA> &res)
{
    for (auto r : res)
    {
        auto *newItem = new QListWidgetItem(r.name, nullptr);
        newItem->setData(QTPLAYLISTDATA_ID, r.id);
        newItem->setData(QTPLAYLISTDATA_URL, r.url);
        ui->listResult->addItem(newItem);
    }
}

void MainWindow::on_btPlay_clicked()
{
    if (m_Playing)
    {
        stop();
        return;
    }

    if (ui->listPlaylist->count() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Playlist is empty!");
        msgBox.exec();
        return;
    }

    int row = -1;
    for (int i=0; i<ui->listPlaylist->count() && row < 0; i++)
        if (ui->listPlaylist->item(i)->data(QTPLAYLISTDATA_STAT).toInt() == 1)
            row = i;
    if (row < 0)
        row = 0;

    setCurrentTrack(row);
    playCurrentTrack();
}

bool MainWindow::setCurrentTrack(int _row)
{
    // Clear play status from all items and set for the item on the selected row
    for (int i=0; i<ui->listPlaylist->count(); i++)
        ui->listPlaylist->item(i)->setData(QTPLAYLISTDATA_STAT, 0);
    ui->listPlaylist->item(_row)->setData(QTPLAYLISTDATA_STAT, 1);

    if (ui->listPlaylist->item(_row)->data(QTPLAYLISTDATA_URL).toString().length() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("No preview available for " + ui->listPlaylist->item(_row)->text());
        msgBox.exec();
        return false;
    }

    player->setMedia(QUrl(ui->listPlaylist->item(_row)->data(QTPLAYLISTDATA_URL).toString()));
    if (m_Playing)
        playCurrentTrack();
    return true;
}

void MainWindow::playCurrentTrack()
{
    for (int i=0; i<ui->listPlaylist->count(); i++)
        if (ui->listPlaylist->item(i)->data(QTPLAYLISTDATA_STAT).toInt() == 1)
        {
            setPlayIcon(i);
            break;
        }

    player->play();
    m_Playing = true;
}

void MainWindow::stop()
{
    setPlayIcon(-1);
    player->stop();
    m_Playing = false;
}

void MainWindow::setPlayIcon(int _row)
{
    for (int i=0; i<ui->listPlaylist->count(); i++)
        ui->listPlaylist->item(i)->setIcon(QIcon());

    if (_row < 0 || _row >= ui->listPlaylist->count())
        return;

    ui->listPlaylist->item(_row)->setIcon(m_PlayIcon);
}

void MainWindow::on_btPrev_clicked()
{
    int t = getCurrentTrack();
    if (t < 0 || !m_Playing)
        return;
    if (t == 0)
        t = ui->listPlaylist->count();

    setCurrentTrack(t-1);
}

int MainWindow::getCurrentTrack()
{
    for (int i=0; i<ui->listPlaylist->count(); i++)
        if (ui->listPlaylist->item(i)->data(QTPLAYLISTDATA_STAT).toInt() == 1)
            return i;
    return -1;

}

void MainWindow::on_btNext_clicked()
{
    int t = getCurrentTrack();
    if (t < 0 || !m_Playing)
        return;
    if (t >= ui->listPlaylist->count()-1)
        t = -1;

    setCurrentTrack(t+1);

}
