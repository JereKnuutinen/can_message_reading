#include <linux/can.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can/raw.h>
#include <can.hpp>

int main (int argc, char** argv) {
    Can sockat_can;
    sockat_can.init("vcan0");
    CanFrame fr;
    //bool temp = sockat_can.processReceivedFrames(fr);
    while(sockat_can.processReceivedFrames(fr) == true)
    {
        printf("len %d byte, id: %d, data: %02x %02x %02x %02x %02x %02x %02x %02x  \n", fr.len, fr.id, 
            fr.data[0], fr.data[1], fr.data[2], fr.data[3],
            fr.data[4], fr.data[5], fr.data[6], fr.data[7]);
    }
}