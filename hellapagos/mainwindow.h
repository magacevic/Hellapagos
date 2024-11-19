#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
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
    QLabel *errorLabel;
    void setFixedWindowSize();


private slots:
    void onJoinButtonClicked();  // Slot za join dugme
    void showErrorMessage();
    void onSettingsButtonClicked();  // Slot za settings dugme
    void onRulesButtonClicked();  // Slot za rules dugme
    void onStartGameButtonClicked();  // Slot za start game dugme
    void onMainMenuButtonClicked();
    void onNext_1ButtonClicked();
    void onNext_2ButtonClicked();
    void onNext_3ButtonClicked();
    void onNext_4ButtonClicked();
    void onNext_5ButtonClicked();
    void onNext_6ButtonClicked();
    void onNext_7ButtonClicked();
    void onPrevious_1ButtonClicked();
    void onPrevious_2ButtonClicked();
    void onPrevious_3ButtonClicked();
    void onPrevious_4ButtonClicked();
    void onPrevious_5ButtonClicked();
    void onPrevious_6ButtonClicked();
    void onPrevious_7ButtonClicked();
    void onPrevious_8ButtonClicked();
    void on_mainmenuButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);
    void on_MuteSoundButton_toggled(bool checked);
    void closeWindow();
};

#endif // MAINWINDOW_H
