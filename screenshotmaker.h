// screenshotmaker.h
#pragma once
#ifndef SCREENSHOTMAKER_H
#define SCREENSHOTMAKER_H

//#include "mainwindow.h"

#include <QPixmap>
#include <QScreen>
#include <QTimer>
#include <QObject>
#include <QCoreApplication>
#include <QDebug>

class MainWindow;
class ScreenshotMaker: public QObject
{
Q_OBJECT

public:
    ScreenshotMaker(MainWindow* mainWindow);

    void invert();
    bool isActive();

private:
    bool active = false;
    int interval = 60 * 1000;
    QPixmap lastScreenshotPixmap;
    QTimer* timer;

    void start();
    void stop();

public slots:
    void takeScreenshot();
    QPixmap takeScreenshotPixmap();
    QPixmap getLastScreenshotPixmap();


signals:
    void screenshotTaken();
};

#endif // SCREENSHOTMAKER_H
