#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class settingsWindow;
}

class settingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit settingsWindow(QWidget *parent = nullptr);
    ~settingsWindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_comboService_currentTextChanged(const QString &arg1);

private:
    Ui::settingsWindow *ui;

    void LoadSettings();
    void SaveSettings();
};

#endif // SETTINGSWINDOW_H
