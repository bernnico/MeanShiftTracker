#include "qcvmeanshiftthread.h"

#include <QMutexLocker>
#include <QImage>

QCvMeanShiftThread::QCvMeanShiftThread(QObject *parent)
    : QThread{parent}
{}

void QCvMeanShiftThread::setTrackRect(QRect rect)
{
    QMutexLocker locker(&rectMutex);
    if ((rect.width() > 2) && (rect.height() > 2)) {
        trackRect.x = rect.left();
        trackRect.y = rect.top();
        trackRect.width = rect.width();
        trackRect.height = rect.height();
        updateHistogram = true;
    }
}

// void QCvMeanShiftThread::newFrame(QPixmap pixmap)
// {}

void QCvMeanShiftThread::run()
{
    cv::VideoCapture video;

    video.open(0);
    while (video.isOpened() && !this->isInterruptionRequested()) {
        cv::Mat frame;
        video >> frame;

        if (frame.empty())
            // break;
            continue;

        // tracking code
        if (/*false &&*/ trackRect.size().area() > 0) {
            QMutexLocker locker(&rectMutex);

            cv::Mat hsv, hue, hist;

            cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

            hue.create(hsv.size(), hsv.depth());

            float hrange[] = {0,179};
            const float *ranges[] = {hrange};
            int bins[] = {24};
            int fromto[] = {0,0};

            cv::mixChannels(&hsv, 1, &hue, 1, fromto, 1);

            if (updateHistogram) {
                cv::Mat roi(hue, trackRect);
                cv::calcHist(&roi, 1, 0, cv::Mat(), hist, 1, bins, ranges);

                normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

                updateHistogram = false;
            }

            cv::Mat backProj;
            cv::calcBackProject(&hue, 1, 0, hist, backProj, ranges);

            cv::TermCriteria criteria;
            criteria.maxCount = 5;
            criteria.epsilon = 3;
            criteria.type = cv::TermCriteria::EPS;
            cv::meanShift(backProj, trackRect, criteria);

            cv::rectangle(frame, trackRect, cv::Scalar(0,0,255), 2);
        }
        // cv::rectangle(frame, trackRect, cv::Scalar(0,0,255), 2);

        emit newFrame(
            QPixmap::fromImage(
                QImage(
                    frame.data,
                    frame.cols,
                    frame.rows,
                    frame.step,
                    QImage::Format_RGB888)
                    .rgbSwapped()));
    }
}
