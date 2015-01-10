#include "UIMsgDispatcher.hpp"

UIMsgDispatcher::UIMsgDispatcher() {}

UIMsgDispatcher::~UIMsgDispatcher() {}

UIMsgDispatcher& UIMsgDispatcher::Instance() {
    static UIMsgDispatcher s_msgDispatcher;
    return s_msgDispatcher;
}

void UIMsgDispatcher::Register(int msg, MsgHandler_t msgHandler) {
    auto& msgHandlerList = m_msgHandlerMap[msg];
    msgHandlerList.push_back(msgHandler);
}

void UIMsgDispatcher::Notify(int msg, uint64_t param1, int64_t param2) {
    auto& msgHandlerList = m_msgHandlerMap[msg];
    for (auto msgHandler : msgHandlerList) {
        msgHandler(msg, param1, param2);
    }
}