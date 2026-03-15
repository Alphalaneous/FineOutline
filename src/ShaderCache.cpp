#include "ShaderCache.hpp"
#include "FineOutline.hpp"

ShaderCache* ShaderCache::get() {
    static ShaderCache instance;
    return &instance;
}

void ShaderCache::add(std::string_view name, CCGLProgram* program) {
    m_shaders[std::string(name)] = program;
}

CCGLProgram* ShaderCache::getProgram(std::string_view name) {
    auto iter = m_shaders.find(name);
    if (iter != m_shaders.end()) {
        return iter->second;
    }
    return nullptr;
}

void ShaderCache::createShader(std::string_view name, std::string_view frag) {
    CCGLProgram* prg = new CCGLProgram();
    prg->initWithVertexShaderByteArray(alpha::fine_outline::shaders::getVertex().c_str(), std::string(frag).c_str());
    prg->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    prg->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    prg->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    prg->autorelease();
    prg->link();
    prg->updateUniforms();
    add(name, prg);
}

void ShaderCache::clearShaders() {
    m_shaders.clear();
}