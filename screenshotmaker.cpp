// screenshotmaker.cpp

#include "mainwindow.h"
#include "screenshotmaker.h"
#include <QGuiApplication>
#include <QTimer>


ScreenshotMaker::ScreenshotMaker(MainWindow* mainWindow)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ScreenshotMaker::takeScreenshot);
    connect(this, &ScreenshotMaker::screenshotTaken, mainWindow, &MainWindow::screenshotTaken);
}

void ScreenshotMaker::start()
{
    if (!active)
    {
        active = true;
        timer->start(interval);
    }
}

void ScreenshotMaker::stop()
{
    active = false;
    timer->stop();
}

void ScreenshotMaker::invert()
{
    active ? stop() : start();
}

bool ScreenshotMaker::isActive()
{
    return active && timer->isActive();
}

void ScreenshotMaker::takeScreenshot()
{
    //qDebug() << "Screenshot taken";
    QPixmap screenshot = takeScreenshotPixmap();
    emit screenshotTaken(screenshot);

}

QPixmap ScreenshotMaker::takeScreenshotPixmap()
{
    lastScreenshotPixmap = QGuiApplication::primaryScreen()->grabWindow(0);
    return lastScreenshotPixmap;
}

QPixmap ScreenshotMaker::getLastScreenshotPixmap()
{
    return lastScreenshotPixmap;
}
