
#include "PauseScene.h"


USING_NS_CC;


Scene* PauseScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PauseScene::create();
    scene->addChild(layer);
    return scene;
}

bool PauseScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Creating menu
    auto playItem = MenuItemImage::create("boton_continuar.png",
                                          "boton_continuar_pulsado.png",
                                          CC_CALLBACK_1(PauseScene::goToPicrossGameScene, this));
    
    auto playItem2 = MenuItemImage::create("boton_salir.png",
                                           "boton_salir_pulsado.png",
                                           CC_CALLBACK_1(PauseScene::goToPicrossSelector, this));
    
    auto menu = Menu::create(playItem, playItem2 , NULL);
    
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
    this->addChild(menu, 1);
    
    // Adding background
    auto background = Sprite::create("Background.png");
    
    background->setPosition(Point((visibleSize.width  /2),
                                  (visibleSize.height /2)));
    
    addChild(background, 0);
    
    
    
    return true;
     
}



void PauseScene::goToPicrossGameScene(Ref *pSender) {
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");
    Director::getInstance()->popScene();
}

void PauseScene::goToPicrossSelector(Ref *pSender) {
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");
    Director::getInstance()->popScene();
    Director::getInstance()->popScene();

}
