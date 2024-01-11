#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>

// #include <QRect>
#include <QPointF>

#include "qcvmeanshiftthread.h"

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

private slots:
    void rubberBandChanged(QRect rect, // TODO
                           QPointF frScn,
                           QPointF toScn);
    void onNewFrame(QPixmap newFrame);

private:
    Ui::MainWindow *ui;

    QCvMeanShiftThread *meanshift;
    QGraphicsPixmapItem pixmap;
};
#endif // MAINWINDOW_H
