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

    yoga_sequence_ = {
        QPair<QString, int>("mountain", 3000),
        QPair<QString, int>("extended_mountain", 3000),
        QPair<QString, int>("half_forward_fold", 3000),
        QPair<QString, int>("forward_fold", 3000),
        QPair<QString, int>("four_limbed_staff", 3000),
        QPair<QString, int>("up_dog", 3000),
        QPair<QString, int>("down_dog", 5000),
        QPair<QString, int>("low_lunge", 3000),
        QPair<QString, int>("four_limbed_staff", 2000),
        QPair<QString, int>("up_dog", 3000),
        QPair<QString, int>("down_dog", 5000),
        QPair<QString, int>("low_lunge", 3000),
        QPair<QString, int>("four_limbed_staff", 2000),
        QPair<QString, int>("up_dog", 3000),
        QPair<QString, int>("down_dog", 3000),
        QPair<QString, int>("forward_fold", 3000),
        QPair<QString, int>("extended_mountain", 3000),
        QPair<QString, int>("mountain", 3000)
    };

    current_pose_ = yoga_sequence_.begin();
    RunPose();
}

MainWindow::~MainWindow() {
    delete ui_;
}

QPixmap MainWindow::GetCurrentFile() {
    if (current_pose_ == yoga_sequence_.end()) return QPixmap{};

    auto path = ":/images/images/" + current_pose_->first + ".jpeg";
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
    NextPose();
}

void MainWindow::OnPlayerFinish(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        NextPose();
    }
}

void MainWindow::NextPose() {
    if (current_pose_ != yoga_sequence_.end()) {
        ++current_pose_;
    }
    RunPose();
}

void MainWindow::RunPose() {
    if (current_pose_ != yoga_sequence_.end()) {
        player_.setSource(QUrl{"qrc:/sounds/sounds/" + current_pose_->first + ".mp3"});
        UpdateImage();
        UpdateEnabled();

        // TODO: somehow compose timer and player
        // timer_.setInterval(current_pose_->second);
        // timer_.start();

        player_.play();
    }
}

void MainWindow::UpdateEnabled() {
    ui_->btn_left->setEnabled(current_pose_ != yoga_sequence_.begin());
    ui_->btn_right->setEnabled(current_pose_ + 1 != yoga_sequence_.end());
}

void MainWindow::on_btn_right_clicked() {
    if (current_pose_ != yoga_sequence_.end()){
        ++current_pose_;
        RunPose();
    }
}

void MainWindow::on_btn_left_clicked() {
    if (current_pose_ != yoga_sequence_.begin()){
        --current_pose_;
        RunPose();
    }
}
