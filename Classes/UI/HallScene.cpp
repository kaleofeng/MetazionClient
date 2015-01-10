#include "HallScene.hpp"

#include <editor-support/cocostudio/ActionTimeline/CSLoader.h>
#include <editor-support/cocostudio/ActionTimeline/CCActionTimeline.h>
#include <ui/UIHelper.h>

#include <Metazion/Share/Utility/StringFunction.hpp>

#include <Common/Packet/PacketCL.hpp>
#include <Common/Packet/PacketLC.hpp>

#include "AppClient.hpp"
#include "UI/SelectServerScene.hpp"
#include "UI/UIMsgDispatcher.hpp"

USING_NS_CC;

Hall::Hall()
    : m_btHouse(nullptr)
    , m_btShop(nullptr)
    , m_btTree(nullptr) {}

Hall::~Hall() {}

Scene* Hall::createScene() {
    auto scene = Scene::create();
    auto layer = Hall::create();
    scene->addChild(layer);
    return scene;
}

bool Hall::init() {
    if (!Layer::init())  {
        return false;
    }

    initUI();
    initUIMsgHandler();
    return true;
}

void Hall::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::createNode("HallScene.csb");
    this->addChild(rootNode);

    auto action = CSLoader::createTimeline("HallScene.csb");
    rootNode->runAction(action);
    action->gotoFrameAndPlay(0, 60, true);

    auto btHouseNode = ui::Helper::seekNodeByName(rootNode, "Button_House");
    m_btHouse = static_cast<ui::Button*>(btHouseNode);
    m_btHouse->addTouchEventListener(CC_CALLBACK_2(Hall::houseButtonCallback, this));

    auto btShopNode = ui::Helper::seekNodeByName(rootNode, "Button_Shop");
    m_btShop = static_cast<ui::Button*>(btShopNode);
    m_btShop->addTouchEventListener(CC_CALLBACK_2(Hall::shopButtonCallback, this));

    auto btTreeNode = ui::Helper::seekNodeByName(rootNode, "Button_Tree");
    m_btTree = static_cast<ui::Button*>(btTreeNode);
    m_btTree->addTouchEventListener(CC_CALLBACK_2(Hall::treeButtonCallback, this));
}

void Hall::initUIMsgHandler() {

}

void Hall::houseButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

}

void Hall::shopButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

}

void Hall::treeButtonCallback(Ref* sender, ui::Widget::TouchEventType type) {
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }

}