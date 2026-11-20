///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Scene_Zelda.h
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////


#pragma once
                    
#include "Scene.h"
#include <map>
#include <memory>

#include "EntityManager.hpp"

class Scene_Zelda : public Scene
{

    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED;
        int HEALTH;
        std::string WEAPON;
    };
                    
protected:

    std::shared_ptr<Entity> m_player;
    std::shared_ptr<Entity> m_selectedEntity;
    std::string             m_levelPath;
    PlayerConfig            m_playerConfig{};
    bool                    m_drawTextures = true;
    bool                    m_drawCollision = false;
    bool                    m_follow = false;
    bool                    m_drawGrid = false;
    bool                    m_drawMinimap = true;
    const Vec2f             m_gridSize = { 64, 64 };
    
    void init(const std::string & levelPath);

    void loadLevel(const std::string & filename);

    void onEnd();
    void update();
    void spawnPlayer();
    void spawnSword();
    void placeSword(const std::shared_ptr<Entity>& sword);
    void syncSwords();
    Vec2f getPosition(int sx, int sy, int tx, int ty) const;
    bool hasLineOfSight(const std::shared_ptr<Entity>& npc) const;
    void drawLine(const Vec2f& p1, const Vec2f& p2, const sf::Color& color);
    void drawMinimap();
    
    void sGUI();
    void sMovement();
    void sAI();
    void sStatus();
    void sAnimation();
    void sCollision();
    void sCamera();
    

public:
                    
    Scene_Zelda(GameEngine& gameEngine, const std::string& levelPath);

    void sRender();
    void sDoAction(const Action& action);
};
          
      
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
