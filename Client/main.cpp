#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameManager.h"


using namespace std;
using namespace sf;

GameManager gameManager(1);
bool gameStarted = false;
bool lobbyMade = false;
sf::Font font;

void error(const char *msg) {
    perror(msg);
    exit(0);
}


void setDicePos() {
    gameManager.die.diceSprites[0].setPosition(260, 284);
    gameManager.die.diceSprites[1].setPosition(310, 284);
}

int main() {
    if (!font.loadFromFile("Fonts/TheLightFont.ttf")) {
        perror("Font couldn't load");
    }

    //Seeding random function
    srand(time(NULL));

    if(!gameStarted) {
         if(!gameManager.initGame()) {
             return 0;
         } else {
             gameStarted = true;
         }
     }

    //Making window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dice game");


    //Temp background location
    Texture bgTex;
    Sprite bgSpr;

    //Loading the background image
    if(!bgTex.loadFromFile("Client/Sprites/board.png")) {
        cout << "Error loading BG texture";
    }
    bgSpr.setTexture(bgTex);

    //Dice tray
    sf::Texture trayTex;
    sf::Sprite traySpr;
    if(!trayTex.loadFromFile("Client/Sprites/spr_diceTray.png")) {
        cout << "Error loading tray Texture";
    }
    traySpr.setTexture(trayTex);
    traySpr.setOrigin(100, 50);
    traySpr.setPosition(300, 300);


    int counter = 0;
    int countto = 3000;

    // run the program as long as the window is open
    while (window.isOpen()) {

        gameManager.network(); // used to take orders received from the server

        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            //Checking for left mouse press
            if(event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Button::Left) {
                    gameManager.roll();
                }
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        //handle player physics
        for(int i = 0 ; i < 6; i++){
            if(!gameManager.players[i].hasPlayer){ continue;}   //if the player is active
            gameManager.players[i].physics();
        }


        Sprite dice1Spr, dice2Spr;

        window.clear(Color::Black);
        window.draw(bgSpr);
        window.draw(traySpr);

        gameManager.fieldList.displayActiveFields(window, gameManager.fieldList);

        //Drawing the dices
        setDicePos();
        window.draw(gameManager.die.diceSprites[0]);
        window.draw(gameManager.die.diceSprites[1]);
        for (int i = 0; i < 6; i++){
            window.draw(gameManager.drawPlayerShadow(i));
            window.draw(gameManager.drawPlayer(i));
            window.draw(gameManager.drawPlayerPic(i));
        }

        if(gameManager.checkWinCondition() != "None"){
            window.clear(Color::Black);
            Text text;
            text.setFont(font);
            string winner = gameManager.checkWinCondition();
            text.setString("Player "+winner+" won!");
            text.setCharacterSize(60);
            text.setFillColor(Color::White);
            text.setPosition(200,200);
            window.draw(text);
        }
        window.display();

    }

    return 0;
}





