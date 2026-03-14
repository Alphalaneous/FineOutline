#pragma once

#include <Geode/Geode.hpp>
#include "ShaderCache.hpp"

using namespace geode::prelude;

namespace alpha::fine_outline {
    
    inline std::string_view getIconFrag() {
        static std::string frag = R"(
            #ifdef GL_ES
            precision mediump float;
            #endif

            varying vec4 v_fragmentColor;
            varying vec2 v_texCoord;
            uniform sampler2D CC_Texture0;

            void main() {
                vec4 c = texture2D(CC_Texture0, v_texCoord);
                float br = max(max(c.r, c.g), c.b);
                float gr = float(abs(c.r - c.g) < 0.25 && abs(c.g - c.b) < 0.25);

                float condition = float(br < 1.0 && c.a > 0.0 && gr > 0.0);
                float fade = smoothstep(0.0, 0.25, br); // adjust 0.25 for AA width
                c.a = mix(c.a, c.a * fade, condition);
                gl_FragColor = v_fragmentColor * c;
            }
        )";
        return frag;
    }

    inline std::string_view getOutlineFrag() {
        static std::string frag = R"(
            #ifdef GL_ES
            precision mediump float;
            #endif

            varying vec4 v_fragmentColor;
            varying vec2 v_texCoord;
            uniform sampler2D CC_Texture0;

            void main() {
                vec4 c = texture2D(CC_Texture0, v_texCoord);
                float br = max(max(c.r, c.g), c.b);
                float gr = float(abs(c.r - c.g) < 0.25 && abs(c.g - c.b) < 0.25);

                float condition = float(br < 1.0 && c.a > 0.0 && gr > 0.0);
                c.a = mix(0.0, c.a * (1.0 - br), condition);
                c.rgb = mix(c.rgb, vec3(1.0), condition);

                gl_FragColor = v_fragmentColor * c;
            }
        )";
        return frag;
    }

    inline void loadShaders() {
        ShaderCache::get()->createShader("icon", getIconFrag());
        ShaderCache::get()->createShader("outline", getOutlineFrag());
    }

    inline void removeShaders(CCSprite* spr) {
        if (!spr) return;
        spr->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        spr->getShaderProgram()->setUniformsForBuiltins();
        spr->getShaderProgram()->use();
        spr->setCascadeOpacityEnabled(false);
        if (!spr->m_pobTexture || !spr->m_pobTexture->hasPremultipliedAlpha()) {
            spr->m_sBlendFunc.src = GL_SRC_ALPHA;
            spr->m_sBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        }
        else {
            spr->m_sBlendFunc.src = CC_BLEND_SRC;
            spr->m_sBlendFunc.dst = CC_BLEND_DST;
        }
    }

    inline CCSprite* createOutline(CCSprite* spr) {
        if (!spr) return nullptr;

        spr->setCascadeOpacityEnabled(true);

        auto blackOutline = CCSprite::createWithSpriteFrame(spr->displayFrame());

        blackOutline->setContentSize(spr->getContentSize());
        blackOutline->setID("black_outline"_spr);
        blackOutline->setPosition(spr->getContentSize()/2);

        if (auto prgOutline = ShaderCache::get()->getProgram("outline")) {
            prgOutline->setUniformsForBuiltins();
            blackOutline->setShaderProgram(prgOutline);
            prgOutline->use();
            blackOutline->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        }

        spr->removeChildByID("black_outline"_spr);
        spr->addChild(blackOutline);

        return blackOutline;
    }

    inline void addShaders(CCSprite* spr) {
        if (!spr) return;

        if (auto progIcon = ShaderCache::get()->getProgram("icon")) {
            progIcon->setUniformsForBuiltins();
            spr->setShaderProgram(progIcon);
            progIcon->use();
            spr->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        }
    }

    inline Mod* getSeparateDualIcons() {
        static auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        return sdi;
    }

    inline bool isSDIDualIcon() {
        auto sdi = getSeparateDualIcons();
        if (!sdi) return false;

        return sdi->getSavedValue<bool>("2pselected");
    }

    inline bool hasOverride() {
        return Mod::get()->getSavedValue<bool>(isSDIDualIcon() ? "override-color-p2" : "override-color"); 
    }

    inline bool hasOverrideP1() {
        return Mod::get()->getSavedValue<bool>("override-color"); 
    }

    inline bool hasOverrideP2() {
        return Mod::get()->getSavedValue<bool>(getSeparateDualIcons() ? "override-color-p2" : "override-color"); 
    }

    inline void setOverride(bool override) {
        Mod::get()->setSavedValue<bool>(isSDIDualIcon() ? "override-color-p2" : "override-color", override);
    }

    inline void setOverrideColor(const ccColor3B& color) {
        Mod::get()->setSavedValue<ccColor3B>(isSDIDualIcon() ? "p2-color" : "p1-color", color);
    }

    inline ccColor3B getOverrideColor() {
        return Mod::get()->getSavedValue<ccColor3B>(isSDIDualIcon() ? "p2-color" : "p1-color");
    }

    inline ccColor3B getOverrideColorP1() {
        return Mod::get()->getSavedValue<ccColor3B>("p1-color");
    }

    inline ccColor3B getOverrideColorP2() {
        return Mod::get()->getSavedValue<ccColor3B>(getSeparateDualIcons() ? "p2-color" : "p1-color");
    }

    inline int getRegularColor() {
        return Mod::get()->getSavedValue<int>(isSDIDualIcon() ? "outline-color-p2" : "outline-color-p1");
    }

    inline int getRegularColorP1() {
        return Mod::get()->getSavedValue<int>("outline-color-p1");
    }

    inline int getRegularColorP2() {
        return Mod::get()->getSavedValue<int>(getSeparateDualIcons() ? "outline-color-p2" : "outline-color-p1");
    }

    inline void setRegularColor(unsigned int color) {
        Mod::get()->setSavedValue<unsigned int>(isSDIDualIcon() ? "outline-color-p2" : "outline-color-p1", color);
    }

    inline ccColor3B getColor() {
        return hasOverride() ? getOverrideColor() : GameManager::get()->colorForIdx(getRegularColor());
    }

    inline ccColor3B getP1Color() {
        return hasOverrideP1() ? getOverrideColorP1() : GameManager::get()->colorForIdx(getRegularColorP1());
    }

    inline ccColor3B getP2Color() {
        return hasOverrideP2() ? getOverrideColorP2() : GameManager::get()->colorForIdx(getRegularColorP2());
    }

    inline ccColor3B getColorFor(bool p2) {
        return p2 ? getP2Color() : getP1Color();
    }
}