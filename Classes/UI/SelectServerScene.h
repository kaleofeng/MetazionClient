#ifndef _CLIENT_SELECTSERVERSCENE_H_
#define _CLIENT_SELECTSERVERSCENE_H_

#include <cocos2d.h>

#include <ui/UIButton.h>
#include <ui/UICheckBox.h>
#include <ui/UIListView.h>

class SelectServer : public cocos2d::Layer {
    CREATE_FUNC(SelectServer);

public:
    SelectServer();

    ~SelectServer();

    static cocos2d::Scene* createScene();

public:
    bool init() override final;

private:
    void initUI();

    void initUIMsgHandler();

    void leftButtonCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void enterButtonCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

private:
    cocos2d::ui::ListView* m_lvLeft;
    cocos2d::ui::ListView* m_lvRight;
    cocos2d::ui::Button* m_btEnter;
};

#endif // _CLIENT_SELECTSERVERSCENE_H_
