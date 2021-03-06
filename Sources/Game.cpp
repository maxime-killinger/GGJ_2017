//
// Created by guillaume on 22/01/17.
//

#include <list>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Event.hpp>
#include "../Headers/Entity.h"
#include "../Headers/Timer.h"
#include "../Headers/Player.h"
#include "../Headers/Curve.h"
#include "../Headers/Speaker.h"
#include "../Headers/Drop.h"
#include "../Headers/Game.h"

using namespace sf;

const int W = 480;
const int H = 720;

bool isCollide(Entity *a,Entity *b)
{
    return (b->_x - a->_x)*(b->_x - a->_x)+
           (b->_y - a->_y)*(b->_y - a->_y)<
           (a->_R + b->_R)*(a->_R + b->_R);
}

int game(sf::RenderWindow &window) {
    std::list < Curve *> lCurve;
    std::list < Curve *> rCurve;
    std::list < Entity *> entities;
    sf::VertexArray lines(sf::LinesStrip, 22);
    Player *player = new Player();
    Timer T;
    sf::Text score;
    sf::Font scoreType;
    Drop    *drop = new Drop();

    sf::SoundBuffer hit_sound;
    hit_sound.loadFromFile("../Music/hit.ogg");
    sf::Sound hit(hit_sound);
    hit.setVolume(100);

    sf::SoundBuffer music_coin;
    music_coin.loadFromFile("../Music/coin.ogg");
    sf::Sound coin(music_coin);
    coin.setVolume(30);

    scoreType.loadFromFile("../Assets/score.ttf");
    score.setFont(scoreType);
    score.setCharacterSize(32);
    score.setColor(sf::Color::Cyan);
    drop->settings((rand() % (W - 200)) + 100, rand() % 50, 0, 15);
    entities.push_back(drop);
    player->settings(W / 2, H * 3 / 4, 0, 10);
    for (int i = 0; i < 5; i++) {
        Speaker *speaker = new Speaker();
        speaker->settings((rand() % (W - 200)) + 100, (rand() % H) * -4 , 0, rand() % 25 + 5);
        entities.push_back(speaker);
    }
    for (int i = 0; i < 21; i++) {
        Curve *_curve = new Curve(i * H / 20, true);
        lCurve.push_back(_curve);
        entities.push_back(_curve);
    }
    for (int i = 0; i < 21; i++) {
        Curve *_curve = new Curve(i * H / 20, false);
        _curve->_x = W;
        rCurve.push_back(_curve);
        entities.push_back(_curve);
    }
    entities.push_back(player);
    while (window.isOpen()) {
        sf::Event event;

        window.clear(sf::Color::Black);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return 1; }
        getMovement(player);
        for (auto a:entities)
            for (auto b:entities) {
                if (a->_type == Entity::PLAYER && (b->_type == Entity::SPEAKER || b->_type == Entity::WAVES))
                    if (isCollide(a, b)) {
                        hit.play();
                        player->settings(W / 2, H * 3 / 4, 0, 10);
                        T.set_points(0);
                    }
                if (a->_type == Entity::PLAYER && b->_type == Entity::DROP) {
                    if (isCollide(a, b)) {
                        coin.play();
                        T.set_points(T.get_points() + 1);
                        b->settings((rand() % (W - 200)) + 100, rand() % (H - 40) + 40);
                    }
                }
            }
        for (auto i = entities.begin(); i != entities.end();) {
            Entity *e = *i;
            if (e->_type == Entity::SPEAKER) {
                e->_speed = T.get_points() + 1;
                if (T.get_points() > T._record)
                    T._record =  T.get_points();
            }
            e->update();
            if (!e->_life) {
                if (e->_type == Entity::SPEAKER) {
                    Speaker *speaker = new Speaker();
                    speaker->settings((rand() % (W - 200)) + 100, rand() % 50, 0, rand() % 20 + 5);
                    entities.push_back(speaker);
                }
                i = entities.erase(i);
                delete e;
            } else {
                e->draw(window);
                i++;
            }
        }
        score.setString(std::to_string(T.get_points()));
        setCurve(window, lCurve, rCurve, lines);
        window.draw(score);
        score.setPosition(W - 100, 0);
        score.setString(std::to_string(T._record));
        window.draw(score);
        score.setPosition(0, 0);
        window.display();
    }
    return 0;
}