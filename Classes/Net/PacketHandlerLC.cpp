#include "PacketHandlerLC.hpp"

#include <Metazion/Share/Misc/MemoryInputStream.hpp>

#include "Common/Packet/PacketLC.hpp"

#include "AppClient.h"
#include "Data/ServerGroupManager.hpp"
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
    case COMMAND_LC_CONNECTFAILED:
        HandleConnectFailed(data, length);
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
    g_appClient->SetConnected(true);
}

void PacketHandlerLC::HandleDisconnected(const void* data, int length) {
    g_appClient->SetConnected(false);
}

void PacketHandlerLC::HandleConnectFailed(const void* data, int length) {
    
}

void PacketHandlerLC::HandlePlayerLogin(const void* data, int length) {
    UIMsgDispatcher::Instance().Notify(UIMSG_LOGIN_RSP
        , reinterpret_cast<uint64_t>(data), length);
}

void PacketHandlerLC::HandleServerList(const void* data, int length) {
    NS_MZ_SHARE::MemoryInputStream inputStream;
    inputStream.Attach(data, length);

    g_appClient->m_socketCL->SetReconnectInterval(-1);
    g_appClient->m_socketCL->Close();

    ServerGroupManager::Instance().RemoveAllServerGroup();

    int8_t serverGroupSize = 0;
    inputStream.ReadInt8(serverGroupSize);

    for (auto index = 0; index < serverGroupSize; ++index) {
        int8_t id = 0;
        inputStream.ReadInt8(id);
        char name[32] = { '\0' };
        inputStream.ReadString(name, sizeof(name));
        int8_t status = 0;
        inputStream.ReadInt8(status);
        uint32_t ip = 0;
        inputStream.ReadUint32(ip);
        uint16_t port = 0;
        inputStream.ReadUint16(port);
        

        NS_MZ_NET::Address address;
        address.m_ip = ip;
        address.m_port = port;

        ServerGroup serverGroup;
        serverGroup.SetId(id);
        serverGroup.SetStatus(status);
        serverGroup.SetName(name);
        serverGroup.SetPublicAddress(address);
        ServerGroupManager::Instance().AddServerGroup(serverGroup);
    }

    UIMsgDispatcher::Instance().Notify(UIMSG_SELECTSERVER_SERVERLIST
        , reinterpret_cast<uint64_t>(data), length);
}