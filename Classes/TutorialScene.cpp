#include "TutorialScene.h"
#include "MainMenuScene.h"

using namespace cocos2d;
using namespace std;


USING_NS_CC;


Sprite* imagen_tutorial;
Sprite* personaje;
Sprite* panel;

Texture2D::TexParams textpar;
Texture2D* imagen;
Texture2D* imagen2;
Texture2D* imagenpanel;

Texture2D* pequeño;



TTFConfig labelConfig;

Label* labelTutorial;

int indice = 1;

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

    
    Texture2D::TexParams tp = {GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    textpar = tp;
    
    //Configuración del label (cambiar tipo de letra)
 
    labelConfig.fontFilePath = "LondrinaSolid-Regular.otf";
    labelConfig.fontSize = 25;
    //labelConfig.glyphs = GlyphCollection::DYNAMIC;
    //labelConfig.customGlyphs = nullptr; //Ni idea
    //labelConfig.distanceFieldEnabled = false;
    
    
    // Creating menu
    
 
    auto botonSiguiente = MenuItemImage::create("derecha.png", "derecha.png" ,CC_CALLBACK_1(TutorialScene::TexturaSiguiente, this));
    
    auto botonAnterior = MenuItemImage::create("izquierda.png", "izquierda.png" ,CC_CALLBACK_1(TutorialScene::TexturaAnterior, this));
    
    auto botonSalir = MenuItemImage::create("x_boton.png", "x_boton.png",CC_CALLBACK_1(TutorialScene::goToMainMenu, this));
 
    botonSiguiente->setScale(0.3);
    botonAnterior->setScale(0.3);
    
    
    auto menu = Menu::create(botonAnterior, botonSiguiente, NULL);
    
    auto menusalir = Menu::create(botonSalir, NULL);
    
    menusalir->setPosition(visibleSize.width - botonSalir->getBoundingBox().size.width / 2, visibleSize.height - botonSalir->getBoundingBox().size.height / 3);
    
    menu->setPosition(visibleSize.width/1.8, visibleSize.height/15);
    
    menu->alignItemsHorizontallyWithPadding(visibleSize.height / 1.5);
    
    this->addChild(menu, 1);
    this->addChild(menusalir,1);
    
    // Adding background
    auto background = Sprite::create("fondopato.png");
    auto labelbackground = Sprite::create("labeltutorial.png");
    
    background->setScale(1.5);
    
    background->setPosition(Point((visibleSize.width  /2),
                                  (visibleSize.height /2)));
    labelbackground->setScale(1.5, 1.2);
    
    labelbackground->setPosition(Point((visibleSize.width  /2),
                                  (visibleSize.height /2)));

    addChild(background, 0);
    addChild(labelbackground, 0);

    
    //Primera imagen del tutorial
    imagen_tutorial = Sprite::create("T2.png");
    imagen = Director::getInstance()->getTextureCache()->addImage("T2.png");
    imagen->setTexParameters(textpar);
    imagen_tutorial->setTexture(imagen);
    imagen_tutorial->setPosition(Point((visibleSize.width  /2), (visibleSize.height /1.5)));
    imagen_tutorial->setScale(3);
    addChild(imagen_tutorial);
    
    panel = Sprite::create("panel.png");
    imagenpanel = Director::getInstance()->getTextureCache()->addImage("panel.png");
    imagenpanel->setTexParameters(textpar);
    panel->setTexture(imagenpanel);
    panel->setPosition(Point((visibleSize.width/2), (visibleSize.height/8)));
    panel->setScale(11);
    addChild(panel);

    personaje = Sprite::create("personaje1.png");
    imagen2 = Director::getInstance()->getTextureCache()->addImage("personaje1.png");
    imagen2->setTexParameters(textpar);
    personaje->setTexture(imagen2);
    personaje->setPosition(Point((visibleSize.width/9), (visibleSize.height/5)));
    personaje->setScale(11);
    addChild(personaje);
    
    //Label del tutorial
    labelTutorial = Label::createWithTTF(labelConfig, "\n""Bienvenido al tutorial""\n""Me llaman LaraGay, sere tu tutor");
    labelTutorial->setPosition(Point((visibleSize.width  /1.8), (visibleSize.height /5)));
    labelTutorial->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTutorial->setColor(Color3B(0, 100, 200));
    //labelTutorial->setCascadeColorEnabled(true);
    //labelTutorial->setMaxLineWidth( ); PONER?
    addChild(labelTutorial);
    
    
    // Sounds
    /*
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonClick.wav");
     if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
     CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/Music.mp3");
     CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/Music.mp3", true);
     }*/
    
    return true;
}

void TutorialScene::TexturaAnterior(Ref *pSender){
    
    log("%i ", indice);
    indice -=1;
    
    switch (indice) {
        case 2:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial2.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("¿Que es Griddle?""\n""Griddle es un juego de puzzles.""\n""Consiste en rellenar casillas en el tablero usando los números de arriba e izquierda como ayuda""\n"" para descubrir el dibujo oculto.");
            break;
        case 3:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial3.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Los números escritos en la parte superior e izquierda indican ""\n""cuántas casillas se deben rellenar para una fila o columna respectivamente.");
            break;
        case 4:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial4.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Números superiores:""\n""Número de casillas que deben ser rellenadas por Columnas");
            break;
        case 5:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial5.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Números izquierda:""\n""Número de casillas que deben ser rellenadas por Filas");
            break;
        case 6:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial6.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Por ejemplo, si aparece un 5 en los números de la izquierda, debes rellenar 5 casillas en horizontal.");
            break;
        case 7:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial7.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Una ayuda como 1 - 3 significa que tras rellenar una casilla,""\n"" deberás dejar al menos una en blanco antes de rellenar otras tres.");
            break;
            
        case 8:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial8.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Una vez rellenadas todas las casillas, se descubrirá la ilustración oculta, ""\n""siempre y cuando lo termines sin fallar o en menos de 10 minutos. (Dependiendo del modo de juego)");
            break;
        case 9:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("textolargo");
            break;
        case 10:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial9.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Vamos a resolver un Griddle como ejemplo.""\n""Primero empezaremos con las áreas que estamos seguros que podremos rellenar.""\n""En este puzzle de 5x5 casillas, empezaremos por la columna que tiene un 5.");
            break;
        case 11:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial10.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("La fila con 3 - 1, significa rellenar tres casillas, dejar al menos una en blanco,""\n"" y rellenar otra. Pondremos una marca en la casilla vacía para ayudarnos en la resolución ""\n"" y así estar seguros de esta fila.");
            break;
        case 12:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial11.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Ahora vemos la fila 2. Gracias a las casillas marcadas por la columna 5 sabemos ""\n"" que casillas no pueden ser rellenadas. ""\n""Para indicarlo, nos pondremos una marca en cada una.");
            break;
        case 13:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial12.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Con las marcas realizadas, podemos resolver la columna 3.""\n"" Y hecho esto, marcaremos las casillas restantes para asegurar la columna.");
            break;
        case 14:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial13.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("A continuación, vemos la columna 2.""\n"" Está claro que no podemos rellenar dos casillas consecutivas contando desde arriba. ""\n""Cada vez que esto ocurra, marcaremos dichas casillas.");
            break;
        case 15:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial14.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Ya que las dos casillas de arriba están marcadas, sabemos que podemos rellenar dos de las tres restantes.");
            break;
        case 16:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("En este espacio vacío, sólo hay dos formas en las cuales podemos rellenar las dos casillas, ""\n""por lo tanto rellenaremos cualquier casilla común a ambas posibilidades.");
            break;
        case 17:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Este método puede ser usado en varias situaciones. Contando el número de casillas, ""\n""podemos saber qué casillas son comunes a las varias posibilidades. ""\n""Esto incluye cuando hay más de un número por cada fila o columna.");
            break;
        case 18:
            imagen = Director::getInstance()->getTextureCache()->addImage("TutorialFIN.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Ahora, aseguramos la columna con 1, ya que está completa. Y tras hacer esto, ""\n""poco a poco iremos rellenando y marcando las demás casillas hasta completar la figura.");
            break;
        case 19:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Aquí concluye el tutorial. ""\n""Esperamos que te diviertas resolviendo puzzles, y recuerda aplicar todas las técnicas aprendidas.");
            break;
    };
}

void TutorialScene::TexturaSiguiente(Ref *pSender){
    
    log("%i ", indice);
    indice += 1;
    
    switch (indice) {
        case 2:
            imagen = Director::getInstance()->getTextureCache()->addImage("T1.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(4,1.5);
            labelTutorial->setString("¿Que es Griddle?""\n""Griddle es un juego de puzzles. ""Consiste en rellenar casillas""\n"" en el tablero"" usando los números de arriba e izquierda como ayuda""\n"" para descubrir el dibujo oculto.");
            
            break;
        case 3:
            imagen = Director::getInstance()->getTextureCache()->addImage("T2.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(3);
            labelTutorial->setString("Los números escritos en la parte superior e izquierda indican ""\n""cuántas casillas se deben rellenar para una fila o columna respectivamente.");
            
            break;
        case 4:
            imagen = Director::getInstance()->getTextureCache()->addImage("T3.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Números superiores:""\n""Número de casillas que deben ser rellenadas por Columnas");
            
            break;
        case 5:
            imagen = Director::getInstance()->getTextureCache()->addImage("T4.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Números izquierda:""\n""Número de casillas que deben ser rellenadas por Filas");
            
            break;
        case 6:
            imagen = Director::getInstance()->getTextureCache()->addImage("T5.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(5);
            labelTutorial->setString("Por ejemplo, si aparece un 5 en los números de la izquierda, ""\n""debes rellenar 5 casillas en horizontal.");
            
            break;
        case 7:
            imagen = Director::getInstance()->getTextureCache()->addImage("T6.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(5);
            labelTutorial->setString("Una ayuda como 3 - 1 significa que tras rellenar una casilla,""\n"" deberás dejar al menos una en blanco antes de rellenar otras tres.");
            break;
            
        case 8:
            imagen = Director::getInstance()->getTextureCache()->addImage("T7.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(4,3);
            labelTutorial->setString("Una vez rellenadas todas las casillas, se descubrirá la ilustración oculta, ""\n""siempre y cuando lo termines sin fallar ""\n""o en menos de 10 minutos.""\n"" (Dependiendo del modo de juego)");
            break;
        case 9:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(3);
            labelTutorial->setString("textolargo");
            break;
        case 10:
            imagen = Director::getInstance()->getTextureCache()->addImage("T9.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(5,2.5);
            labelTutorial->setString("Vamos a resolver un Griddle como ejemplo.""\n""Primero empezaremos con las áreas que estamos seguros que podremos rellenar.""\n""En este puzzle de 5x5 casillas, ""\n""empezaremos por la columna que tiene un 5.");
            break;
        case 11:
            imagen = Director::getInstance()->getTextureCache()->addImage("T10.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(3);
            labelTutorial->setString("La fila con 3 - 1, ""\n""significa rellenar tres casillas, dejar al menos una en blanco,""\n"" y rellenar otra. Pondremos una marca en la casilla vacía para ayudarnos en la resolución ""\n"" y así estar seguros de esta fila.");
            break;
        case 12:
            imagen = Director::getInstance()->getTextureCache()->addImage("T11.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Ahora vemos la fila 2. Gracias a las casillas marcadas por la columna 5 sabemos ""\n"" que casillas no pueden ser rellenadas. ""\n""Para indicarlo, nos pondremos una marca en cada una.");
            break;
        case 13:
            imagen = Director::getInstance()->getTextureCache()->addImage("T12.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Con las marcas realizadas, ""\n""podemos resolver la columna 3. Y hecho esto, ""\n""marcaremos las casillas restantes para asegurar la columna.");
            break;
        case 14:
            imagen = Director::getInstance()->getTextureCache()->addImage("Tutorial13.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("A continuación, vemos la columna 2.""\n"" Está claro que no podemos rellenar dos casillas consecutivas""\n"" contando desde arriba. Cada vez que esto ocurra,""\n"" marcaremos dichas casillas.");
            break;
        case 15:
            imagen = Director::getInstance()->getTextureCache()->addImage("T14.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Ya que las dos casillas de arriba están marcadas,""\n"" sabemos que podemos rellenar dos de las tres restantes.");
            break;
        case 16:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("En este espacio vacío, sólo hay dos formas ""\n""en las cuales podemos rellenar las dos casillas, ""\n""por lo tanto rellenaremos cualquier casilla común a ambas posibilidades.");
            break;
        case 17:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Este método puede ser usado en varias situaciones.""\n"" Contando el número de casillas, podemos saber ""\n"" qué casillas son comunes a las varias posibilidades. ""\n""Esto incluye cuando hay más de un número por cada fila o columna.");
            break;
        case 18:
            imagen = Director::getInstance()->getTextureCache()->addImage("T17.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            imagen_tutorial->setScale(5,2.5);
            labelTutorial->setString("Ahora, aseguramos la columna con 1, ya que está completa.""\n"" Y tras hacer esto, ""\n""poco a poco iremos rellenando y marcando las demás casillas""\n"" hasta completar la figura.");
            break;
        case 19:
            imagen = Director::getInstance()->getTextureCache()->addImage("FALTA.png");
            imagen->setTexParameters(textpar);
            imagen_tutorial->setTexture(imagen);
            labelTutorial->setString("Aquí concluye el tutorial. ""\n""Esperamos que te diviertas resolviendo puzzles,""\n"" y recuerda aplicar todas las técnicas aprendidas.");
            break;
    };
    
}


void TutorialScene::goToMainMenu(Ref *pSender) {
    
    //Constant::GAMEMODE = GameMode::NORMAL;
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");
    
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5,scene));
}
