#ifndef CIMAGE_H
#define CIMAGE_H

#include "SVG/Assets/cprimitive.h"
#include "Algebra/cmatrix.h"

/**
* @brief Работаем с картинками
*/
class CImage: public CPrimitive<CImage>
{
public:
    CImage(CPoint topLeft, QString type, QString encoding, QByteArray data);
    void cstream(QDataStream &dataStream, double scale) override;
    CPoints lianirize(double tol) const override;
    QString imageType() const;

private:
    QString _imgType;
    QString _encoding;
    QByteArray _data;
};

#endif // CIMAGE_H
