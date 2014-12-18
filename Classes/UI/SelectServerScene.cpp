#include "SelectServerScene.h"

#include <editor-support/cocostudio/ActionTimeline/CSLoader.h>
#include <editor-support/cocostudio/ActionTimeline/CCActionTimeline.h>
#include <ui/UIHelper.h>

#include <Metazion/Share/Misc/MemoryInputStream.hpp>

#include "StringManager.h"
#include "UI/UIMsgDispatcher.h"

USING_NS_CC;

SelectServer::SelectServer() {}

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

    InitUI();
    InitUIMsgHandler();
    return true;
}

void SelectServer::InitUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::createNode("SelectServerScene.csb");
    this->addChild(rootNode);

    auto action = CSLoader::createTimeline("SelectServerScene.csb");
    rootNode->runAction(action);
    action->gotoFrameAndPlay(0, 60, true);

    auto listViewLeftNode = ui::Helper::seekNodeByName(rootNode, "ListView_Left");
    m_listViewLeft = static_cast<ui::ListView*>(listViewLeftNode);
}

void SelectServer::InitUIMsgHandler() {
    UIMsgDispatcher::Instance().Register(UIMSG_SELECTSERVER_SERVERLIST
        , [this](int msg, uint64_t param1, int64_t param2) {
        const auto data = reinterpret_cast<const char*>(param1);
        const auto length = static_cast<int>(param2);

        NS_MZ_SHARE::MemoryInputStream inputStream;
        inputStream.Attach(data, length);

        int8_t servverGroupSize = 0;
        inputStream.ReadInt8(servverGroupSize);

        for (auto index = 0; index < servverGroupSize; ++index) {
            int8_t id = 0;
            inputStream.ReadInt8(id);
            char name[32] = { '\0' };
            inputStream.ReadString(name, sizeof(name));
            uint32_t ip = 0;
            inputStream.ReadUint32(ip);
            uint16_t port = 0;
            inputStream.ReadUint16(port);
            int8_t status = 0;
            inputStream.ReadInt8(status);

            char text[64] = "";
            const std::string& format = StringManager::Instance().GetString(1001);
            snprintf(text, sizeof(text), format.c_str(), name, status);

            auto button = ui::Button::create();
            button->loadTextureNormal("Images/SelectServer/LeftNormal.png");
            button->loadTexturePressed("Images/SelectServer/LeftHighlight.png");
            button->loadTextureDisabled("Images/SelectServer/LeftHighlight.png");
            button->setTitleFontName("Fonts/MS_YaHei_14M.ttf");
            button->setTitleFontSize(16);
            button->setTitleText(text);
            button->addTouchEventListener(CC_CALLBACK_2(SelectServer::leftButtonCallback, this));

            m_listViewLeft->pushBackCustomItem(button);
        }
    });
}

void SelectServer::leftButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

    auto button = static_cast<ui::Button*>(sender);
    button->setBright(false);
    button->setTouchEnabled(false);

    auto& buttons = m_listViewLeft->getItems();
    for (auto eachButton : buttons) {
        if (eachButton != button) {
            eachButton->setBright(true);
            eachButton->setTouchEnabled(true);
        }
    }

    const auto curIndex = m_listViewLeft->getCurSelectedIndex();
    auto j = curIndex;
}