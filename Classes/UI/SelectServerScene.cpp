#include "SelectServerScene.hpp"

#include <editor-support/cocostudio/ActionTimeline/CSLoader.h>
#include <editor-support/cocostudio/ActionTimeline/CCActionTimeline.h>
#include <ui/UIHelper.h>

#include <Common/Packet/PacketCL.hpp>
#include <Common/Packet/PacketLC.hpp>

#include "Data/ServerGroupManager.hpp"
#include "Data/StringManager.hpp"
#include "Data/User.hpp"
#include "UI/HallScene.hpp"
#include "UI/UIMsgDispatcher.hpp"
#include "AppClient.hpp"

USING_NS_CC;

SelectServer::SelectServer()
    : m_lvLeft(nullptr)
    , m_lvRight(nullptr)
    , m_btEnter(nullptr) {}

SelectServer::~SelectServer() {}

Scene* SelectServer::createScene() {
    auto scene = Scene::create();
    auto layer = SelectServer::create();
    scene->addChild(layer);
    return scene;
}

bool SelectServer::init() {
    if (!Layer::init())  {
        return false;
    }

    initUI();
    initUIMsgHandler();
    return true;
}

void SelectServer::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::createNode("SelectServerScene.csb");
    this->addChild(rootNode);

    auto action = CSLoader::createTimeline("SelectServerScene.csb");
    rootNode->runAction(action);
    action->gotoFrameAndPlay(0, 60, true);

    auto lvLeftNode = ui::Helper::seekNodeByName(rootNode, "ListView_Left");
    m_lvLeft = static_cast<ui::ListView*>(lvLeftNode);

    auto btEnterNode = ui::Helper::seekNodeByName(rootNode, "Button_Enter");
    m_btEnter = static_cast<ui::Button*>(btEnterNode);
    m_btEnter->addTouchEventListener(CC_CALLBACK_2(SelectServer::enterButtonCallback, this));
}

void SelectServer::initUIMsgHandler() {
    UIMsgDispatcher::Instance().Register(UIMSG_SELECTSERVER_SERVERLIST
        , [this](int msg, uint64_t param1, int64_t param2) {
        const auto serverGroupSize = ServerGroupManager::Instance().GetServerGroupSize();
        for (auto index = 0; index < serverGroupSize; ++index) {
            auto serverGroup = ServerGroupManager::Instance().GetServerGroup(index);
            MZ_ASSERT_TRUE(!NS_MZ::IsNull(serverGroup));

            char text[64] = "";
            const std::string& format = StringManager::Instance().GetString(1001);
            snprintf(text, sizeof(text), format.c_str()
                , serverGroup->GetName(), serverGroup->GetStatus());

            auto button = ui::Button::create();
            button->loadTextureNormal("Images/SelectServer/LeftNormal.png");
            button->loadTexturePressed("Images/SelectServer/LeftHighlight.png");
            button->loadTextureDisabled("Images/SelectServer/LeftHighlight.png");
            button->setTitleFontName("Fonts/MS_YaHei_14M.ttf");
            button->setTitleFontSize(16);
            button->setTitleText(text);
            button->addTouchEventListener(CC_CALLBACK_2(SelectServer::leftButtonCallback, this));

            m_lvLeft->pushBackCustomItem(button);
        }
    });

    UIMsgDispatcher::Instance().Register(UIMSG_SELECTSERVER_RSP
        , [this](int msg, uint64_t param1, int64_t param2) {
        const auto rsp = reinterpret_cast<const SelectServerLC*>(param1);

        m_btEnter->setBright(true);
        m_btEnter->setTouchEnabled(true);

        if (rsp->m_authCode <= 0) {
            return;
        }

        User::Instance().SetAuthCode(rsp->m_authCode);

        g_appClient->m_socketCL->SetReconnectInterval(-1);
        g_appClient->m_socketCL->Close();

        NS_MZ_NET::Host host;
        host.FromAddress(rsp->m_address);
        g_appClient->connectToGateway(host);

        auto hallScene = Hall::createScene();
        Director::getInstance()->replaceScene(hallScene);
    });
}

void SelectServer::leftButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

    auto button = static_cast<ui::Button*>(sender);
    button->setBright(false);
    button->setTouchEnabled(false);

    auto& buttons = m_lvLeft->getItems();
    for (auto eachButton : buttons) {
        if (eachButton != button) {
            eachButton->setBright(true);
            eachButton->setTouchEnabled(true);
        }
    }
}

void SelectServer::enterButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

    const auto index = m_lvLeft->getCurSelectedIndex();

    auto serverGroup = ServerGroupManager::Instance().GetServerGroup(index);
    MZ_ASSERT_TRUE(!NS_MZ::IsNull(serverGroup));

    SelectServerCL req;
    req.m_userId = User::Instance().GetUserId();
    req.m_serverId = serverGroup->GetId();
    g_appClient->m_socketCL->SendData(req.COMMAND, &req, sizeof(req));

    m_btEnter->setBright(false);
    m_btEnter->setTouchEnabled(false);
}