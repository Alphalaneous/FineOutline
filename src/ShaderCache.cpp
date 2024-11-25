#include "ShaderCache.h"

ShaderCache* ShaderCache::instance = nullptr;

void ShaderCache::init() {
    m_shaders = CCDictionary::create();
}

void ShaderCache::add(std::string name, CCGLProgram* program) {
    m_shaders->setObject(program, name);
}

CCGLProgram* ShaderCache::getProgram(std::string name) {
    return static_cast<CCGLProgram*>(m_shaders->objectForKey(name));
}
std::string ShaderCache::getVertex() {
    return m_vertex;
}

void ShaderCache::createShader(std::string name, std::string frag) {
    CCGLProgram* prg = new CCGLProgram();
    prg->initWithVertexShaderByteArray(getVertex().c_str(), frag.c_str());
    prg->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    prg->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    prg->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    prg->autorelease();
    prg->link();
    prg->updateUniforms();
    add(name, prg);
}

