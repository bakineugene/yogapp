#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QTimer>
#include <QList>
#include <QPixmap>

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
    void SetDirectory(const QString& directory);
    QPixmap GetCurrentFile();

private slots:
    void on_btn_right_clicked();
    void on_btn_left_clicked();

private:
    void resizeEvent(QResizeEvent *event) override;
    void UpdateEnabled();

private:
    // TODO: single struct
    QDir images_directory_;
    size_t current_file_index_{};
    QList<QPixmap> files_list_{};

    Ui::MainWindow *ui_;
    QLabel picture_label_{this};
    QPixmap active_pixmap_;
    QTimer timer_{this};

};
#endif // MAINWINDOW_H
