#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SoundDevice.h"
#include "libtinyfiledialogs/tinyfiledialogs.h"

#include <chrono>
#include <string>

#include <fmt/format.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("KVK Audio Player");

    SoundDevice::init();

    ui->playButton->setFixedSize(100, 100);
    ui->playButton->setIcon(QIcon("../res/play_icon.png"));
    ui->playButton->setIconSize(QSize(100, 100));
    ui->playButton->setStyleSheet("border: none;");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_Music;
    delete m_MusicThread;
}

void MainWindow::on_actionOpen_triggered()
{
    const char* sFilterPatterns[2] = { "*.mp3", "*.wav" };
    const char* sPath = tinyfd_openFileDialog("Open File", "", 2, sFilterPatterns, "Music Files", 0);

    qDebug() << "File selected: " << sPath;

    m_Music = new MusicBuffer(sPath);
    ui->playButton->setEnabled(true);
}

void MainWindow::on_actionAbout_triggered()
{
    tinyfd_messageBox(
        "About", "KVK Audio player by @hackpulsar",
        "ok", "info", 0
    );
}

void MainWindow::PlayingLoop()
{
    while(m_Music->IsPlaying())
    {
        m_Music->UpdateBufferStream();

        auto elapsed = std::chrono::duration<double>(m_Music->GetCurrentElapsedTime());
        std::string sElapsed = fmt::format("{:02.0f}:{:02.0f}", std::floor(elapsed.count() / 60), std::fmod(elapsed.count(), 60));
        ui->elapsedLabel->setText(sElapsed.c_str());

        ui->progressSlider->setValue(m_Music->GetElapsedPercent());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MainWindow::on_playButton_clicked()
{
    if (m_Music != nullptr)
    {
        if (m_Music->IsPlaying())
        {
            m_Music->Pause();
            m_MusicThread->join();
            this->ui->playButton->setIcon(QIcon("../res/play_icon.png"));
        }
        else
        {
            m_Music->Play();

            m_MusicThread = new std::thread(&MainWindow::PlayingLoop, this);
            this->ui->playButton->setIcon(QIcon("../res/pause_icon.png"));
        }
    }
    else qDebug() << "No music loaded";
}

void MainWindow::on_progressSlider_sliderMoved(int position)
{
    qDebug() << "Slider moved to " << position;
    if (m_Music != nullptr)
        m_Music->SetCurrentPlayPosition(position);
    else qDebug() << "No music loaded";
}


void MainWindow::on_progressSlider_sliderReleased()
{
    qDebug() << "Slider released";
    if (m_Music != nullptr)
    {
        m_Music->Play();
        m_MusicThread = new std::thread(&MainWindow::PlayingLoop, this);
    }
    else qDebug() << "No music loaded";
}


void MainWindow::on_progressSlider_sliderPressed()
{
    qDebug() << "Slider pressed";
    if (m_Music != nullptr)
    {
        m_Music->Pause();
        m_MusicThread->join();
    }
    else qDebug() << "No music loaded";
}

