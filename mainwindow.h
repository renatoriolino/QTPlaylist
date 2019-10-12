#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_txtSearch_returnPressed();

    void on_listResult_itemDoubleClicked(QListWidgetItem *item);

    void on_listPlaylist_itemDoubleClicked(QListWidgetItem *item);

    void on_btDelete_clicked();

    void on_btMoveUp_clicked();

    void on_btMoveDown_clicked();

    void on_menuServicesSettings();
private:
    Ui::MainWindow *ui;

    QMenu *menuServices;
    QAction *actionServicesSettings;
};
#endif // MAINWINDOW_H
