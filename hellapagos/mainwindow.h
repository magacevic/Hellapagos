#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include <QMainWindow>
#include "soundmanager.h"

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

private:
    Ui::MainWindow *ui;
    Client *client;
    SoundManager *soundManager;


private slots:
    void onJoinButtonClicked();  // Slot za join dugme
    void onSettingsButtonClicked();  // Slot za settings dugme
    void onRulesButtonClicked();  // Slot za rules dugme
    void onStartGameButtonClicked();  // Slot za start game dugme
};

#endif // MAINWINDOW_H
