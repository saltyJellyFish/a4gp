///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Action.hpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
                    
                    
#pragma once

#include "Vec2.hpp"
                    
#include <string>
#include <sstream>

class Action
{
    std::string m_name = "NONE";
    std::string m_type = "NONE";
    Vec2i m_position;
                    
public:

    Action() = default;

    Action(const std::string& name, const std::string& type)
        : m_name(name)
        , m_type(type) { }

    Action(const std::string& name, const std::string& type, const Vec2i& pos)
        : m_name(name)
        , m_type(type)
        , m_position(pos) { }
                    
    const std::string& name() const
    {
        return m_name;
    }
                    
    const std::string& type() const
    {
        return m_type;
    }
                    
    Vec2i pos() const
    {
        return m_position;
    }
};


// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
