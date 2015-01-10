#ifndef _CLIENT_SERVERGROUPMANAGER_HPP_
#define _CLIENT_SERVERGROUPMANAGER_HPP_

#include <Metazion/Share/Collection/DynamicArray.hpp>

#include "ServerGroup.hpp"

class ServerGroupManager {
public:
    using ServerGroupArray_t = NS_MZ_SHARE::DynamicArray<ServerGroup>;

public:
    ServerGroupManager();

    ~ServerGroupManager();

    static ServerGroupManager& Instance();

public:
    void Initialize();

    void Finalize();

    int GetServerGroupSize() const;
    ServerGroup* GetServerGroup(int index);
    void AddServerGroup(const ServerGroup& gatewayConfig);
    void RemoveAllServerGroup();

private:
    ServerGroupArray_t m_serverGroupArray;
};

#endif // _CLIENT_SERVERGROUPMANAGER_HPP_
