#ifndef MESSAGE_1_H
#define MESSAGE_1_H

#pragma pack(push, 1)
struct Message1
{
public:
    quint16 header = 0xABCD;
    quint16 height;

    explicit Message1(quint16 height = 0) : height{height}{}
};
#pragma pack(pop)

#endif //MESSAGE_1_H