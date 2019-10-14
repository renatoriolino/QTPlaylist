#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "imusicservice.h"

#include <QMediaPlayer>

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

    void on_menuServicesConnect();

    void on_menuServicesSettings();

    void connected();

    void on_searchResult(const QList<iMusicService::S_TRACK_DATA> &res);

    void on_btPlay_clicked();

    void on_btPrev_clicked();

    void on_btNext_clicked();

    void on_btClear_clicked();

private:
    Ui::MainWindow *ui;

    QMenu *menuServices;
    QAction *actionServicesConnect;
    QAction *actionServicesSettings;

    iMusicService *musicService;

    QMediaPlayer *player;
    bool m_Playing = false;
    QIcon m_PlayIcon;

    int getCurrentTrack(void);
    bool setCurrentTrack(int _row);
    void playCurrentTrack(void);
    void stop(void);

    void setPlayIcon(int _row);

    void loadPlaylist(void);
    void savePlaylist(void);
};
#endif // MAINWINDOW_H
