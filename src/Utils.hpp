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

    inline std::string_view getIconFragAlt() {
        static std::string frag = R"(
            #ifdef GL_ES
            precision mediump float;
            #endif

            varying vec4 v_fragmentColor;
            varying vec2 v_texCoord;
            uniform sampler2D CC_Texture0;

            void main() {
                gl_FragColor = texture2D(CC_Texture0, v_texCoord) * v_fragmentColor;
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

    inline std::string_view getOutlineFragAlt() {
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
                float isOutline = smoothstep(0.9, 0.0, brightness);
                c = vec4(c.a * isOutline);
                gl_FragColor = c * v_fragmentColor;
            }
        )";
        return frag;
    }

    inline void loadShaders() {
        bool alt = Mod::get()->getSettingValue<bool>("alternative-shader");
        ShaderCache::get()->createShader("icon", alt ? getIconFragAlt() : getIconFrag());
        ShaderCache::get()->createShader("outline", alt ? getOutlineFragAlt() : getOutlineFrag());
    }

    inline void removeShaders(CCSprite* spr) {
        spr->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        spr->getShaderProgram()->setUniformsForBuiltins();
        spr->getShaderProgram()->use();
        spr->setCascadeOpacityEnabled(false);
        spr->removeChildByID("black_outline"_spr);
    }

    inline void updateSprite(CCSprite* spr, const ccColor3B& color = {0, 0, 0}) {
        if (!spr || color == ccColor3B{0, 0, 0}) return;

        bool alt = Mod::get()->getSettingValue<bool>("alternative-shader");

        spr->setCascadeOpacityEnabled(true);

        auto blackOutline = CCSprite::createWithSpriteFrame(spr->displayFrame());

        blackOutline->setContentSize(spr->getContentSize());
        blackOutline->setID("black_outline"_spr);
        blackOutline->setPosition(spr->getContentSize()/2);
        blackOutline->setColor(color);

        if (auto prgOutline = ShaderCache::get()->getProgram("outline")) {
            prgOutline->setUniformsForBuiltins();
            blackOutline->setShaderProgram(prgOutline);
            prgOutline->use();
            if (!alt)
                blackOutline->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        }

        if (auto progIcon = ShaderCache::get()->getProgram("icon")) {
            progIcon->setUniformsForBuiltins();
            spr->setShaderProgram(progIcon);
            progIcon->use();
            if (!alt)
                spr->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        }

        spr->removeChildByID("black_outline"_spr);
        spr->addChild(blackOutline);
    }
}