#include "AppClient.hpp"

#include <Metazion/Net/Network.hpp>

#include "Data/ServerGroupManager.hpp"
#include "Data/StringManager.hpp"

USING_NAMESPACE_MZ_NET

AppClient* g_appClient = nullptr;

AppClient::AppClient()
    : m_connected(false) {}

AppClient::~AppClient() {}

bool AppClient::init() {
    StringManager::Instance().Initialize();
    ServerGroupManager::Instance().Initialize();

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

    ServerGroupManager::Instance().Finalize();
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

void AppClient::connectToLogin(const NS_MZ_NET::Host& host) {
    m_socketCL = new ClientSocketCL();
    m_socketCL->Retain();
    m_socketCL->SetRemoteHost(host);
    m_socketCL->SetReconnectInterval(10000);
    m_socketCL->Connect();
    m_networkService.Attach(m_socketCL);
}

void AppClient::connectToGateway(const NS_MZ_NET::Host& host) {
    m_socketCG = new ClientSocketCG();
    m_socketCG->Retain();
    m_socketCG->SetRemoteHost(host);
    m_socketCG->SetReconnectInterval(10000);
    m_socketCG->Connect();
    m_networkService.Attach(m_socketCG);
}

bool AppClient::hasConnected() {
    return m_connected;
}

void AppClient::SetConnected(bool connected) {
    m_connected = connected;
}

void AppClient::initNetworkService() {
    Network::Startup();

    m_networkService.Initialize(4, 2);

    ::memset(m_sockets, 0, sizeof(m_sockets));
    m_socketArray.Attach(m_sockets, 64, 0);

    Host host;
    host.SetFamily(AF_INET);
    host.SetIp("192.168.1.101");
    host.SetPort(21010);
    connectToLogin(host);
}

void AppClient::clearNetworkService() {
    m_socketArray.Detach();
    m_networkService.Finalize();
}