/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainCrashyScene.h"
#include "SimpleAudioEngine.h"
#include <BugsnagCocos2dx/Bugsnag.hpp>

USING_NS_CC;

Scene* CrashyScene::createScene()
{
    return CrashyScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool CrashyScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = DrawNode::create();
    background->drawSolidRect(origin, visibleSize, Color4F(0.9,0.9,0.9,1.0));
    this->addChild(background);

    auto crashItem = Sprite::create("Crash.png");
    float x = origin.x + (visibleSize.width)/2;
    float y = origin.y + (11 * visibleSize.height/12);
    crashItem->setPosition(Vec2(x,y));
    this->addChild(crashItem);

    auto notifyItem = Sprite::create("Notify.png");
    y -= visibleSize.height/12;
    notifyItem->setPosition(Vec2(x,y));
    this->addChild(notifyItem);

    auto diagnosticsItem = Sprite::create("Diagnostics.png");
    y -= visibleSize.height/12;
    diagnosticsItem->setPosition(Vec2(x,y));
    this->addChild(diagnosticsItem);

    auto breadcrumbItem = Sprite::create("Breadcrumb.png");
    y -= visibleSize.height/12;
    breadcrumbItem->setPosition(Vec2(x,y));
    this->addChild(breadcrumbItem);

    auto userItem = Sprite::create("User.png");
    y -= visibleSize.height/12;
    userItem->setPosition(Vec2(x,y));
    this->addChild(userItem);
    
    auto pauseSessionItem = Sprite::create("PauseSession.png");
    y -= visibleSize.height/12;
    pauseSessionItem->setPosition(Vec2(x,y));
    this->addChild(pauseSessionItem);
    
    auto resumeSessionItem = Sprite::create("ResumeSession.png");
    y -= visibleSize.height/12;
    resumeSessionItem->setPosition(Vec2(x,y));
    this->addChild(resumeSessionItem);
    
    auto startSessionItem = Sprite::create("StartSession.png");
    y -= visibleSize.height/12;
    startSessionItem->setPosition(Vec2(x,y));
    this->addChild(startSessionItem);

    auto triggerANRItem = Sprite::create("TriggerANR.png");
    y -= visibleSize.height/12;
    triggerANRItem->setPosition(Vec2(x,y));
    this->addChild(triggerANRItem);

    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = [this, crashItem, notifyItem, diagnosticsItem, breadcrumbItem, userItem, pauseSessionItem, startSessionItem, resumeSessionItem, triggerANRItem](Touch* touch, Event* event)
    {
        if (crashItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a crash");
            this->crashEvent();
        } else if (notifyItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a notify");
            this->notifyEvent();
        } else if (diagnosticsItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a diagnostics");
            this->attachCustomDiagnostics();
        } else if (breadcrumbItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a breadcrumb");
            this->attachManualBreadcrumb();
        } else if (userItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a user");
            this->createUser();
        } else if (pauseSessionItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a pauseSession");
            this->pauseSession();
        } else if (resumeSessionItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a resumeSession");
            this->resumeSession();
        } else if (startSessionItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a startSession");
            this->startSession();
        } else if (triggerANRItem->getBoundingBox().containsPoint(touch->getLocation())) {
            CCLOG("Doing a ANR");
            this->triggerANR();
        }
        return false;
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

    return true;
}

int CrashyScene::crashyFunction(int* value)
{
    if (value == 0)
    {
        throw std::invalid_argument("Requires a non-zero!");
    }
    return 1;
}

void CrashyScene::crashEvent()
{
    this->crashyFunction(0);
}

void CrashyScene::notifyEvent()
{
    try {
        this->crashyFunction(0);
    } catch (std::invalid_argument& e) {
        bugsnag::Bugsnag::notify(e);
    }
}

void CrashyScene::attachCustomDiagnostics()
{
    bugsnag::Bugsnag::addMetadata("Foo", "Bar", "FooBar");
}

void CrashyScene::attachManualBreadcrumb()
{
    bugsnag::Bugsnag::leaveBreadcrumb("Attaching manual breadcrumb", bugsnag::BreadcrumbType::Navigation,
        {{"Foo", "foo"}, {"Bar", "bar"}});
}

void CrashyScene::createUser()
{
    bugsnag::Bugsnag::setUser("User_id", "User@Usernet.net", "Anonymoose");
}

void CrashyScene::pauseSession()
{
    bugsnag::Bugsnag::pauseSession();
}

void CrashyScene::startSession()
{
    bugsnag::Bugsnag::startSession();
}

void CrashyScene::resumeSession()
{
    bugsnag::Bugsnag::resumeSession();
}

void CrashyScene::triggerANR()
{
    while (true) {}
}

void CrashyScene::triggerOOM()
{
    std::string inString("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    std::string outString("");
    while (true)
    {
        outString += outString + inString;
    }
}
