#include "PacketHandlerGC.hpp"

#include "Common/Packet/PacketGC.hpp"

#include "AppClient.h"
#include "UI/UIMsgDispatcher.h"

void PacketHandlerGC::Handle(int command, const void* data, int length) {
    ::printf("Command[%d] data[%p] length[%d]\n", command, data, length);

    switch (command) {
    case COMMAND_GC_CONNECTED:
        HandleConnected(data, length);
        break;
    case COMMAND_GC_DISCONNECTED:
        HandleDisconnected(data, length);
        break;
    case COMMAND_GC_CONNECTFAILED:
        HandleConnectFailed(data, length);
        break;
    default: break;
    }
}

void PacketHandlerGC::HandleConnected(const void* data, int length) {
    g_appClient->SetConnected(true);
}

void PacketHandlerGC::HandleDisconnected(const void* data, int length) {
    g_appClient->SetConnected(false);
}

void PacketHandlerGC::HandleConnectFailed(const void* data, int length) {
    
}