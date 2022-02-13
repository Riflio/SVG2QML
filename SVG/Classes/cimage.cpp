#include "cimage.h"
#include <QDebug>

CImage::CImage(CPoint topLeft, QString imgType, QString encoding, QByteArray data):
    CPrimitive(PT_IMAGE), _imgType(imgType), _encoding(encoding), _data(data)
{
    qDebug()<<"IMAGE"<<imgType<<encoding<<data.left(80);

    CPoints p;
    p.add(topLeft);
}

void CImage::cstream(QDataStream &dataStream, double scale)
{
    Q_UNUSED(dataStream);
    Q_UNUSED(scale);
    //TODO: Пора бы сделать
}

CPoints CImage::lianirize(double tol) const
{
    Q_UNUSED(tol);
    CPoints res;
    return res;
}

QString CImage::imageType() const
{
    return _imgType;
}
