#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>

QPixmap ResizeImgToFit(const QPixmap &image, int window_width, int window_height) {
    double width_ratio = double(image.width()) / window_width;
    double height_ratio = double(image.height()) / window_height;

    if ( width_ratio > height_ratio ) {
        return image.scaledToWidth(window_width);
    } else {
        return image.scaledToHeight(window_height);
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    showMaximized();

    player_.setAudioOutput(&audio_output_);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::OnTimer);
    connect(&player_, &QMediaPlayer::mediaStatusChanged,
            this, &MainWindow::OnPlayerFinish);

    yoga_sequence_ = YogaSequence([this](YogaPose pose){this->RunPose(pose);});

    yoga_sequence_.StartMorningSalutation();
}

MainWindow::~MainWindow() {
    delete ui_;
}

QPixmap MainWindow::GetCurrentFile() {
    if (!yoga_sequence_.IsInProgress()) return QPixmap{};
    YogaPose pose = yoga_sequence_.GetCurrent();

    auto path = QString{":/images/images/%1.jpeg"}.arg(pose.first.c_str());
    return QPixmap{path};
}

void MainWindow::UpdateImage() {
    auto pixmap = ResizeImgToFit(GetCurrentFile(), width(), height());
    picture_label_.setPixmap(pixmap);
    int label_x = (width() - pixmap.width()) / 2;
    int label_y = (height() - pixmap.height()) / 2;

    picture_label_.setGeometry(label_x, label_y, pixmap.width(), pixmap.height());
}

void MainWindow::resizeEvent(QResizeEvent*) {
    UpdateImage();
}

void MainWindow::OnTimer() {
    yoga_sequence_.OnTimer();
}

void MainWindow::OnPlayerFinish(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        yoga_sequence_.OnAudioFinished();
    }
}

void MainWindow::NextPose() {
    yoga_sequence_.Next();
}

void MainWindow::RunPose(const YogaPose& pose) {
    auto path = QString{"qrc:/sounds/sounds/%1.mp3"}.arg(pose.first.c_str());
    player_.setSource(QUrl{path});
    UpdateImage();
    UpdateEnabled();

    timer_.setInterval(pose.second);
    timer_.start();

    player_.play();
}

void MainWindow::UpdateEnabled() {
    ui_->btn_left->setEnabled(yoga_sequence_.HasPrevious());
    ui_->btn_right->setEnabled(yoga_sequence_.HasNext());
}

void MainWindow::on_btn_right_clicked() {
    yoga_sequence_.Next();
}

void MainWindow::on_btn_left_clicked() {
    yoga_sequence_.Previous();
}
