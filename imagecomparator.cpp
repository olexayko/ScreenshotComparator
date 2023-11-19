// imagecomparator.cpp

#include "imagecomparator.h"

#include <QDateTime>

double ImageComparator::calculateSimilarity(QImage image1, QImage image2)
{
    if (image1.size() != image2.size()) {
        qDebug() << "Images have different sizes:";
        qDebug() << "Image1 size:" << image1.size();
        qDebug() << "Image2 size:" << image2.size();
        return -1;
    }

    int matchingPixels = 0;
    int totalPixels = image1.width() * image1.height();

    for (int y = 0; y < image1.height(); y++ ) {
        for (int x = 0; x < image1.width(); x++) {
            QRgb pixel1 = image1.pixel(x, y);
            QRgb pixel2 = image2.pixel(x, y);

            if (pixel1 == pixel2) {
                matchingPixels++;
            }
        }
    }

    double similarityPercentage = (static_cast<double>(matchingPixels) / totalPixels) * 100.0;

    qDebug() << "Non-matching pixels:" << totalPixels - matchingPixels;
    qDebug() << "Matching pixels:" << matchingPixels;
    qDebug() << "Similarity Percentage:" << similarityPercentage << "%";

    return similarityPercentage;
}
