#include "SimplePlayer.hpp"
#include "../FineOutline.hpp"

void MySimplePlayer::setupOutlines() {
    auto fields = m_fields.self();

    fields->m_outlines.clear();

    fields->m_outlines[m_firstLayer] = alpha::fine_outline::shaders::createOutline(m_firstLayer);
    if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(m_firstLayer);

    if (m_robotSprite && m_robotSprite->m_paSprite && m_robotSprite->m_paSprite->m_spriteParts) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            fields->m_outlines[part] = alpha::fine_outline::shaders::createOutline(part);
            if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(part);
        }
        if (m_robotSprite->m_extraSprite) {
            fields->m_outlines[m_robotSprite->m_extraSprite] = alpha::fine_outline::shaders::createOutline(m_robotSprite->m_extraSprite);
            if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(m_robotSprite->m_extraSprite);
        }
    }
    if (m_spiderSprite && m_spiderSprite->m_paSprite && m_spiderSprite->m_paSprite->m_spriteParts) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            fields->m_outlines[part] = alpha::fine_outline::shaders::createOutline(part);
            if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(part);
        }
        if (m_spiderSprite->m_extraSprite) {
            fields->m_outlines[m_spiderSprite->m_extraSprite] = alpha::fine_outline::shaders::createOutline(m_spiderSprite->m_extraSprite);
            if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(m_spiderSprite->m_extraSprite);
        }
    }
}

void MySimplePlayer::setOutlineColor(const ccColor3B& color) {
    auto fields = m_fields.self();
    fields->m_outlineColor = color;

    fields->m_usingDefaultColor = color == ccColor3B{0, 0, 0};

    updateOutlineColors();
}

ccColor3B MySimplePlayer::getOutlineColor() {
    auto fields = m_fields.self();
    return fields->m_outlineColor;
}

void MySimplePlayer::enableOutlineColor(bool enable) {
    auto fields = m_fields.self();
    fields->m_usesOutlineColor = enable;

    for (auto [k, v] : fields->m_outlines) {
        if (enable && !fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(k);
        else alpha::fine_outline::shaders::removeShaders(k);
        if (v) v->setVisible(enable && !fields->m_usesOutlineColor);
    }
}

void MySimplePlayer::updateOutlineColors() {
    auto fields = m_fields.self();
    setupOutlines();

    for (auto [k, v] : fields->m_outlines) {
        if (fields->m_usingDefaultColor) {
            alpha::fine_outline::shaders::removeShaders(k);
        }
        else if (fields->m_usesOutlineColor) {
            alpha::fine_outline::shaders::addShaders(k);
        }
        if (v) {
            v->setColor(fields->m_outlineColor);
            v->setVisible(fields->m_usesOutlineColor && !fields->m_usingDefaultColor);
        }
    }
}

void MySimplePlayer::updatePlayerFrame(int p0, IconType p1) {
    SimplePlayer::updatePlayerFrame(p0, p1);
    updateOutlineColors();
}