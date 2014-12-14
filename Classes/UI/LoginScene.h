#ifndef _CLIENT_LOGINSCENE_H_
#define _CLIENT_LOGINSCENE_H_

#include <cocos2d.h>
#include <ui/UIButton.h>
#include <ui/UITextField.h>

class Login : public cocos2d::Layer {
    CREATE_FUNC(Login);

public:
    Login();

    ~Login();

    static cocos2d::Scene* createScene();

public:
    bool init() override final;

private:
    void initUI();

    void initUIMsgHandler();

    void loginButtonCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

private:
    cocos2d::ui::TextField* m_username;
    cocos2d::ui::TextField* m_password;
    cocos2d::ui::Button* m_login;
};

#endif // _CLIENT_LOGINSCENE_H_
