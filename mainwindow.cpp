#include <QBuffer>
#include <QCryptographicHash>
#include <QThread>
#include <QDateTime>
#include <Qvector>

#include "mainwindow.h"
#include "image.h"
#include "imagedatabase.h"
#include <vector>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageDatabase = new ImageDatabase();
    screenshotMaker = new ScreenshotMaker(this);
    imageComparator = new ImageComparator();

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget);

    launchButton = new QPushButton("Start screenshoting && comparing", this);
    launchButton->setFixedHeight(50);
    launchButton->setStyleSheet("font-size: 24px;");
    connect(launchButton, &QPushButton::clicked, [&]() {
        screenshotMaker->invert();
        launchButton->setText(QString(!screenshotMaker->isActive() ? "Start" : "Stop") + " screenshoting && comparing");
        updateGrid(imageDatabase->getImages());
    });

    connect(screenshotMaker, &ScreenshotMaker::screenshotTaken, this, &MainWindow::onScreenShotTaken);
    verticalLayout->addWidget(launchButton);

    createScrollArea(imageDatabase->getImages());
    verticalLayout->addWidget(scrollArea);

    const char* qtVersion = qVersion();
    int cppVersion = __cplusplus / 100;
    QString str = QString("C++ Standard Version: %1 with QT version: %2").arg(cppVersion).arg(qtVersion);
    setWindowTitle(str);

    showMaximized();
}



void MainWindow::createScrollArea(QVector<Image> images)
{
    scrollArea = new QScrollArea(this);
    scrollAreaWidget = new QWidget(scrollArea);
    gridLayout = new QGridLayout(scrollAreaWidget);
    gridLayout->setSpacing(50);

    fillGrid(images);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
}

void MainWindow::onScreenShotTaken()
{
    //qDebug() << "void MainWindow::onScreenShotTaken()";
    QImage image = screenshotMaker->getLastScreenshotPixmap().toImage();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    auto hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);

    Image img(image, hash, 100, QDateTime::currentMSecsSinceEpoch());
    if (imageDatabase->getImages().size() >= 1) {
        compareImagesParallel(imageDatabase->getLastImage(), img, imageDatabase, imageComparator);
    }
    else {
        imageDatabase->saveImage(img);
        updateGrid(imageDatabase->getImages());
    }
}

void MainWindow::fillGrid(QVector<Image> images) {
    int labelWidth = 1142;
    int labelHeight = 642;
    for (int i = 0; i < images.size(); i++) {
        QPixmap pixmap = QPixmap::fromImage(images[i]);
        pixmap = pixmap.scaled(labelWidth, labelHeight);

        QLabel* imageLabel = new QLabel(this);
        imageLabel->setPixmap(pixmap);
        imageLabel->setFixedSize(labelWidth, labelHeight);

        gridLayout->addWidget(imageLabel, i, 0);

        auto similarity = images[i].getSimilarityPercentage();
        auto date = QDateTime::fromMSecsSinceEpoch(images[i].getDate()).toString("yyyy-MM-dd hh:mm:ss");
        auto hash = images[i].getHash();
        //qDebug() << "SIMILARITY FROM DB = " << images[i].getSimilarityPercentage();
        QLabel* infoLabel = new QLabel(QString("Similarity: %1%\nDate: %2\nHash:%3").arg(similarity).arg(date).arg(QString(hash.toHex())), this);
        QFont font = infoLabel->font();
        font.setPointSize(24);
        infoLabel->setFont(font);
        gridLayout->addWidget(infoLabel, i, 1);
    }
}

void MainWindow::clearGrid() {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::updateGrid(QVector<Image> images) {
    clearGrid();
    fillGrid(images);
}

void MainWindow::compareImagesParallel(Image lastImage, Image newImage, ImageDatabase* idb, ImageComparator* ic)
{
    workerThreads.push_back(new QThread);
    int i = workerThreads.size() - 1;
    QObject::connect(workerThreads[i], &QThread::started, [lastImage, newImage, &idb, &ic, this]() {
        Image lastImg = lastImage;
        Image newImg = newImage;

        //qDebug() << "comparing new screenshot with date: " << QDateTime::fromMSecsSinceEpoch(newImage.getDate()).toString("yyyy-MM-dd hh:mm:ss");
        //qDebug() << "comparing old screenshot with date: " << QDateTime::fromMSecsSinceEpoch(lastImage.getDate()).toString("yyyy-MM-dd hh:mm:ss");

        double similarity = ic->calculateSimilarity(lastImg, newImg);
        newImg.setSimilarityPercentage(similarity);


        QMetaObject::invokeMethod(this, [this, idb, newImg]() {
                idb->saveImage(newImg);
                updateGrid(idb->getImages());
            }, Qt::QueuedConnection);

    });
    workerThreads[i]->start();
}
