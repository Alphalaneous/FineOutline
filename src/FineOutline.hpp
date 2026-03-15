#pragma once

#include <Geode/Geode.hpp>
#include "hooks/SimplePlayer.hpp"
#include "hooks/PlayerObject.hpp"
#include "ShaderCache.hpp"
#include "../include/Enums.hpp"

using namespace geode::prelude;

namespace alpha::fine_outline {

    namespace impl {
        template<class T>
        concept PlayerType =
            std::derived_from<T, PlayerObject> ||
            std::derived_from<T, SimplePlayer>;

        inline bool isSeparate() {
            return Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        }

        inline bool isEditingP2() {
            auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
            if (!sdi) return false;

            return sdi->getSavedValue<bool>("2pselected");
        }

        inline bool usesCustomColor(PlayerIcon player = PlayerIcon::SELECTED) {
            switch (player) {
                case PlayerIcon::SELECTED: {
                    return Mod::get()->getSavedValue<bool>(isEditingP2() ? "override-color-p2" : "override-color"); 
                }
                case PlayerIcon::ONE: {
                    return Mod::get()->getSavedValue<bool>("override-color"); 
                }
                case PlayerIcon::TWO: {
                    return Mod::get()->getSavedValue<bool>(Loader::get()->getLoadedMod("weebify.separate_dual_icons") ? "override-color-p2" : "override-color"); 
                }
            }
            return false;
        }

        inline void useCustomColor(PlayerIcon player, bool use) {
            switch (player) {
                case PlayerIcon::SELECTED: {
                    Mod::get()->setSavedValue(isEditingP2() ? "override-color-p2" : "override-color", use);
                    break;
                }
                case PlayerIcon::ONE: {
                    Mod::get()->setSavedValue("override-color", use); 
                    break;
                }
                case PlayerIcon::TWO: {
                    Mod::get()->setSavedValue("override-color-p2", use); 
                    break;
                }
            }
        }

        inline cocos2d::ccColor3B getCustomColor(PlayerIcon player = PlayerIcon::SELECTED) {
            switch (player) {
                case PlayerIcon::SELECTED: {
                    return Mod::get()->getSavedValue<ccColor3B>(isEditingP2() ? "p2-color" : "p1-color");
                }
                case PlayerIcon::ONE: {
                    return Mod::get()->getSavedValue<ccColor3B>("p1-color");
                }
                case PlayerIcon::TWO: {
                    return Mod::get()->getSavedValue<ccColor3B>(Loader::get()->getLoadedMod("weebify.separate_dual_icons") ? "p2-color" : "p1-color");
                }
            }
            return {};
        }

        inline void setCustomColor(PlayerIcon player, const cocos2d::ccColor3B& color) {
            switch (player) {
                case PlayerIcon::SELECTED: {
                    Mod::get()->setSavedValue(isEditingP2() ? "p2-color" : "p1-color", color);
                    break;
                }
                case PlayerIcon::ONE: {
                    Mod::get()->setSavedValue("p1-color", color);
                    break;
                }
                case PlayerIcon::TWO: {
                    Mod::get()->setSavedValue("p2-color", color);
                    break;
                }
            }
        }

        inline int getColorIndex(PlayerIcon player = PlayerIcon::SELECTED) {
            switch (player) {
                case PlayerIcon::SELECTED: {
                    return Mod::get()->getSavedValue<int>(isEditingP2() ? "outline-color-p2" : "outline-color-p1");
                }
                case PlayerIcon::ONE: {
                    return Mod::get()->getSavedValue<int>("outline-color-p1");
                }
                case PlayerIcon::TWO: {
                    return Mod::get()->getSavedValue<int>(Loader::get()->getLoadedMod("weebify.separate_dual_icons") ? "outline-color-p2" : "outline-color-p1");
                }
            }
            return 0;
        }

        inline void setColorIndex(PlayerIcon player, int color) {
            switch (player) {
                case PlayerIcon::SELECTED: {
                    Mod::get()->setSavedValue(isEditingP2() ? "outline-color-p2" : "outline-color-p1", color);
                    break;
                }
                case PlayerIcon::ONE: {
                    Mod::get()->setSavedValue("outline-color-p1", color);
                    break;
                }
                case PlayerIcon::TWO: {
                    Mod::get()->setSavedValue("outline-color-p2", color);
                    break;
                }
            }
        }

        inline cocos2d::ccColor3B getColor(PlayerIcon player = PlayerIcon::SELECTED) {
            return impl::usesCustomColor(player) ? impl::getCustomColor(player) : GameManager::get()->colorForIdx(impl::getColorIndex(player));
        }

        template<PlayerType T>
        inline void setOutlineColor(T* player, const cocos2d::ccColor3B& color) {
            if constexpr (std::is_base_of_v<PlayerObject, T>) {
                auto pl = static_cast<MyPlayerObject*>(player);
                pl->setOutlineColor(color);
            }
            else if constexpr (std::is_base_of_v<SimplePlayer, T>) {
                auto pl = static_cast<MySimplePlayer*>(player);
                pl->setOutlineColor(color);
            }
        }

        template<PlayerType T>
        inline cocos2d::ccColor3B getOutlineColor(T* player) {
            if constexpr (std::is_base_of_v<PlayerObject, T>) {
                auto pl = static_cast<MyPlayerObject*>(player);
                return pl->getOutlineColor();
            }
            else if constexpr (std::is_base_of_v<SimplePlayer, T>) {
                auto pl = static_cast<MySimplePlayer*>(player);
                return pl->getOutlineColor();
            }
        }

        template<PlayerType T>
        inline void updateOutline(T* player) {
            if constexpr (std::is_base_of_v<PlayerObject, T>) {
                auto pl = static_cast<MyPlayerObject*>(player);
                pl->updateOutlineColors();
            }
            else if constexpr (std::is_base_of_v<SimplePlayer, T>) {
                auto pl = static_cast<MySimplePlayer*>(player);
                pl->updateOutlineColors();
            }
        }
    }

    namespace shaders {
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

                    float brightness = dot(c.rgb, vec3(1./3.)) / max(c.a, 0.001);

                    float outlineMask = smoothstep(1.0, 0.0, brightness);

                    float shadowMask = smoothstep(0.5, 0.0, brightness) * (1.0 - outlineMask);

                    vec3 shadowDarkened = mix(c.rgb, c.rgb * 0.1, shadowMask);

                    vec3 finalRGB = mix(shadowDarkened, vec3(1.0), outlineMask);

                    float isOutline = smoothstep(0.3, 0.0, brightness);
                    float finalA = c.a * (1.0 - isOutline);

                    finalRGB *= finalA;

                    gl_FragColor = vec4(finalRGB, finalA) * v_fragmentColor;
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

                    float brightness = dot(c.rgb, vec3(1./3.)) / max(c.a, 0.001);
                    float isOutline = smoothstep(0.75, 0.0, brightness);

                    float offset = 0.003;
                    vec2 offsets[4];
                    offsets[0] = vec2(-offset, 0.0);
                    offsets[1] = vec2(offset, 0.0);
                    offsets[2] = vec2(0.0, -offset);
                    offsets[3] = vec2(0.0, offset);

                    for (int i = 0; i < 4; ++i) {
                        vec4 n = texture2D(CC_Texture0, v_texCoord + offsets[i]);
                        float b = dot(n.rgb, vec3(1./3.)) / max(n.a, 0.001);
                        isOutline = max(isOutline, smoothstep(0.75, 0.0, b));
                    }

                    gl_FragColor = vec4(c.a * isOutline);
                    gl_FragColor *= v_fragmentColor;
                }
            )";
            return frag;
        }

        inline std::string getVertex() {
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

        inline void reloadShaders() {
            ShaderCache::get()->clearShaders();
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

    namespace utils {
        template<class T>
        inline void* getCustomVTablePtr() {
            static void* vtable = []() -> void* {
                T node;
                return *(void**)&node;
            }();
            return vtable;
        }

        template<class T>
        inline void replaceVTable(void* ptr) {
            *reinterpret_cast<void**>(ptr) = getCustomVTablePtr<T>();
        }
    }
}