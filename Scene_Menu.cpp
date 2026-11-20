///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Scene_Menu.cpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#include "Scene_Menu.h"
#include "Scene_Zelda.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

Scene_Menu::Scene_Menu(GameEngine& gameEngine)
    : Scene(gameEngine)
{
    init();
}
                    
void Scene_Menu::init()
{
    registerAction(sf::Keyboard::Scancode::W,       "UP");
    registerAction(sf::Keyboard::Scancode::S,       "DOWN");
    registerAction(sf::Keyboard::Scancode::D,       "ZELDA");
    registerAction(sf::Keyboard::Scancode::Escape,  "QUIT");

    m_title = "Definitely Not Zelda";
    m_menuStrings.push_back("Level  1");
    m_menuStrings.push_back("Level  2");
    m_menuStrings.push_back("Level  3");

    m_levelPaths.push_back("level1.txt");
    m_levelPaths.push_back("level2.txt");
    m_levelPaths.push_back("level3.txt");
}

void Scene_Menu::update()
{
    m_entityManager.update();

}

void Scene_Menu::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (m_selectedMenuIndex > 0) { m_selectedMenuIndex--; }
            else { m_selectedMenuIndex = m_menuStrings.size() - 1; }
        }
        else if (action.name() == "DOWN")
        {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        }
        else if (action.name() == "ZELDA")
        {
            m_game.changeScene("ZELDA", std::make_shared<Scene_Zelda>(m_game, m_levelPaths[m_selectedMenuIndex]));
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void Scene_Menu::sRender()
{
    // clear the window to a blue
    m_game.window().setView(m_game.window().getDefaultView());
    m_game.window().clear(sf::Color(0, 0, 0));

    sf::Text menuText(Assets::Instance().getFont("Megaman"));
    menuText.setCharacterSize(48);
    
    // draw the game title in the top-left of the screen
    menuText.setString(m_title);
    menuText.setFillColor(sf::Color(100, 100, 100));
    menuText.setPosition(sf::Vector2f(10, 10));
    m_game.window().draw(menuText);
    
    // draw all of the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        menuText.setString(m_menuStrings[i]);
        menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
        m_game.window().draw(menuText);
    }

    // draw the controls in the bottom-left
    menuText.setCharacterSize(20);
    menuText.setFillColor(sf::Color(100, 100, 100));
    menuText.setString("up: w     down: s    play: d      back: esc");
    menuText.setPosition(sf::Vector2f(10, 690));
    m_game.window().draw(menuText);
}

void Scene_Menu::onEnd()
{
    m_hasEnded = true;
    m_game.quit();
}

// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
