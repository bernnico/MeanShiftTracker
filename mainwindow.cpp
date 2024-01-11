#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    connect(ui->graphicsView, SIGNAL(rubberBandChanged(QRect,QPointF,QPointF)),
            this, SLOT(rubberBandChanged(QRect,QPointF,QPointF)));

    meanshift = new QCvMeanShiftThread();

    connect(meanshift, SIGNAL(newFrame(QPixmap)),
            this, SLOT(onNewFrame(QPixmap)));

    meanshift->start();

    ui->graphicsView->scene()->addItem(&pixmap);
}

MainWindow::~MainWindow()
{
    meanshift->requestInterruption();
    meanshift->wait();
    delete meanshift;

    delete ui;
}

void MainWindow::rubberBandChanged(QRect rect, QPointF frScn, QPointF toScn)
{
    Q_UNUSED(toScn);
    // Note: rect coordinates in the main windows
    QRect fromSceneRect(static_cast<int>(frScn.x()),
                        static_cast<int>(frScn.y()),
                        rect.width(),
                        rect.height());
    meanshift->setTrackRect(fromSceneRect);
}

void MainWindow::onNewFrame(QPixmap newFrame)
{
    pixmap.setPixmap(newFrame);
}

