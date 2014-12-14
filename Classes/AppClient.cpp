#include "AppClient.h"

#include <Metazion/Net/Network.hpp>

#include "StringManager.h"

USING_NAMESPACE_MZ_NET

AppClient* g_appClient = nullptr;

AppClient::AppClient() {}

AppClient::~AppClient() {}

bool AppClient::init() {
    StringManager::Instance().Initialize();

    initNetworkService();

    onEnter();
    onEnterTransitionDidFinish();
    scheduleUpdate();
    return true;
}

void AppClient::clear() {
    onExit();
    unscheduleUpdate();

    clearNetworkService();

    StringManager::Instance().Finalize();
    Network::Cleanup();
}

void AppClient::update(float dt) {
    const auto size = m_networkService.LockSockets([](Socket* socket) {
            return socket->GetType() != SOCKET_TCP_LISTEN;
        }, m_socketArray);

    for (auto i = 0; i < size; ++i) {
        auto socket = m_socketArray[i];
        MZ_ASSERT_TRUE(!NS_MZ::IsNull(socket));
        socket->Dispatch();
    }

    m_networkService.UnlockSockets(m_socketArray);
}

void AppClient::initNetworkService() {
    Network::Startup();

    m_networkService.Initialize(4, 2);

    ::memset(m_sockets, 0, sizeof(m_sockets));
    m_socketArray.Attach(m_sockets, 64, 0);

    Host hostCL;
    hostCL.SetFamily(AF_INET);
    hostCL.SetIp("192.168.1.101");
    hostCL.SetPort(21010);

    m_socketCL = new ClientSocketCL();
    m_socketCL->Retain();
    m_socketCL->SetRemoteHost(hostCL);
    m_socketCL->SetReconnectInterval(10000);
    m_socketCL->Connect();
    m_networkService.Attach(m_socketCL);
}

void AppClient::clearNetworkService() {
    m_socketArray.Detach();
    m_networkService.Finalize();
}