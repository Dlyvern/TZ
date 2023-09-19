#ifndef TZ_MESSAGE1_H
#define TZ_MESSAGE1_H

struct Message1
{
    quint16 header = 0xABCD;
    quint16 height;

    explicit Message1(quint16 height = 0)
    {
        this->height = height;
    }
};

#endif //TZ_MESSAGE1_H
