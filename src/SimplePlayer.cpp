#include "SimplePlayer.hpp"
#include "Utils.hpp"

bool MySimplePlayer::init(int id) {
    if (!SimplePlayer::init(id)) return false;
    setupOutlines();
    return true;
}

void MySimplePlayer::setupOutlines() {
    auto fields = m_fields.self();

    for (auto [k, v] : fields->m_outlines) {
        if (v) v->removeFromParent();
    }
    fields->m_outlines.clear();

    fields->m_outlines[m_firstLayer] = alpha::fine_outline::createOutline(m_firstLayer);
    alpha::fine_outline::addShaders(m_firstLayer);

    if (m_robotSprite && m_robotSprite->m_paSprite) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            fields->m_outlines[part] = alpha::fine_outline::createOutline(part);
            alpha::fine_outline::addShaders(part);
        }
    }
    if (m_spiderSprite && m_spiderSprite->m_paSprite) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            fields->m_outlines[part] = alpha::fine_outline::createOutline(part);
            alpha::fine_outline::addShaders(part);
        }
    }
}

void MySimplePlayer::setOutlineColor(const ccColor3B& color) {
    auto fields = m_fields.self();
    fields->m_outlineColor = std::move(color);

    fields->m_usingDefaultColor = color == ccColor3B{0, 0, 0};

    updateOutlineColors();
}

void MySimplePlayer::enableOutlineColor(bool enable) {
    auto fields = m_fields.self();
    fields->m_usesOutlineColor = enable;

    if (fields->m_usingDefaultColor) return;

    for (auto [k, v] : fields->m_outlines) {
        if (enable) alpha::fine_outline::addShaders(k);
        else alpha::fine_outline::removeShaders(k);
        if (v) v->setVisible(enable);
    }
}

void MySimplePlayer::updateOutlineColors() {
    auto fields = m_fields.self();
    setupOutlines();

    for (auto [k, v] : fields->m_outlines) {
        if (fields->m_usingDefaultColor) {
            alpha::fine_outline::removeShaders(k);
        }
        else if (fields->m_usesOutlineColor) {
            alpha::fine_outline::addShaders(k);
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