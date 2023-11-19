// image.h

#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>

class Image : public QImage {

public:
    Image(const QImage &image, const QByteArray &hash, double similarityPercentage, qlonglong dateInSeconds);

    QByteArray getHash() const;
    double getSimilarityPercentage() const;
    qlonglong getDate() const;
    QByteArray asByteArray() const;

    void setHash(QByteArray hash) ;
    void setSimilarityPercentage(double similarity);
    void setDate(qlonglong date);

private:
    QByteArray hash;
    double similarityPercentage;
    qlonglong dateInSeconds;
};

#endif // IMAGE_H

