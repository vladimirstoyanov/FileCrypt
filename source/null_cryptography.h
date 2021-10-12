#ifndef NULLCRYPTOGRAPHY_H
#define NULLCRYPTOGRAPHY_H

#include <QDebug>
#include "cryptography.h"

//null object design pattern
class NullCryptography: public ICryptography
{
public:
    NullCryptography();
    void execute () override { qDebug()<<__PRETTY_FUNCTION__; }
};

#endif // NULLCRYPTOGRAPHY_H
