// image.cpp

#include "image.h"

#include <QBuffer>
#include <QPixmap>

Image::Image(const QImage &image, const QByteArray &hash, double similarityPercentage, qlonglong dateInSeconds)
    : QImage(image), hash(hash), similarityPercentage(similarityPercentage), dateInSeconds(dateInSeconds)
{

}

QByteArray Image::getHash() const
{
    return hash;
}

double Image::getSimilarityPercentage() const
{
    return similarityPercentage;
}

qlonglong Image::getDate() const
{
    return dateInSeconds;
}

void Image::setHash(QByteArray h)
{
    hash = h;
}

void Image::setSimilarityPercentage(double similarity)
{
    similarityPercentage = similarity;
}

void Image::setDate(qlonglong date)
{
    dateInSeconds = date;
}

QByteArray Image::asByteArray() const
{
    QPixmap inPixmap = QPixmap::fromImage(*this);
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );

    return inByteArray;
}
