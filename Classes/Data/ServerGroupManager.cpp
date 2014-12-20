#include "ServerGroupManager.hpp"

#include <Common/Xml/rapidxml.hpp>
#include <Common/Xml/rapidxml_utils.hpp>

ServerGroupManager::ServerGroupManager() {}

ServerGroupManager::~ServerGroupManager() {}

ServerGroupManager& ServerGroupManager::Instance() {
    static ServerGroupManager s_instance;
    return s_instance;
}

void ServerGroupManager::Initialize() {}

void ServerGroupManager::Finalize() {}

int ServerGroupManager::GetServerGroupSize() const {
    return m_serverGroupArray.GetSize();
}

ServerGroup* ServerGroupManager::GetServerGroup(int index) {
    auto size = m_serverGroupArray.GetSize();
    if (index < 0 || index >= size) {
        return nullptr;
    }

    return &m_serverGroupArray[index];
}

void ServerGroupManager::AddServerGroup(const ServerGroup& serverGroup) {
    m_serverGroupArray.Append(serverGroup);
}

void ServerGroupManager::RemoveAllServerGroup() {
    m_serverGroupArray.Clear();
}