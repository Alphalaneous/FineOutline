#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShaderCache {
public:
    CCGLProgram* getProgram(std::string_view name);

    void add(std::string_view name, CCGLProgram* program);
    void createShader(std::string_view name, std::string_view frag);
    void clearShaders();

    static std::string getVertex();
    static ShaderCache* get();

protected:
    StringMap<Ref<CCGLProgram>> m_shaders;
};