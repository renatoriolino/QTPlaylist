#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    actionServicesSettings = new QAction("&Settings", this);
    connect(actionServicesSettings, &QAction::triggered, this, &MainWindow::on_menuServicesSettings);
    menuServices = menuBar()->addMenu("&Services");
    menuServices->addAction(actionServicesSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_txtSearch_returnPressed()
{
    ui->listResult->clear();
    ui->listResult->addItem(ui->txtSearch->text());
}

void MainWindow::on_listResult_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listPlaylist->addItem(item->text());
}

void MainWindow::on_listPlaylist_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "Playing " << item->text() << "...";
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

void MainWindow::on_menuServicesSettings()
{
    qDebug() << "settings";
    settingsWindow settingsWin;
    settingsWin.setModal(true);
    settingsWin.exec();

}
