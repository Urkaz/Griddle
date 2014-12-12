#include "TutorialScene.h"
#include "MainMenuScene.h"

USING_NS_CC;


Scene* TutorialScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TutorialScene::create();
    scene->addChild(layer);
    return scene;
}

bool TutorialScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Creating menu
    
 
    auto pItem = MenuItemImage::create("boton_pausa.png", "boton_pausa.png" ,CC_CALLBACK_1(TutorialScene::goToMainMenu, this));
    
    
    
    auto menu = Menu::create(pItem, NULL);
    
    //menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
    this->addChild(menu, 1);
    
    // Adding background
    auto background = Sprite::create("Background.png");
    
    background->setPosition(Point((visibleSize.width  /2),
                                  (visibleSize.height /2)));
    
    addChild(background, 0);
     
    
    // Sounds
    /*
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonClick.wav");
     if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
     CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/Music.mp3");
     CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/Music.mp3", true);
     }*/
    
    return true;
}

void TutorialScene::goToMainMenu(Ref *pSender) {
    
    //Constant::GAMEMODE = GameMode::NORMAL;
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");
    
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5,scene));
}
