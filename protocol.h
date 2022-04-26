#ifndef PROTOCOL_H
#define PROTOCOL_H

#define SYNC 0x57
struct packet{
   unsigned char sync;
   unsigned short length;
   unsigned char cmdID;
   unsigned char data[0]; //0 长数组
}__attribute__((gcc_struct, packed)); //在gcc 中，一般不加 gcc_struct
typedef struct packet Packet;


#endif // PROTOCOL_H
