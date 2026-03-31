#include "PlayerObject.hpp"
#include "../FineOutline.hpp"
#include "../BypassBatchNode.hpp"

void MyPlayerObject::setupOutlines() {
    auto fields = m_fields.self();

    fields->m_outlines.clear();
    
    fields->m_outlines[m_iconSprite] = alpha::fine_outline::shaders::createOutline(m_iconSprite);
    fields->m_outlines[m_vehicleSprite] = alpha::fine_outline::shaders::createOutline(m_vehicleSprite);

    if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(m_iconSprite);
    if (!fields->m_usingDefaultColor) alpha::fine_outline::shaders::addShaders(m_vehicleSprite);

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

    schedule(schedule_selector(MyPlayerObject::updateOpacity));
}

void MyPlayerObject::updateOutlineColors() {
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

void MyPlayerObject::setOutlineColor(const ccColor3B& color) {
    auto fields = m_fields.self();
    fields->m_outlineColor = color;

    fields->m_usingDefaultColor = color == ccColor3B{0, 0, 0};

    updateOutlineColors();
}

ccColor3B MyPlayerObject::getOutlineColor() {
    auto fields = m_fields.self();
    return fields->m_outlineColor;
}

void MyPlayerObject::enableOutlineColor(bool enable) {
    auto fields = m_fields.self();
    fields->m_usesOutlineColor = enable;

    if (fields->m_usingDefaultColor) return;
    
    for (auto [k, v] : fields->m_outlines) {
        if (enable && !fields->m_usesOutlineColor) alpha::fine_outline::shaders::addShaders(k);
        else alpha::fine_outline::shaders::removeShaders(k);
        if (v) v->setVisible(enable && !fields->m_usesOutlineColor);
    }
}

void MyPlayerObject::updatePlayerFrame(int frame) {
    PlayerObject::updatePlayerFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::updatePlayerShipFrame(int frame) {
    PlayerObject::updatePlayerShipFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::updatePlayerRollFrame(int frame) {
    PlayerObject::updatePlayerRollFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::updatePlayerBirdFrame(int frame) {
    PlayerObject::updatePlayerBirdFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::updatePlayerDartFrame(int frame) {
    PlayerObject::updatePlayerDartFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::updatePlayerSwingFrame(int frame) {
    PlayerObject::updatePlayerSwingFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::updatePlayerJetpackFrame(int frame) {
    PlayerObject::updatePlayerJetpackFrame(frame);
    updateOutlineColors();
}

void MyPlayerObject::createRobot(int frame) {
    PlayerObject::createRobot(frame);

    alpha::fine_outline::utils::replaceVTable<alpha::fine_outline::BypassBatchNode>(m_robotBatchNode);
    updateOutlineColors();
}

void MyPlayerObject::createSpider(int frame) {
    PlayerObject::createSpider(frame);

    alpha::fine_outline::utils::replaceVTable<alpha::fine_outline::BypassBatchNode>(m_spiderBatchNode);
    updateOutlineColors();
}

void MyPlayerObject::updateOpacity(float dt) {
    auto fields = m_fields.self();

    for (auto [k, v] : fields->m_outlines) {
        if (v) v->setOpacity(k->getOpacity());
    }
}