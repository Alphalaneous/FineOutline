#include "SimplePlayer.hpp"
#include "Utils.hpp"
#include <Geode/binding/CCSpritePart.hpp>

void MySimplePlayer::removeAllShaders() {
    auto fields = m_fields.self();

    fields->m_isShaderSpr = false;
    alpha::fine_outline::removeShaders(m_firstLayer);

    if (m_robotSprite && m_robotSprite->m_paSprite) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            alpha::fine_outline::removeShaders(part);
        }
    }
    if (m_spiderSprite && m_spiderSprite->m_paSprite) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            alpha::fine_outline::removeShaders(part);
        }
    }
}

void MySimplePlayer::setOutlineColor(const ccColor3B& color, bool dual) {

    if (color == ccColor3B{0, 0, 0}) {
        removeAllShaders();
        return;
    }

    if (auto blackOutline = typeinfo_cast<CCSprite*>(m_firstLayer->getChildByID("black_outline"_spr))) {
        blackOutline->setColor(color);
    }
    else {
        updatePlayerShaders(dual);
    }

    if (m_robotSprite && m_robotSprite->m_paSprite) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            if (auto blackOutline = typeinfo_cast<CCSprite*>(part->getChildByID("black_outline"_spr))) {
                blackOutline->setColor(color);
            }
        }
    }
    if (m_spiderSprite && m_spiderSprite->m_paSprite) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            if (auto blackOutline = typeinfo_cast<CCSprite*>(part->getChildByID("black_outline"_spr))) {
                blackOutline->setColor(color);
            }
        }
    }
}

void MySimplePlayer::updatePlayerShaders(bool dual) {
    auto fields = m_fields.self();

    fields->m_isShaderSpr = true;
    fields->m_shaderSprDual = dual;

    std::string whichPlayer = dual ? "outline-color-p2" : "outline-color-p1";

    ccColor3B outlineColor;
    if (Mod::get()->getSavedValue<bool>("override-color")) {
        outlineColor = Mod::get()->getSavedValue<ccColor3B>(dual ? "p2-color" : "p1-color");
    }
    else {
        outlineColor = GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>(whichPlayer, 15));
    }

    if (m_robotSprite && m_robotSprite->m_paSprite) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            alpha::fine_outline::updateSprite(part, outlineColor);
        }
    }
    if (m_spiderSprite && m_spiderSprite->m_paSprite) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            alpha::fine_outline::updateSprite(part, outlineColor);
        }
    }
    alpha::fine_outline::updateSprite(m_firstLayer, outlineColor);
    
}

void MySimplePlayer::updatePlayerFrame(int p0, IconType p1) {
    SimplePlayer::updatePlayerFrame(p0, p1);
    auto fields = m_fields.self();

    if (fields->m_isShaderSpr) {
        updatePlayerShaders(fields->m_shaderSprDual);
    }
}