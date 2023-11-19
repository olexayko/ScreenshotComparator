#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
#include "image.h"
#include "imagecomparator.h"
#include "imagedatabase.h"
#include "screenshotmaker.h"

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void createScrollArea(QVector<Image> images);
    void fillGrid(QVector<Image> images);
    void clearGrid();
    void updateGrid(QVector<Image> images);
    void compareImagesParallel(Image pixmap1, Image pixmap2, ImageDatabase* idb, ImageComparator* ic);

    ImageDatabase* imageDatabase;
    ScreenshotMaker* screenshotMaker;
    ImageComparator* imageComparator;
    QVector<QThread*> workerThreads;

    QPushButton *launchButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidget;
    QGridLayout *gridLayout;

signals:
    void screenshotTaken();
public slots:
    void onScreenShotTaken();
};

#endif // MAINWINDOW_H
