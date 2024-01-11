#ifndef QCVMEANSHIFTTHREAD_H
#define QCVMEANSHIFTTHREAD_H

// #include <QObject>
#include <QThread>
#include <QRect>
#include <QPixmap>
#include <QMutex>

#include <opencv2/opencv.hpp>

class QCvMeanShiftThread : public QThread
{
    Q_OBJECT
public:
    explicit QCvMeanShiftThread(QObject *parent = nullptr);

public slots:
    /**
     * @brief setTrackRect sets the initial MeanShift track window
     * @param rect
     */
    void setTrackRect(QRect rect);

signals:
    /**
     * @brief newFrame will be emitted after each frame is processed
     *      so that the GUI can display it.
     * @param pix
     */
    void newFrame(QPixmap pix);

private:
    void run();

    cv::Rect trackRect;
    QMutex rectMutex;
    bool updateHistogram;
};

#endif // QCVMEANSHIFTTHREAD_H
