#ifndef CAN_H
#define CAN_H

#include <string>
#include <linux/can.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstring> 
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can/raw.h>
#include <libsocketcan.h>
#include <linux/can/bcm.h>

#include "ISOBUSframe.hpp"

struct CanFrame
{
    uint32_t id = 0;
    uint8_t len = 0;
    uint8_t flags = 0;
    uint8_t data[64];
};

class Can {

    public:
        Can() {}
        bool init(std::string dev) {
            m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
            if (m_socket < 0) {
                perror("Failed to create CAN socket");
                return false;
            }
        // Set the bitrate to 500 kbps
            // int bitrate = 500000;
            // if (socket.canSetBitrate(bitrate) < 0) {
            //     std::cerr << "Failed to set bitrate" << std::endl;
            //     return -1;
            // }
            const char* cmd = "sudo ip link set vcan0 type can bitrate 10";
            int ret = system(cmd);
            if (ret == -1) {
                perror("Failed to set bitrate");
                return false;
                // error handling
            }

            struct sockaddr_can addr;
            struct ifreq ifr;

            std::strcpy(ifr.ifr_name, dev.c_str());

            // To determine the interface index an appropriate ioctl() has to be used
            if (ioctl(m_socket, SIOCGIFINDEX, &ifr) == -1)
            {
                perror("Failed to get CAN interface index");
                close(m_socket);
                return false;
            }

            addr.can_family = AF_CAN;
            addr.can_ifindex = ifr.ifr_ifindex;
            if (bind(m_socket, (sockaddr*)&addr, sizeof(addr)) == -1)
            {
                perror("Failed to bind CAN socket to interface");
                close(m_socket);
                return false;
            }
   
            return true;
        }

        bool processReceivedFrames(CanFrame & msg) {
            //struct can_frame frame;
            struct canfd_frame frame;
            auto nbytes = read(m_socket, &frame, sizeof(struct can_frame));

            if (nbytes < 0) {
                perror("can raw socket read");
                return 1;
            }

            /* paranoid check ... */
            if (nbytes < sizeof(struct can_frame)) {
                fprintf(stderr, "read: incomplete CAN frame\n");
                return 1;
            }
            // msg.id = frame.can_id;
            // msg.len = frame.len;
            // msg.flags = frame.flags;
            // memcpy(msg.data, frame.data, frame.len);
            ISOBUSFrame iframe(frame.can_id, frame.data, frame.len);
            // emit this iframe to NMEA2000 part of the code (ros message here)
            
            msg.id = frame.can_id;
            msg.len = frame.len;
            msg.flags = frame.flags;
            memcpy(msg.data, frame.data, frame.len);

            return true;
        }


        ~Can()
        {
            close(m_socket);
        }
    private:
        int m_socket = -1;
};

#endif