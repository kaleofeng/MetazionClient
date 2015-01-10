#include "Sockets.hpp"

#include "AppClient.hpp"

#include "Common/Packet/PacketLC.hpp"
#include "Common/Packet/PacketGC.hpp"

void ClientSocketCL::OnConnected() {
    ::printf("ClientSocket to Login Connected\n");
    PostData(COMMAND_LC_CONNECTED, nullptr, 0);
}

void ClientSocketCL::OnDisconnected() {
    ::printf("ClientSocket to Login Disconnected\n");
    PostData(COMMAND_LC_DISCONNECTED, nullptr, 0);
}

void ClientSocketCL::OnConnectFailed() {
    ::printf("ClientSocket to Login Connect Failed\n");
    PostData(COMMAND_LC_CONNECTFAILED, nullptr, 0);
}

void ClientSocketCL::OnValidPacket(int command, const void* data, int length) {
    g_appClient->m_packetHandlerLC.Handle(command, data, length);
}

void ClientSocketCL::OnInvalidPacket() {
    Close();
}


void ClientSocketCG::OnConnected() {
    ::printf("ClientSocket to Gateway Connected\n");
    PostData(COMMAND_GC_CONNECTED, nullptr, 0);
}

void ClientSocketCG::OnDisconnected() {
    ::printf("ClientSocket to Gateway Disconnected\n");
    PostData(COMMAND_GC_DISCONNECTED, nullptr, 0);
}

void ClientSocketCG::OnConnectFailed() {
    ::printf("ClientSocket to Gateway Connect Failed\n");
    PostData(COMMAND_GC_CONNECTFAILED, nullptr, 0);
}

void ClientSocketCG::OnValidPacket(int command, const void* data, int length) {
    g_appClient->m_packetHandlerGC.Handle(command, data, length);
}

void ClientSocketCG::OnInvalidPacket() {
    Close();
}