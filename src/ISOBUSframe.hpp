#ifndef ISOBUSFRAME_H
#define ISOBUSFRAME_H

#include <stdint.h>
#include <string.h>
#include <iostream>
class ISOBUSFrame
{
public:
    uint8_t priority = 0;
    bool page = false;
    uint16_t PGN = 0;
    uint8_t SA = 0;
    uint8_t PF = 0;
    uint8_t PS = 0;
    uint8_t data[8];

    ISOBUSFrame() {}

    ISOBUSFrame(const ISOBUSFrame &other)
    {
        priority = other.priority;
        page = other.page;
        PGN = other.PGN;
        SA = other.SA;
        PF = other.PF;
        PS = other.PS;
        memcpy(data, other.data, sizeof(data));
    }

    ISOBUSFrame(uint32_t id, const uint8_t* payload, size_t size)
    {
        SA = (uint8_t)((id >> 0) & 0xFF);
        PGN = (uint16_t)((id >> 8) & 0xFFFF);
        std::cout << "My uint8 value in hex is: 0x" << std::hex << static_cast<int>(PGN) << std::endl;
        page = (bool)((id >> 24) & 0x01);
        priority = (uint8_t)((id >> 26) & 0x07);

        PF = (uint8_t)((PGN >> 8) & 0xFF);
        PS = (uint8_t)((PGN >> 0) & 0xFF);

        memcpy(data, payload, size);
    }

    uint32_t getCanID() const
    {
        if(SA != 0)
            return (priority<<26) | (page << 24) | (PGN << 8) | (SA);
        else
            return 0;
    }
};

#endif // ISOBUSFRAME_H
