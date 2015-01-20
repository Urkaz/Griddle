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
    auto playItem = MenuItemImage::create("continuar.png",
                                          "continuarpulsado.png",
                                          CC_CALLBACK_1(PauseScene::goToPicrossGameScene, this));
    
    auto playItem2 = MenuItemImage::create("salirs.png",
                                           "salir.png",
                                           CC_CALLBACK_1(PauseScene::goToPicrossSelector, this));
    auto playItem3= MenuItemImage::create("pausado.png", "pausado.png");
    
    playItem->setScale(0.2f);
    playItem2->setScale(0.2f);
    playItem3->setScale(0.2f);
    
    auto menu = Menu::create(playItem3,playItem, playItem2 , NULL);
    
    menu->alignItemsVerticallyWithPadding(visibleSize.height / 15);
    this->addChild(menu, 1);
    
    // Adding background
    auto background = Sprite::create("fondo_prueba2.png");
    
    background->setPosition(Point((visibleSize.width  /2),
                                  (visibleSize.height /2)));
    
    addChild(background, 0);

    return true;
     
}



void PauseScene::goToPicrossGameScene(Ref *pSender) {
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	Director::getInstance()->popScene();

	/*
	ESTO NO OS FUNCIONARÁ, getPreviousScene ES UNA FUNCIÓN NUEVA EN EL Director para obtener la escena anterior en la pila de escenas y poderla cambiar con una transicion.
	Scene * nextScene = Director::getInstance()->getPreviousScene();
	Scene * fadeScene = TransitionFade::create(1.0, nextScene);
	Director::getInstance()->replaceScene(fadeScene);*/
}

void PauseScene::goToPicrossSelector(Ref *pSender) {
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");
    Director::getInstance()->popScene();
    Director::getInstance()->popScene();
}
