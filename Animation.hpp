///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Animation.hpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#pragma once
                    
#include "Assets.h"

#include <vector>
#include <SFML/Graphics.hpp>
                    
class Animation
{
    size_t      m_frameCount = 1;       // total number of frames of animation
    size_t      m_currentFrame = 0;     // the current frame of animation being played
    size_t      m_speed = 0;            // speed to play this animation
    std::string m_name = "none";        // name of the animation
    std::string m_textureName = "none"; // name of the texture to get the frames from
    sf::IntRect m_textureRect;          // sub-rectangle to draw

public:
                    
                    
    Animation() = default;

    Animation(const std::string& name, const std::string& textureName)
        : Animation(name, textureName, 1, 0)
    {
                    
    }

    Animation(const std::string& name, const std::string& textureName, size_t frameCount, size_t speed)
        : m_name(name)
        , m_frameCount(frameCount)
        , m_currentFrame(0)
        , m_speed(speed)
        , m_textureName(textureName)
    {
        const sf::Texture& t = Assets::Instance().getTexture(textureName);

        m_textureRect.size = { int(t.getSize().x / frameCount), int(t.getSize().y) };
    }

    // updates the animation to show the next frame, depending on its speed
    // animation loops when it reaches the end
    void update()
    {
        if (m_frameCount < 2) { return; }

        // add the speed variable to the current frame
        m_currentFrame++;
                    
        // set the currect texture based on the frame of animation
        size_t frame = (m_currentFrame / m_speed) % m_frameCount;
        m_textureRect.position.x = int(frame * m_textureRect.size.x);
    }

    bool hasEnded() const
    {
        return m_currentFrame >= m_frameCount * m_speed;
    }
                    
    const std::string& getName() const
    {
        return m_name;
    }

    const sf::IntRect& getRect() const
    {
        return m_textureRect;
    }
                    
    sf::Sprite getSprite() const
    {
        sf::Sprite sprite(Assets::Instance().getTexture(m_textureName));

        sprite.setOrigin({ m_textureRect.size.x / 2.0f, m_textureRect.size.y / 2.0f });
        sprite.setTextureRect(m_textureRect);

        return sprite;
    }
};


// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
