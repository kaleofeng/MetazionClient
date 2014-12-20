#include "LoginScene.h"

#include <editor-support/cocostudio/ActionTimeline/CSLoader.h>
#include <editor-support/cocostudio/ActionTimeline/CCActionTimeline.h>
#include <ui/UIHelper.h>

#include <Metazion/Share/Utility/StringFunction.hpp>

#include <Common/Packet/PacketCL.hpp>
#include <Common/Packet/PacketLC.hpp>

#include "AppClient.h"
#include "UI/SelectServerScene.h"
#include "UI/UIMsgDispatcher.h"

USING_NS_CC;

Login::Login()
    : m_tfUsername(nullptr)
    , m_tfPassword(nullptr)
    , m_btLogin(nullptr) {}

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

    auto tfUsernameNode = ui::Helper::seekNodeByName(rootNode, "TextField_Username");
    m_tfUsername = static_cast<ui::TextField*>(tfUsernameNode);
    m_tfUsername->setFocused(true);

    auto tfPasswordNode = ui::Helper::seekNodeByName(rootNode, "TextField_Password");
    m_tfPassword = static_cast<ui::TextField*>(tfPasswordNode);

    auto btLoginNode = ui::Helper::seekNodeByName(rootNode, "Button_Login");
    m_btLogin = static_cast<ui::Button*>(btLoginNode);
    m_btLogin->addTouchEventListener(CC_CALLBACK_2(Login::loginButtonCallback, this));
}

void Login::initUIMsgHandler() {
    UIMsgDispatcher::Instance().Register(UIMSG_LOGIN_RSP
        , [this](int msg, uint64_t param1, int64_t param2) {
        const auto rsp = reinterpret_cast<const PlayerLoginLC*>(param1);

        m_btLogin->setBright(true);
        m_btLogin->setTouchEnabled(true);

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

    const auto& username = m_tfUsername->getString();
    const auto& password = m_tfPassword->getString();

    PlayerLoginCL req;
    NS_MZ_SHARE::mzstrcpy(req.m_username, sizeof(req.m_username), username.c_str());
    NS_MZ_SHARE::mzstrcpy(req.m_password, sizeof(req.m_password), password.c_str());
    g_appClient->m_socketCL->SendData(req.COMMAND, &req, sizeof(req));

    m_btLogin->setBright(false);
    m_btLogin->setTouchEnabled(false);
    m_tfUsername->setString("");
    m_tfPassword->setString("");
}