#include <QDebug>

#include "protocol.h"



/**
 * @brief commonPacket
 * @param cmd  {@link eCmdID}
 * @param content dataCollection
 * @param data
 * @param errorMsg
 * @return
 */
QByteArray commonPacket(const eCmdID cmd, unsigned char content ,const QByteArray &data, eErrorMsg *errorMsg)
{
    int packetLen = sizeof(Packet) + data.length(); // 1个字节给 data
    Packet *protocolPacket = (Packet * )malloc(packetLen);
    memset(protocolPacket, 0 , packetLen);
    if(nullptr == protocolPacket)
    {
        if(errorMsg != nullptr)
        {
           *errorMsg = MALLOC_ERROR;
        }
        return nullptr;
    }
    protocolPacket->sync = SYNC;
    protocolPacket->length = packetLen;
    protocolPacket->cmdID = cmd;
    protocolPacket->data[0] = content;
    memcpy(protocolPacket->data+1, data.data(), data.length());
    QByteArray packet((char *)protocolPacket, packetLen);
    free(protocolPacket); // 记得释放内存
    if(errorMsg != nullptr)
    {
        *errorMsg = MALLOC_SUCCESS;
    }

    return packet;
}
