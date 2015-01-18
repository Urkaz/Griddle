#pragma once

#include "cocos2d.h"
#include "Picross.h"
#include "Global.h"

class TutorialScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(TutorialScene);
    
    // Added
    void goToMainMenu(Ref *pSender);

private:
	int indice;

	void Siguiente(Ref *pSender);
	void Anterior(Ref *pSender);
	void ActualizarTextura(int index);

	void TexturaSiguiente(Ref *pSender);
	void TexturaAnterior(Ref *pSender);

};