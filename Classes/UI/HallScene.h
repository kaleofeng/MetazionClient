#ifndef _CLIENT_HALLSCENE_H_
#define _CLIENT_HALLSCENE_H_

#include <cocos2d.h>
#include <ui/UIButton.h>
#include <ui/UITextField.h>

class Hall : public cocos2d::Layer {
    CREATE_FUNC(Hall);

public:
    Hall();

    ~Hall();

    static cocos2d::Scene* createScene();

public:
    bool init() override final;

private:
    void initUI();

    void initUIMsgHandler();

    void houseButtonCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void shopButtonCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void treeButtonCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

private:
    cocos2d::ui::Button* m_btHouse;
    cocos2d::ui::Button* m_btShop;
    cocos2d::ui::Button* m_btTree;
};

#endif // _CLIENT_HALLSCENE_H_
