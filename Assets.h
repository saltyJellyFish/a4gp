///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Assets.h
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <cassert>
#include <iostream>
#include <fstream>

class Animation;
                    
class Assets
{
    std::map<std::string, sf::Texture>      m_textureMap;
    std::map<std::string, Animation>        m_animationMap;
    std::map<std::string, sf::Font>         m_fontMap;
    std::map<std::string, sf::SoundBuffer>  m_soundBufferMap;
    std::map<std::string, sf::Sound>        m_soundMap;

    void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
    void addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed);
    void addFont(const std::string& fontName, const std::string& path);
    void addSound(const std::string& soundName, const std::string& path);

    Assets() = default;

public:
                    
    static Assets& Instance();

    void loadFromFile(const std::string& path);
    void clear();
                    
    const sf::Texture& getTexture(const std::string& textureName) const;
    const Animation& getAnimation(const std::string& animationName) const;
    const sf::Font& getFont(const std::string& fontName) const;
    const std::map<std::string, sf::Texture>& getTextures() const;
    const std::map<std::string, Animation>& getAnimations() const;
    sf::Sound& getSound(const std::string& soundName);
};
                    
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
