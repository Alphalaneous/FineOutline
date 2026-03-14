#include "PlayerObject.hpp"
#include "BypassBatchNode.hpp"
#include "Utils.hpp"

bool MyPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer, cocos2d::CCLayer* layer, bool playLayer) {
    if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) return false;
    setupOutlines();
    return true;
}

void MyPlayerObject::setupOutlines() {
    auto fields = m_fields.self();

    fields->m_outlines.clear();
    
    fields->m_outlines[m_iconSprite] = alpha::fine_outline::createOutline(m_iconSprite);
    fields->m_outlines[m_vehicleSprite] = alpha::fine_outline::createOutline(m_vehicleSprite);
    fields->m_outlines[m_birdVehicle] = alpha::fine_outline::createOutline(m_birdVehicle);

    alpha::fine_outline::addShaders(m_iconSprite);
    alpha::fine_outline::addShaders(m_vehicleSprite);
    alpha::fine_outline::addShaders(m_birdVehicle);

    if (m_robotSprite && m_robotSprite->m_paSprite && m_robotSprite->m_paSprite->m_spriteParts) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            fields->m_outlines[part] = alpha::fine_outline::createOutline(part);
            alpha::fine_outline::addShaders(part);
        }
    }
    if (m_spiderSprite && m_spiderSprite->m_paSprite && m_spiderSprite->m_paSprite->m_spriteParts) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            fields->m_outlines[part] = alpha::fine_outline::createOutline(part);
            alpha::fine_outline::addShaders(part);
        }
    }
}

void MyPlayerObject::updateOutlineColors() {
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

void MyPlayerObject::setOutlineColor(const ccColor3B& color) {
    auto fields = m_fields.self();
    fields->m_outlineColor = std::move(color);

    fields->m_usingDefaultColor = color == ccColor3B{0, 0, 0};

    updateOutlineColors();
}

void MyPlayerObject::enableOutlineColor(bool enable) {
    auto fields = m_fields.self();
    fields->m_usesOutlineColor = enable;

    if (fields->m_usingDefaultColor) return;
    
    for (auto [k, v] : fields->m_outlines) {
        if (enable) alpha::fine_outline::addShaders(k);
        else alpha::fine_outline::removeShaders(k);
        if (v) v->setVisible(enable);
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

void* MyPlayerObject::getCustomVTablePtr() {
    static void* vtable = []() -> void* {
        alpha::fine_outline::BypassBatchNode node;
        return *(void**)&node;
    }();
    return vtable;
}

void MyPlayerObject::replaceBatchWithNode(CCSpriteBatchNode* batchNode) {
    alpha::fine_outline::BypassBatchNode node;
    *reinterpret_cast<void**>(batchNode) = getCustomVTablePtr();
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
    replaceBatchWithNode(m_robotBatchNode);
    updateOutlineColors();
}

void MyPlayerObject::createSpider(int frame) {
    PlayerObject::createSpider(frame);
    replaceBatchWithNode(m_spiderBatchNode);
    updateOutlineColors();
}