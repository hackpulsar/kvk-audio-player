#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MusicBuffer.h"
#include <QMainWindow>
#include <thread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_playButton_clicked();
 
    void on_progressSlider_sliderMoved(int position);
    void on_progressSlider_sliderReleased();

    void on_actionAbout_triggered();

private:
    void PlayingLoop();

private:
    Ui::MainWindow *ui;

    MusicBuffer* m_Music = nullptr;
    std::thread* m_MusicThread = nullptr;

    int m_nCurrentPlayPosition = 0;

};

#endif // MAINWINDOW_H
