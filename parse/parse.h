#ifndef _PARSE_H_
#define _PARSE_H_

#if defined __cplusplus
extern "C" {
#endif

#pragma warning(disable:4200)
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned int __u32;
typedef unsigned long long __u64;

typedef char __i8;
typedef short __i16;
typedef int __i32;
typedef long long __i64;

/**
 * Q/GDW 376.2 ½âÎö
 */
#define PACKET_VALID_START 0x1
#define PACKET_VALID_END 0x2
#define PACKET_VALID_LEN 0x4
#define PACKET_VALID_CHECKSUM 0x8

#define PACKET_VALID (PACKET_VALID_START | PACKET_VALID_END | PACKET_VALID_LEN | PACKET_VALID_CHECKSUM)

#pragma pack(1)
typedef struct  packet_prefix {
    __u8 start;
    __u16 len;
    struct {
        __u8 commMode:6;
        __u8 PRM:1;
        __u8 DIR:1;
    } control;
    union {
        struct {
            __u8 routerID:1;
            __u8 affiliatedNodeID:1;
            __u8 commModuleID:1;
            __u8 collisionDetection:1;
            __u8 relayLevel:4;

            __u8 channelID:4;
            __u8 errorCorrectionCodeID:4;

            __u8 responseBytes;

            __u16 commBaudRate:15;
            __u16 rateUnitID:1;

            __u8 reserve;
        } info_down;

        struct {
            __u8 routerID:1;
            __u8 no_use1:1;
            __u8 commModuleID:1;
            __u8 no_use2:1;
            __u8 relayLevel:4;

            __u8 channelID:4;
            __u8 no_use3:4;

            __u8 phaseLineMark:4;
            __u8 meterChannelFeature:4;

            __u8 lastCommandSignalQuality:4;
            __u8 lastResponseSignalQuality:4;
            __u16 reserve;
        } info_up;
    }info;
} packet_prefix;
#pragma pack()

#pragma pack(1)
typedef struct packet_infix {
    __u8 AFN;
    __u8 DT1;
    __u8 DT2;
} packet_infix;
#pragma pack()

#pragma pack(1)
typedef struct packet_postfix {
    __u8 checkSum;
    __u8 end;
} packet_postfix;
#pragma pack()

#pragma pack(4)
typedef struct packet_addr {
    __u8 *sourceAddr;
    __u8 *relayAddr;
    __u8 *destinationAddr;
} packet_addr;
#pragma pack()

#pragma pack(4)
typedef struct datagram {
    packet_prefix * prefix;
    __u32 packet_status;
    packet_addr packet_addr;
    packet_infix * infix;
    packet_postfix * postfix;
    __u16 userDataLen;
    __u8 * userData;
    __u16 len;
    __u8 packet[];
} Datagram;
#pragma pack()

#pragma pack(1)
typedef struct _dcontrol
{
  __u8 funcode:5;
  __u8 subframes:1;
  __u8 responseID:1;
  __u8 direction:1;
} dcontrol;
#pragma pack()

#pragma pack(1)
typedef struct {
	__u8 *addr;
	struct _control
    {
        __u8 funcode:5;
        __u8 subframes:1;
        __u8 responseID:1;
        __u8 direction:1;
    } control;	
    __u8 datalen;
    __u8 *data;
	__u8 checksum;
	struct _further {
		__u8 *funcode;
		__u8 codelen;
		__u8 has_date;
		__u8 has_operator;
		__u8 has_password;
		__u8 *date;
		__u8 *_operator;
		__u8 *password;
		__u8 *data;
		__u8 datalen;
	} further;
    __u8 packetlen;
    __u8 packet[];
} Packet;

#pragma pack(1)
typedef struct _err {
	__u8 other:1;
	__u8 request:1;
	__u8 password:1;
	__u8 baudrate:1;
	__u8 year:1;
	__u8 day:1;
	__u8 cost:1;
	__u8 reserve:1;
} Error;
#pragma pack()

Datagram * ParseDatagram(__u8 *packet, __u16 len);
Packet *ParsePacket(__u8 *data, __u16 len);
__u8 GetFn(__u8  dt1, __u8 dt2);
int CorrectDatagram(__u8 *packet, __u16 len);
#if defined __cplusplus
}
#endif

#endif