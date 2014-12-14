#ifndef _CLIENT_MSGDISPATCHER_H_
#define _CLIENT_MSGDISPATCHER_H_

#include <cstdint>
#include <list>
#include <map>
#include <functional>

#include "UI/UIMsg.h"

class UIMsgDispatcher {
    using MsgHandler_t = std::function<void(int, uint64_t, int64_t)>;
    using MsgHandlerList_t = std::list<MsgHandler_t>;
    using MsgHandlerMap_t = std::map<int, MsgHandlerList_t>;

private:
    UIMsgDispatcher();

    ~UIMsgDispatcher();

public:
    static UIMsgDispatcher& Instance();

public:
    void Register(int msg, MsgHandler_t msgHandler);

    void Notify(int msg, uint64_t param1, int64_t param2);

private:
    MsgHandlerMap_t m_msgHandlerMap;

};

#endif // _CLIENT_MSGDISPATCHER_H_
