///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       GameEngine.cpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////



#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Menu.h"
                    
#include "imgui.h"
#include "imgui-SFML.h"

GameEngine::GameEngine(const std::string & path)
{
    init(path);
}

void GameEngine::init(const std::string & path)
{
    Assets::Instance().loadFromFile(path);

    m_window.create(sf::VideoMode({ 1280, 768 }), "Definitely Not Zelda", sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    if (!ImGui::SFML::Init(m_window)) {}
                    
    // scale the imgui ui and text size by 2
    // ImGui::GetStyle().ScaleAllSizes(2.0f);
    // ImGui::GetIO().FontGlobalScale = 2.0f;

    changeScene("MENU", std::make_shared<Scene_Menu>(*this));
}
                    
std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}
                    
bool GameEngine::isRunning()
{ 
    return m_running && m_window.isOpen();
}

sf::RenderWindow & GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }

    // release all textures while SFML is still alive: if the last one is destroyed
    // during static destruction, SFML's internal globals are already gone and it crashes
    ImGui::SFML::Shutdown();
    Assets::Instance().clear();
}

void GameEngine::sUserInput()
{
    while (auto event = m_window.pollEvent())
    {
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(m_window, *event);

        // this event triggers when the window is closed
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // if the current scene does not have an action associated with this key, skip the event
            auto it = currentScene()->getActionMap().find(keyPressed->scancode);
            if (it != currentScene()->getActionMap().end())
            {
                currentScene()->doAction(Action(it->second, "START"));
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>())
        {
            // if the current scene does not have an action associated with this key, skip the event
            auto it = currentScene()->getActionMap().find(keyPressed->scancode);
            if (it != currentScene()->getActionMap().end())
            {
                currentScene()->doAction(Action(it->second, "END"));
            }
        }

        // if the mouse is over an ImGui window, don't process this mouse event
        if (ImGui::GetIO().WantCaptureMouse) { continue; }

        // process mouse events
        if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>())
        {
            switch (mb->button)
            {
                case sf::Mouse::Button::Left:   { currentScene()->doAction(Action("LEFT_CLICK",   "START", mb->position)); break; }
                case sf::Mouse::Button::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "START", mb->position)); break; }
                case sf::Mouse::Button::Right:  { currentScene()->doAction(Action("RIGHT_CLICK",  "START", mb->position)); break; }
                default: break;
            }
        }

        if (const auto* mb = event->getIf<sf::Event::MouseButtonReleased>())
        {
            switch (mb->button)
            {
                case sf::Mouse::Button::Left:   { currentScene()->doAction(Action("LEFT_CLICK",   "END", mb->position)); break; }
                case sf::Mouse::Button::Middle: { currentScene()->doAction(Action("MIDDLE_CLICK", "END", mb->position)); break; }
                case sf::Mouse::Button::Right:  { currentScene()->doAction(Action("RIGHT_CLICK",  "END", mb->position)); break; }
                default: break;
            }
        }

        if (const auto* mm = event->getIf<sf::Event::MouseMoved>())
        {
            const Vec2i mpos(mm->position.x, mm->position.y);
            currentScene()->doAction(Action("MOUSE_MOVE", "START", mpos));
        }
    }
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    if (scene)
    {
        m_sceneMap[sceneName] = scene;
    }
    else
    {
        if (m_sceneMap.find(sceneName) == m_sceneMap.end())
        {
            std::cerr << "Warning: Scene does not exist: " << sceneName << std::endl;
            return;
        }
    }

    if (endCurrentScene)
    {
        m_sceneMap.erase(m_sceneMap.find(m_currentScene));
    }

    m_currentScene = sceneName;
}

void GameEngine::update()
{
    ImGui::SFML::Update(m_window, m_deltaClock.restart());

    if (!isRunning()) { return; }
    
    if (m_sceneMap.empty()) { return; }
    
    sUserInput();

    // the window may have been closed while processing input, so don't draw to it
    if (!isRunning()) { return; }

    currentScene()->simulate(m_simulationSpeed);
    currentScene()->sRender();
    ImGui::SFML::Render(m_window);
    m_window.display();
}
                    
void GameEngine::quit()
{
    m_running = false;
}

void GameEngine::playSound(const std::string& soundName)
{
    Assets::Instance().getSound(soundName).play();
}
                    
void GameEngine::stopSound(const std::string& soundName)
{
    Assets::Instance().getSound(soundName).stop();
}



// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
