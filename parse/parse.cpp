#include "parse.h"

Datagram * ParseDatagram(__u8 *packet, __u16 len)
{
	__u8 sum = 0;
    Datagram * dg = (Datagram *)calloc(1, sizeof(Datagram) + len);
    for (int i = 0; i < len; i++)
        if (packet[0] == 0x3f)
            packet++;
        else
            break;
    if (packet[0] == 0x68)
        dg->packet_status |= PACKET_VALID_START;
    dg->len = *(__u16 *)(packet + 1);
    if (dg->len <= len)
        dg->packet_status |= PACKET_VALID_LEN;
    if (dg->packet_status & PACKET_VALID_LEN)
    {
        if (packet[dg->len - 1] == 0x16)
            dg->packet_status |= PACKET_VALID_END;
        if (dg->packet_status & PACKET_VALID_END)
        {
            for (int i = 3; i < dg->len - 2; i++)
                sum += packet[i];
            if (sum == packet[dg->len - 2])
                dg->packet_status |= PACKET_VALID_CHECKSUM;
        }
    }
    if (~dg->packet_status & PACKET_VALID)
        return dg;
    memcpy(dg->packet, packet, len);

    dg->prefix = (packet_prefix *)dg->packet;

    if (dg->prefix->info.info_down.commModuleID == 0)
    {
        dg->packet_addr.destinationAddr = NULL;
        dg->packet_addr.relayAddr = NULL;
        dg->packet_addr.sourceAddr = NULL;
    }
    else
    {
        if (dg->prefix->control.DIR == 0)
        {
            dg->packet_addr.sourceAddr = (__u8 *)dg->prefix + 10 /* sizeof(packet_prefix) */; 
            if (dg->prefix->info.info_down.relayLevel != 0)
            {
                dg->packet_addr.relayAddr = dg->packet_addr.sourceAddr + 6;
                dg->packet_addr.destinationAddr = dg->packet_addr.relayAddr + 6 * dg->prefix->info.info_down.relayLevel;
            }
            else
            {
                dg->packet_addr.relayAddr = NULL;
                dg->packet_addr.destinationAddr = dg->packet_addr.sourceAddr + 6;
            }
        }
        else
        {
            dg->packet_addr.sourceAddr = (__u8 *)dg->prefix + sizeof(packet_prefix); 
            dg->packet_addr.relayAddr = NULL;
            dg->packet_addr.destinationAddr = dg->packet_addr.sourceAddr + 6;
        }
    }

    dg->infix = (packet_infix *)(&(dg->prefix->info.info_up.reserve) + 1);
    if (dg->packet_addr.destinationAddr != 0)
        dg->infix = (packet_infix *)((__u8 *)dg->packet_addr.destinationAddr + 6);

    dg->userData = (__u8 *)dg->infix + sizeof(packet_infix);

    if (dg->packet[dg->len - 1] == 0x16)
        dg->postfix = (packet_postfix *)((__u8 *)dg->packet + len - 2);
    else
        dg->postfix = NULL;

    if (dg->postfix == NULL)
        dg->userDataLen = dg->packet + dg->len - dg->userData;
    else
        dg->userDataLen = (__u8 *)dg->postfix - (__u8 *)dg->userData;
    return dg;
}

Packet *ParsePacket(__u8 *data, __u8 len)
{
	Packet *pt;
	if ((pt = (Packet *)malloc(sizeof Packet + len)) == NULL)
		return NULL;
	memcpy(pt->packet, data, len);
	pt->packetlen = len;
	pt->addr = &data[1];
	*(__u8 *)&pt->control = data[8];
	pt->datalen = data[9];
	if (pt->datalen)
		pt->data = &data[10];
	else
		pt->data = NULL;
	pt->checksum = *(pt->data + pt->datalen);	
	if (pt->control.direction && pt->control.responseID) // 从站应答并且为异常应答侦
	{
		pt->further.funcode = NULL;
		pt->further.codelen = 0;
		pt->further.has_date = pt->further.has_operator =
		pt->further.has_password = 0;
		pt->further.date = pt->further._operator = 
		pt->further.password = NULL;
		pt->further.datalen = data[9]; // 此分支 data[9] == 1
		pt->further.data = &data[10];
	}
	else if (pt->control.direction) // 从站应答并且为正常应答侦
	{
		if (pt->datalen == 0)
		{
			pt->further.funcode = NULL;
			pt->further.codelen = 0;
			pt->further.has_date = pt->further.has_operator =
			pt->further.has_password = 0;
			pt->further.date = pt->further._operator = 
			pt->further.password = NULL;
			pt->further.data = NULL;
			pt->further.datalen = 0;
		}
		else
		{
			if (pt->control.responseID == 0x11
				|| pt->control.responseID == 0x12
				|| pt->control.responseID == 0x13)
			{
				pt->further.funcode = &data[10];
				pt->further.codelen = 4;
				if (pt->datalen > 5)
				{
					pt->further.datalen = pt->datalen - 5;
					pt->further.data = &data[14];
				}
			}
		}
	}
	return pt;
}