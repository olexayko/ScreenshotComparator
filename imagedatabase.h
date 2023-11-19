// imagedatabase.h
#pragma once
#ifndef IMAGEDATABASE_H
#define IMAGEDATABASE_H


#include "image.h"

#include <QtSql/QSqlDatabase>

class ImageDatabase {
public:
    ImageDatabase();

    bool saveImage(const Image &image);
    QVector<Image> getImages();
    Image getLastImage();

private:
    QSqlDatabase db;
    bool createTable();
    void initializeDatabase();
};

#endif // IMAGEDATABASE_H
