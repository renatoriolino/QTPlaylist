#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QSettings>
#include <QDebug>

settingsWindow::settingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsWindow)
{
    ui->setupUi(this);

    ui->comboService->clear();
    ui->comboService->addItem("Spotify");   // hard coded for now

    LoadSettings();
}

settingsWindow::~settingsWindow()
{
    delete ui;
}

void settingsWindow::on_buttonBox_accepted()
{
    SaveSettings();
}

void settingsWindow::on_buttonBox_rejected()
{
}

void settingsWindow::LoadSettings()
{
    QSettings settings("QTPlaylist", "Renato Riolino");

    ui->txtId->setText("");
    ui->txtSecret->setText("");

    if (ui->comboService->currentText().length() == 0)
        return;

    ui->txtId->setText(settings.value(ui->comboService->currentText() + "/id", "").toString());
    ui->txtSecret->setText(settings.value(ui->comboService->currentText() + "/secret", "").toString());
}

void settingsWindow::SaveSettings()
{
    if (ui->comboService->currentText().length() == 0)
        return;

    QSettings settings("QTPlaylist", "Renato Riolino");

    settings.setValue(ui->comboService->currentText() + "/id", ui->txtId->text());
    settings.setValue(ui->comboService->currentText() + "/secret", ui->txtSecret->text());
}

void settingsWindow::on_comboService_currentTextChanged(const QString &arg1)
{
    LoadSettings();
}
