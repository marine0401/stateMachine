#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>

#define SYNC 0x57


typedef enum{
    GET_MESSAGE = 0x01,
}eCmdID; // e代表枚举

typedef enum{
    MAC_ADDRESS = 0x01,
}eContent; // e代表枚举

typedef enum{
    MALLOC_SUCCESS = 0x00,
    MALLOC_ERROR = 0x01
}eErrorMsg; // e代表枚举

struct packet{
    unsigned char sync = 0x57;
    unsigned short length;
    unsigned char cmdID;
    unsigned char data[0];
}__attribute__((gcc_struct,packed));
typedef struct packet Packet;

QByteArray commonPacket(const eCmdID cmd, unsigned char content ,const QByteArray &data, eErrorMsg *errorMsg = nullptr);



#endif // PROTOCOL_H
