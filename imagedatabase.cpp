// imagedatabase.cpp

#include "imagedatabase.h"
#include <QBuffer>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QPixmap>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>
#include <QScreen>

ImageDatabase::ImageDatabase()
{
    initializeDatabase();
}

void ImageDatabase::initializeDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("imagedb.sqlite");

    if (!db.open()) {
        qDebug() << "Error: Unable to open the database";
    }

    createTable();
}

bool ImageDatabase::createTable()
{
    QSqlQuery query;
    QString queryString = "CREATE TABLE IF NOT EXISTS images ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "image BLOB, "
                          "hash TEXT, "
                          "similarity_percentage REAL, "
                          "date INTEGER)";
    if (!query.exec(queryString)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return false;
    }

    return true;
}



bool ImageDatabase::saveImage(const Image &image)
{
    QSqlQuery query;

    query.prepare("INSERT INTO images (image, hash, similarity_percentage, date) VALUES (:image, :hash, :similarity, :date)");

    QByteArray imageData = image.asByteArray();

    query.bindValue(":image", imageData);
    query.bindValue(":hash", QString(image.getHash().toHex()));
    query.bindValue(":similarity", image.getSimilarityPercentage());
    query.bindValue(":date", image.getDate());

    if (!query.exec()) {
        qDebug() << "Error saving image:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<Image> ImageDatabase::getImages()
{
    QVector<Image> images;

    QSqlQuery query("SELECT image, hash, similarity_percentage, date FROM images ORDER BY date DESC");
    while (query.next()) {
        QByteArray imageData = query.value(0).toByteArray();
        QImage image;
        image.loadFromData(imageData);

        QByteArray hash = QByteArray::fromHex(query.value(1).toByteArray());
        double similarity = query.value(2).toDouble();
        qlonglong date = query.value(3).toLongLong();

        Image loadedImage(image, hash, similarity, date);
        images.append(loadedImage);
    }
    return images;
}

Image ImageDatabase::getLastImage()
{
    auto images = getImages();
    if (images.size() > 0)
    {
        //qDebug() << "LATEST: " << QDateTime::fromMSecsSinceEpoch(images[0].getDate()).toString("yyyy-MM-dd hh:mm:ss");
        return images[0];
    }
}
