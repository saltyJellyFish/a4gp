///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Scene_Menu.h
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
#include "EntityManager.hpp"

#include <map>
#include <memory>
#include <deque>
#include <vector>

class Scene_Menu : public Scene
{

protected:
                    
    std::string                 m_title;
    std::vector<std::string>    m_menuStrings;
    std::vector<std::string>    m_levelPaths;
    size_t                      m_selectedMenuIndex = 0;
    
    void init();
    void update();
    void onEnd();
    void sDoAction(const Action& action);
                    
public:

    Scene_Menu(GameEngine& gameEngine);
    void sRender();

};
                                   
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
