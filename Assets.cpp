///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Assets.cpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#include "Assets.h"
                    
#include "Animation.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <cassert>
#include <iostream>
#include <fstream>
                    
Assets& Assets::Instance()
{
    static Assets assets;
    return assets;
}

void Assets::clear()
{
    m_soundMap.clear();
    m_soundBufferMap.clear();
    m_animationMap.clear();
    m_fontMap.clear();
    m_textureMap.clear();
}

void Assets::addTexture(const std::string& textureName, const std::string& path, bool smooth)
{
    m_textureMap[textureName] = sf::Texture();
                    
    if (!m_textureMap[textureName].loadFromFile(path))
    {
        std::cerr << "Could not load texture file: " << path << std::endl;
        m_textureMap.erase(textureName);
    }
    else
    {
        m_textureMap[textureName].setSmooth(smooth);
        std::cout << "Loaded Texture: " << path << std::endl;
    }
}

void Assets::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed)
{
    m_animationMap[animationName] = Animation(animationName, textureName, frameCount, speed);
}

void Assets::addFont(const std::string& fontName, const std::string& path)
{
    m_fontMap[fontName] = sf::Font();
    if (!m_fontMap[fontName].openFromFile(path))
    {
        std::cerr << "Could not load font file: " << path << std::endl;
        m_fontMap.erase(fontName);
    }
    else
    {
        std::cout << "Loaded Font:    " << path << std::endl;
    }
}

void Assets::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    std::string str;
    while (file.good())
    {
        file >> str;

        if (str == "Texture")
        {
            std::string name, path;
            file >> name >> path;
            addTexture(name, path);
        }
        else if (str == "Animation")
        {
            std::string name, texture;
            size_t frames, speed;
            file >> name >> texture >> frames >> speed;
            addAnimation(name, texture, frames, speed);
        }
        else if (str == "Font")
        {
            std::string name, path;
            file >> name >> path;
            addFont(name, path);
        }
        else if (str == "Sound")
        {
            std::string name, path;
            file >> name >> path;
            addSound(name, path);
        }
        else
        {
            std::cerr << "Unknown Asset Type: " << str << std::endl;
        }
    }
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const
{
    assert(m_textureMap.find(textureName) != m_textureMap.end());
    return m_textureMap.at(textureName);
}

const Animation& Assets::getAnimation(const std::string& animationName) const
{
    assert(m_animationMap.find(animationName) != m_animationMap.end());
    return m_animationMap.at(animationName);
}


const sf::Font& Assets::getFont(const std::string& fontName) const
{
    assert(m_fontMap.find(fontName) != m_fontMap.end());
    return m_fontMap.at(fontName);
}

const std::map<std::string, sf::Texture>& Assets::getTextures() const
{
    return m_textureMap;
}

const std::map<std::string, Animation>& Assets::getAnimations() const
{
    return m_animationMap;
}

void Assets::addSound(const std::string& soundName, const std::string& path)
{
    // create (or find) an empty buffer entry
    auto [bufIt, bufInserted] = m_soundBufferMap.emplace(soundName, sf::SoundBuffer{});
                    
    if (!bufIt->second.loadFromFile(path))
    {
        std::cerr << "Could not load sound file: " << path << std::endl;
        if (bufInserted) { m_soundBufferMap.erase(bufIt); }
        return;
    }

    std::cout << "Loaded Sound:   " << path << std::endl;

    // construct sf::Sound directly from the buffer (no default ctor)
    auto [sndIt, sndInserted] =
        m_soundMap.emplace(std::piecewise_construct,
            std::forward_as_tuple(soundName),
            std::forward_as_tuple(bufIt->second));
                    
    // set properties on the stored sound
    sndIt->second.setVolume(25);
}

sf::Sound& Assets::getSound(const std::string& soundName)
{
    assert(m_soundMap.find(soundName) != m_soundMap.end());
    return m_soundMap.at(soundName);
}
                    
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
