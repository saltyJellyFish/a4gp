///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Scene.h
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#pragma once
                    
#include "Action.hpp"
#include "EntityManager.hpp"

#include <memory>

class GameEngine;

using ActionMap = std::map<sf::Keyboard::Scancode, std::string>;

class Scene
{
                    
protected: 
    
    GameEngine&     m_game;
    EntityManager   m_entityManager;
    ActionMap       m_actionMap;
    bool            m_paused = false;
    bool            m_hasEnded = false;
    size_t          m_currentFrame = 0;

    virtual void onEnd() = 0;
    void setPaused(bool paused);
                    
public:

    Scene(GameEngine& gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action & action) = 0;
    virtual void sRender() = 0;

    virtual void doAction(const Action& action);
    void simulate(const size_t frames);
    void registerAction(sf::Keyboard::Scancode inputKey, const std::string& actionName);

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;

    bool hasEnded() const;
    const ActionMap& getActionMap() const;
};
                    
                    
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
