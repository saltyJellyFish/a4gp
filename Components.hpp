///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Components.hpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
                    
#pragma once
                    
#include "Animation.hpp"
#include "Assets.h"

class Component
{
public:
    bool exists = false;
};

class CTransform : public Component
{
public:
    Vec2f pos        = { 0.0, 0.0 };
    Vec2f prevPos    = { 0.0, 0.0 };
    Vec2f scale      = { 1.0, 1.0 };
    Vec2f velocity   = { 0.0, 0.0 };
    Vec2f facing     = { 0.0, 1.0 };
    float angle     = 0;

    CTransform() = default;
    CTransform(const Vec2f & p)
        : pos(p) {}
    CTransform(const Vec2f & p, const Vec2f & sp, const Vec2f & sc, float a)
        : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}

};

class CLifeSpan : public Component
{
public:
    int lifespan = 0;
    int frameCreated = 0;
    CLifeSpan() = default;
    CLifeSpan(int duration, int frame) 
        : lifespan(duration), frameCreated(frame) {}
};

class CDamage : public Component
{
public:
    int damage = 1;
    CDamage() = default;
    CDamage(int d)
        : damage(d) {}
};

class CInvincibility : public Component
{
public:
    int iframes = 0;
    CInvincibility() = default;
    CInvincibility(int f)
        : iframes(f) {}
};

class CHealth : public Component
{
public:
    int max = 1;
    int current = 1;
    CHealth() = default;
    CHealth(int m, int c)
        : max(m), current(c) {}
};

class CInput : public Component
{
public:
    bool up         = false;
    bool down       = false;
    bool left       = false;
    bool right      = false;
    bool shoot      = false;
    bool canShoot   = true;
    bool canJump    = true;
                    
    CInput() {}
};

class CBoundingBox : public Component
{
public:
    Vec2f size;
    Vec2f halfSize;    
    bool blockMove = false;
    bool blockVision = false;
    CBoundingBox() = default;
    CBoundingBox(const Vec2f& s)
        : size(s), halfSize(s.x / 2, s.y / 2) {}
    CBoundingBox(const Vec2f& s, bool m, bool v)
        : size(s), blockMove(m), blockVision(v), halfSize(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component
{
public:
    Animation animation;
    bool repeat = false;
    CAnimation() = default;
    CAnimation(const Animation & animation, bool r)
        : animation(animation), repeat(r) {}
};

class CGravity : public Component
{
public:
    float gravity = 0;
    CGravity() = default;
    CGravity(float g) : gravity(g) {}
};

class CState : public Component
{
public:
    std::string state = "jumping";
    CState() = default;
    CState(const std::string & s) : state(s) {}
};

class CFollowPlayer : public Component
{
public:
    Vec2f home = { 0, 0 };
    float speed = 0;
    CFollowPlayer() = default;
    CFollowPlayer(Vec2f p, float s)
        : home(p), speed(s) {}

};

class CPatrol : public Component
{
public:
    std::vector<Vec2f> positions;
    size_t currentPosition = 0;
    float speed = 0;
    CPatrol() = default;
    CPatrol(std::vector<Vec2f>& pos, float s) : positions(pos), speed(s) {}
};


static_assert(std::is_default_constructible_v<CTransform>);
static_assert(std::is_default_constructible_v<CLifeSpan>);
static_assert(std::is_default_constructible_v<CInput>);
static_assert(std::is_default_constructible_v<CBoundingBox>);
static_assert(std::is_default_constructible_v<CAnimation>);
static_assert(std::is_default_constructible_v<CGravity>);
static_assert(std::is_default_constructible_v<CState>);
                    
static_assert(std::is_default_constructible_v<Vec2f>);
static_assert(std::is_default_constructible_v<Animation>);

// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
