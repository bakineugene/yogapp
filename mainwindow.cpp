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

    SetDirectory(":/yoga/images/poses");
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::UpdateImage() {
    auto pixmap = ResizeImgToFit(GetCurrentFile(), width(), height());
    picture_label_.setPixmap(pixmap);
    int label_x = (width() - pixmap.width()) / 2;
    int label_y = (height() - pixmap.height()) / 2;

    picture_label_.setGeometry(label_x, label_y, pixmap.width(), pixmap.height());
}

void MainWindow::SetDirectory(const QString &directory) {
    current_file_index_ = 0;
    images_directory_ = QDir{directory};

    for (auto file : images_directory_.entryList()) {
        QPixmap pixmap{images_directory_.filePath(file)};

        if (!pixmap.isNull()) {
            files_list_.append(pixmap);
        }
    }

    UpdateImage();
    UpdateEnabled();
}

QPixmap MainWindow::GetCurrentFile() {
    if (files_list_.isEmpty()) return QPixmap{};

    return files_list_[current_file_index_];
}

void MainWindow::resizeEvent(QResizeEvent*) {
    UpdateImage();
}

void MainWindow::UpdateEnabled() {
    ui_->btn_left->setEnabled(current_file_index_ > 0);
    ui_->btn_right->setEnabled(current_file_index_ + 1 < (size_t) files_list_.size());
}

void MainWindow::on_btn_right_clicked() {
    ++current_file_index_;
    UpdateImage();
    UpdateEnabled();
}

void MainWindow::on_btn_left_clicked() {
    --current_file_index_;
    UpdateImage();
    UpdateEnabled();
}
