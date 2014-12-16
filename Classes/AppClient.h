#ifndef _CLIENT_APPCLIENT_
#define _CLIENT_APPCLIENT_

#include <2d/CCNode.h>

#include <Metazion/Net/AppNetworkService.hpp>

#include "Net/PacketHandlerLC.hpp"
#include "Net/Sockets.hpp"

class AppClient : public cocos2d::Node {
public:
    AppClient();

    ~AppClient();

public:
    bool init();

    void clear();

    void update(float dt);

    bool hasConnected();

    void SetConnected(bool connected);

private:
    void initNetworkService();

    void clearNetworkService();

public:
    ClientSocketCL* m_socketCL;
    ClientSocketCG* m_socketCG;

    PacketHandlerLC m_packetHandlerLC;
    PacketHandlerLC m_packetHandlerGC;

private:
    NS_MZ_NET::AppNetworkService m_networkService;
    NS_MZ_NET::AppNetworkService::SocketArray_t m_socketArray;
    NS_MZ_NET::Socket* m_sockets[64];

    bool m_connected;
};

extern AppClient* g_appClient;

#endif // _CLIENT_APPCLIENT_
