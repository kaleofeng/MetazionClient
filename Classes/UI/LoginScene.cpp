#include "LoginScene.h"

#include <editor-support/cocostudio/ActionTimeline/CSLoader.h>
#include <editor-support/cocostudio/ActionTimeline/CCActionTimeline.h>
#include <ui/UIHelper.h>

#include <Metazion/Share/Utility/StringFunction.hpp>

#include <Common/Packet/PacketCL.hpp>
#include <Common/Packet/PacketLC.hpp>

#include "AppClient.h"
#include "UI/UIMsgDispatcher.h"
#include "UI/SelectServerScene.h"

USING_NS_CC;

Login::Login()
    : m_username(nullptr)
    , m_password(nullptr)
    , m_login(nullptr) {}

Login::~Login() {}

Scene* Login::createScene() {
    auto scene = Scene::create();
    auto layer = Login::create();
    scene->addChild(layer);
    return scene;
}

bool Login::init() {
    if (!Layer::init())  {
        return false;
    }

    initUI();
    initUIMsgHandler();
    return true;
}

void Login::initUI() {

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::createNode("LoginScene.csb");
    this->addChild(rootNode);

    auto action = CSLoader::createTimeline("LoginScene.csb");
    rootNode->runAction(action);
    action->gotoFrameAndPlay(0, 60, true);

    auto usernameNode = ui::Helper::seekNodeByName(rootNode, "TextField_Username");
    m_username = static_cast<ui::TextField*>(usernameNode);
    m_username->setFocused(true);

    auto passwordNode = ui::Helper::seekNodeByName(rootNode, "TextField_Password");
    m_password = static_cast<ui::TextField*>(passwordNode);

    auto loginNode = ui::Helper::seekNodeByName(rootNode, "Button_Login");
    m_login = static_cast<ui::Button*>(loginNode);
    m_login->addTouchEventListener(CC_CALLBACK_2(Login::loginButtonCallback, this));
}

void Login::initUIMsgHandler() {
    UIMsgDispatcher::Instance().Register(UIMSG_LOGIN_RSP
        , [this](int msg, uint64_t param1, int64_t param2) {
        const auto rsp = reinterpret_cast<const PlayerLoginLC*>(param1);

        m_login->setBright(true);
        m_login->setTouchEnabled(true);

        if (rsp->m_success) {
            auto selectServerScene = SelectServer::createScene();
            Director::getInstance()->replaceScene(selectServerScene);
        }
    });
}

void Login::loginButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

    if (!g_appClient->hasConnected()) {
        return;
    }

    const auto& username = m_username->getString();
    const auto& password = m_password->getString();

    PlayerLoginCL req;
    NS_MZ_SHARE::mzstrcpy(req.m_username, sizeof(req.m_username), username.c_str());
    NS_MZ_SHARE::mzstrcpy(req.m_password, sizeof(req.m_password), password.c_str());
    g_appClient->m_socketCL->SendData(req.COMMAND, &req, sizeof(req));

    m_login->setBright(false);
    m_login->setTouchEnabled(false);
    m_username->setString("");
    m_password->setString("");
}