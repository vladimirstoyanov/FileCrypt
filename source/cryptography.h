#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

class ICryptography
{
public:
    ICryptography();
    virtual ~ICryptography ();

    virtual void execute () = 0;
};

#endif // CRYPTOGRAPHY_H
