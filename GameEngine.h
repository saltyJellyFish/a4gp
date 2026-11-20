///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       GameEngine.h
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
#include "Assets.h"

#include <memory>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{

protected:

    sf::RenderWindow    m_window;
    std::string         m_currentScene;
    SceneMap            m_sceneMap;
    size_t              m_simulationSpeed = 1;
    bool                m_running = true;
    sf::Clock           m_deltaClock;

    void init(const std::string & path);
    void update();
                    
    void sUserInput();

    std::shared_ptr<Scene> currentScene();
                    
public:
    
    GameEngine(const std::string & path);
                    
    void changeScene(const std::string & sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

    void quit();
    void run();

    void playSound(const std::string& soundName);
    void stopSound(const std::string& soundName);
                    
    sf::RenderWindow & window();
    bool isRunning();
};
                    
                    
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
