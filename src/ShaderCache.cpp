#include "ShaderCache.hpp"

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

std::string ShaderCache::getVertex() {
    static const std::string vertex = R"(
        attribute vec4 a_position;							
        attribute vec2 a_texCoord;							
        attribute vec4 a_color;								
                                                            
        #ifdef GL_ES										
        varying lowp vec4 v_fragmentColor;					
        varying mediump vec2 v_texCoord;					
        #else												
        varying vec4 v_fragmentColor;						
        varying vec2 v_texCoord;							
        #endif												
                                                            
        void main()	{													
            gl_Position = CC_MVPMatrix * a_position;		
            v_fragmentColor = a_color;						
            v_texCoord = a_texCoord;						
        }													
    )";
    return vertex;
}

void ShaderCache::createShader(std::string_view name, std::string_view frag) {
    CCGLProgram* prg = new CCGLProgram();
    prg->initWithVertexShaderByteArray(getVertex().c_str(), std::string(frag).c_str());
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