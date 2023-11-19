// imagecomparator.h
#pragma once
#ifndef IMAGECOMPARATOR_H
#define IMAGECOMPARATOR_H

#include <QImage>
#include <QDebug>

class ImageComparator {
public:
    double calculateSimilarity(QImage image1, QImage image2);
};

#endif // IMAGECOMPARATOR_H
