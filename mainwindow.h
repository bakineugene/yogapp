#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QList>
#include <QPixmap>

#include "yogasequence.h"

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

    void SetPixmap(const QPixmap pixmap) {
        active_pixmap_ = pixmap;
    }

    void UpdateImage();
    QPixmap GetCurrentFile();

private slots:
    void on_btn_right_clicked();
    void on_btn_left_clicked();
    void OnTimer();
    void OnPlayerFinish(QMediaPlayer::MediaStatus status);

private:
    void resizeEvent(QResizeEvent *event) override;
    void UpdateEnabled();
    void NextPose();
    void RunPose(const YogaPose&);

private:
    Ui::MainWindow *ui_;
    QLabel picture_label_{this};
    QPixmap active_pixmap_;

    QTimer timer_{this};
    QMediaPlayer player_{};
    QAudioOutput audio_output_;

    YogaSequence yoga_sequence_;


};
#endif // MAINWINDOW_H
