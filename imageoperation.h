#ifndef IMAGEOPERATION_H
#define IMAGEOPERATION_H

#include<QObject>

class imageOperation : public QObject
{
public:
    imageOperation();
public slots:
    void renewAlbum_rebuild();
    void renewAlbun_biaod();
};

#endif // IMAGEOPERATION_H
