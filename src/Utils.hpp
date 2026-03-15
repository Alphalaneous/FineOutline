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
                float fade = smoothstep(0.0, 0.25, br);
                c.a = mix(c.a, c.a * fade, condition);
                gl_FragColor = v_fragmentColor * c;
            }
        )";
        return frag;
    }

    // based off of mats implementation, slightly tweaked
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

                float brightness = dot(c.rgb, vec3(1./3.)) / c.a;
                float isOutline = smoothstep(0.75, 0.0, brightness);

                c = vec4(c.a * isOutline);
                gl_FragColor = c * v_fragmentColor;
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
        }
    }
}