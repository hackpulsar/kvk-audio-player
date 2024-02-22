#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SoundDevice.h"
#include "libtinyfiledialogs/tinyfiledialogs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SoundDevice::init();
    ui->statusLabel->setText("Not loaded");
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
    ui->statusLabel->setText("Loaded");
    ui->playButton->setEnabled(true);
}

void MainWindow::PlayingLoop()
{
    while(m_Music->IsPlaying())
    {
        m_Music->UpdateBufferStream();
        ui->elapsedLabel->setText(QString::number(m_Music->GetElapsedPercent()) + "%");
        ui->progressSlider->setValue(m_Music->GetElapsedPercent());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    if (ui->progressSlider->isSliderDown() == false)
    {
        qDebug() << "Audio file ended";
        this->ui->playButton->setEnabled(true);
        this->ui->elapsedLabel->setText("100%");
        this->ui->progressSlider->setValue(100);
    }
}

void MainWindow::on_playButton_clicked()
{
    if (m_Music != nullptr)
    {
        m_Music->Play();

        m_MusicThread = new std::thread(&MainWindow::PlayingLoop, this);
        this->ui->playButton->setEnabled(false);
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

