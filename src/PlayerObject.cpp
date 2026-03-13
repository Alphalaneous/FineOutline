#include "PlayerObject.hpp"
#include "BypassBatchNode.hpp"
#include "Geode/utils/cocos.hpp"
#include "Utils.hpp"

void MyPlayerObject::updatePlayerShaders() {
    if (!m_gameLayer || !(m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) return;

    ccColor3B outlineColor;
    if (Mod::get()->getSavedValue<bool>("override-color")) {
        std::string whichPlayer = Loader::get()->isModLoaded("weebify.separate_dual_icons") && m_gameLayer->m_player2 == this ? "p2-color" : "p1-color";
        outlineColor = Mod::get()->getSavedValue<ccColor3B>(whichPlayer);
    }
    else {
        std::string whichPlayer = Loader::get()->isModLoaded("weebify.separate_dual_icons") && m_gameLayer->m_player2 == this ? "outline-color-p2" : "outline-color-p1";
        outlineColor = GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>(whichPlayer, 15));
    }

    alpha::fine_outline::updateSprite(m_iconSprite, outlineColor);
    alpha::fine_outline::updateSprite(m_vehicleSprite, outlineColor);
    alpha::fine_outline::updateSprite(m_birdVehicle, outlineColor);
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
}

bool MyPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool highGraphics) {
    if (!PlayerObject::init(player, ship, gameLayer, layer, highGraphics)) return false;
    runAction(CallFuncExt::create([this] {
        updatePlayerShaders();
    }));
    return true;
}

void MyPlayerObject::updatePlayerFrame(int frame) {
    PlayerObject::updatePlayerFrame(frame);
    updatePlayerShaders();
}

void MyPlayerObject::updatePlayerShipFrame(int frame) {
    PlayerObject::updatePlayerShipFrame(frame);
    updatePlayerShaders();
}

void MyPlayerObject::updatePlayerRollFrame(int frame) {
    PlayerObject::updatePlayerRollFrame(frame);
    updatePlayerShaders();
}

void MyPlayerObject::updatePlayerBirdFrame(int frame) {
    PlayerObject::updatePlayerBirdFrame(frame);
    updatePlayerShaders();
}

void MyPlayerObject::updatePlayerDartFrame(int frame) {
    PlayerObject::updatePlayerDartFrame(frame);
    updatePlayerShaders();
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

void MyPlayerObject::createRobot(int frame) {
    PlayerObject::createRobot(frame);
    replaceBatchWithNode(m_robotBatchNode);

    if (m_robotSprite && m_robotSprite->m_paSprite) {
        for (auto part : m_robotSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            alpha::fine_outline::updateSprite(part);
        }
    }
}

void MyPlayerObject::createSpider(int frame) {
    PlayerObject::createSpider(frame);
    replaceBatchWithNode(m_spiderBatchNode);

    if (m_spiderSprite && m_spiderSprite->m_paSprite) {
        for (auto part : m_spiderSprite->m_paSprite->m_spriteParts->asExt<CCSpritePart>()) {
            alpha::fine_outline::updateSprite(part);
        }
    }
}

void MyPlayerObject::updatePlayerSwingFrame(int frame) {
    PlayerObject::updatePlayerSwingFrame(frame);
    updatePlayerShaders();
}

void MyPlayerObject::updatePlayerJetpackFrame(int frame) {
    PlayerObject::updatePlayerJetpackFrame(frame);
    updatePlayerShaders();
}