#include "PacketHandlerLC.hpp"

#include <Metazion/Share/Misc/MemoryInputStream.hpp>

#include "Common/Packet/PacketLC.hpp"

#include "UI/UIMsgDispatcher.h"

void PacketHandlerLC::Handle(int command, const void* data, int length) {
    ::printf("Command[%d] data[%p] length[%d]\n", command, data, length);

    switch (command) {
    case COMMAND_LC_CONNECTED:
        HandleConnected(data, length);
        break;
    case COMMAND_LC_DISCONNECTED:
        HandleDisconnected(data, length);
        break;
    case COMMAND_LC_PLAYERLOGIN:
        HandlePlayerLogin(data, length);
        break;
    case COMMAND_LC_SERVERLIST:
        HandleServerList(data, length);
        break;
    default: break;
    }
}

void PacketHandlerLC::HandleConnected(const void* data, int length) {

}

void PacketHandlerLC::HandleDisconnected(const void* data, int length) {

}

void PacketHandlerLC::HandlePlayerLogin(const void* data, int length) {
    UIMsgDispatcher::Instance().Notify(UIMSG_LOGIN_RSP
        , reinterpret_cast<uint64_t>(data), length);
}

void PacketHandlerLC::HandleServerList(const void* data, int length) {
    UIMsgDispatcher::Instance().Notify(UIMSG_SELECTSERVER_SERVERLIST
        , reinterpret_cast<uint64_t>(data), length);
}