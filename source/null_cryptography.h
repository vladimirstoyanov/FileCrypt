#ifndef NULLCRYPTOGRAPHY_H
#define NULLCRYPTOGRAPHY_H

#include "cryptography.h"

class NullCryptography: public ICryptography
{
public:
    NullCryptography();
    void execute () override { }
};

#endif // NULLCRYPTOGRAPHY_H
