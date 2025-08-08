#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/SimplePlayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/CharacterColorPage.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/GJScoreCell.hpp>
#include "ShaderCache.h"
#include "DummyCCSpriteBatchNode.h"

using namespace geode::prelude;

void loadShaders() {
    bool alt = Mod::get()->getSettingValue<bool>("alternative-shader");

    std::string fragIcon = R"(
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
            c.rgb = mix(c.rgb, vec3(1.0), float(br < 1.0 && c.a > 0.0 && gr > 0.0));

            gl_FragColor = v_fragmentColor * c;
        }
    )";
    if (alt) {
        fragIcon = R"(
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
    }

    ShaderCache::get()->createShader("icon", fragIcon);

    std::string fragOutline = R"(
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
    if (alt) {
        fragOutline = R"(
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
    }

    ShaderCache::get()->createShader("outline", fragOutline);
};

$on_mod(Loaded) {

    if (Mod::get()->hasSavedValue("p1-color") && !Mod::get()->hasSavedValue("override-color")) {
        Mod::get()->setSavedValue("override-color", true);
    }

    loadShaders();
    listenForSettingChanges("alternative-shader", [](bool) {
        loadShaders();
    });
}

class $modify(MyGameManager, GameManager) {

    void reloadAllStep5() {
        GameManager::reloadAllStep5();
        ShaderCache::get()->clearShaders();
        loadShaders();
    }
};

void removeShaders(CCSprite* spr) {
    spr->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    spr->getShaderProgram()->setUniformsForBuiltins();
    spr->getShaderProgram()->use();
    spr->setCascadeOpacityEnabled(false);
    spr->removeChildByID("black_outline"_spr);
}

void updateSprite(CCSprite* spr, ccColor3B color = {0, 0, 0}) {
    if (!spr || color == ccColor3B{0, 0, 0}) return;

    bool alt = Mod::get()->getSettingValue<bool>("alternative-shader");

    spr->setCascadeOpacityEnabled(true);

    CCSprite* blackOutline = CCSprite::createWithSpriteFrame(spr->displayFrame());

    blackOutline->setContentSize(spr->getContentSize());
    blackOutline->setID("black_outline"_spr);
    blackOutline->setPosition(spr->getContentSize()/2);
    blackOutline->setColor(color);

    if (CCGLProgram* prgOutline = ShaderCache::get()->getProgram("outline")) {
        prgOutline->setUniformsForBuiltins();
        blackOutline->setShaderProgram(prgOutline);
        prgOutline->use();
        if (!alt)
            blackOutline->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    }

    if (CCGLProgram* progIcon = ShaderCache::get()->getProgram("icon")) {
        progIcon->setUniformsForBuiltins();
        spr->setShaderProgram(progIcon);
        progIcon->use();
        if (!alt)
            spr->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    }

    spr->removeChildByID("black_outline"_spr);
    spr->addChild(blackOutline);
}

class $modify(MySimplePlayer, SimplePlayer) {

    struct Fields {
        bool m_isGlobedSelf = false;
        bool m_isShaderSpr = false;
        bool m_shaderSprDual = false;
    };

    void removeAllShaders() {
        auto fields = m_fields.self();

        fields->m_isShaderSpr = false;
        removeShaders(m_firstLayer);

        if (m_robotSprite && m_robotSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_robotSprite->m_paSprite->m_spriteParts)) {
                removeShaders(part);
            }
        }
        if (m_spiderSprite && m_spiderSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_spiderSprite->m_paSprite->m_spriteParts)) {
                removeShaders(part);
            }
        }
    }

    void setOutlineColor(const ccColor3B& color, bool dual) {

        if (color == ccColor3B{0, 0, 0}) {
            removeAllShaders();
            return;
        }

        if (CCSprite* blackOutline = typeinfo_cast<CCSprite*>(m_firstLayer->getChildByID("black_outline"_spr))) {
            blackOutline->setColor(color);
        }
        else {
            updatePlayerShaders(dual);
        }

        if (m_robotSprite && m_robotSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_robotSprite->m_paSprite->m_spriteParts)) {
                if (CCSprite* blackOutline = typeinfo_cast<CCSprite*>(part->getChildByID("black_outline"_spr))) {
                    blackOutline->setColor(color);
                }
            }
        }
        if (m_spiderSprite && m_spiderSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_spiderSprite->m_paSprite->m_spriteParts)) {
                if (CCSprite* blackOutline = typeinfo_cast<CCSprite*>(part->getChildByID("black_outline"_spr))) {
                    blackOutline->setColor(color);
                }
            }
        }
    }

    void updatePlayerShaders(bool dual) {
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
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_robotSprite->m_paSprite->m_spriteParts)) {
                updateSprite(part, outlineColor);
            }
        }
        if (m_spiderSprite && m_spiderSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_spiderSprite->m_paSprite->m_spriteParts)) {
                updateSprite(part, outlineColor);
            }
        }
        updateSprite(m_firstLayer, outlineColor);
        
    }

    void updatePlayerFrame(int p0, IconType p1) {
        SimplePlayer::updatePlayerFrame(p0, p1);
        auto fields = m_fields.self();

        if (fields->m_isShaderSpr) {
            updatePlayerShaders(fields->m_shaderSprDual);
        }
    }
};

class $modify(MyPlayerObject, PlayerObject) {

    static void onModify(auto& self) {
        (void)self.setHookPriority("PlayerObject::playSpawnEffect", -2);
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerShipFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerRollFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerBirdFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerDartFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::createRobot", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::createSpider", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerSwingFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerJetpackFrame", "hiimjustin000.more_icons");
    }

    void updatePlayerShaders() {

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

        updateSprite(m_iconSprite, outlineColor);
        updateSprite(m_vehicleSprite, outlineColor);
        updateSprite(m_birdVehicle, outlineColor);
        if (m_robotSprite && m_robotSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_robotSprite->m_paSprite->m_spriteParts)) {
                updateSprite(part, outlineColor);
            }
        }
        if (m_spiderSprite && m_spiderSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_spiderSprite->m_paSprite->m_spriteParts)) {
                updateSprite(part, outlineColor);
            }
        }
    }

    bool init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool highGraphics) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, highGraphics)) return false;
        queueInMainThread([this] {
            updatePlayerShaders();
        });
        return true;
    }

    void updatePlayerFrame(int frame) {
        PlayerObject::updatePlayerFrame(frame);
        updatePlayerShaders();
    }

    void updatePlayerShipFrame(int frame) {
        PlayerObject::updatePlayerShipFrame(frame);
        updatePlayerShaders();
    }

    void updatePlayerRollFrame(int frame) {

        PlayerObject::updatePlayerRollFrame(frame);
        updatePlayerShaders();
    }

    void updatePlayerBirdFrame(int frame) {
        PlayerObject::updatePlayerBirdFrame(frame);
        updatePlayerShaders();
    }

    void updatePlayerDartFrame(int frame) {
        PlayerObject::updatePlayerDartFrame(frame);
        updatePlayerShaders();
    }

    void* getCustomVTablePtr() {
        static void* vtable = []() -> void* {
            DummyCCSpriteBatchNode node;
            return *(void**)&node;
        }();
        return vtable;
    }

    void replaceBatchWithNode(CCSpriteBatchNode* batchNode) {
        DummyCCSpriteBatchNode node;
        *reinterpret_cast<void**>(batchNode) = getCustomVTablePtr();
    }

    void createRobot(int frame) {
        PlayerObject::createRobot(frame);
        replaceBatchWithNode(m_robotBatchNode);

        if (m_robotSprite && m_robotSprite->m_paSprite) {
            for (CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_robotSprite->m_paSprite->m_spriteParts)) {
                updateSprite(part);
            }
        }
    }

    void createSpider(int frame) {
        PlayerObject::createSpider(frame);
        replaceBatchWithNode(m_spiderBatchNode);

        if (m_spiderSprite && m_spiderSprite->m_paSprite) {
            for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_spiderSprite->m_paSprite->m_spriteParts)) {
                updateSprite(part);
            }
        }
    }

    void updatePlayerSwingFrame(int frame) {
        PlayerObject::updatePlayerSwingFrame(frame);
        updatePlayerShaders();
    }

    void updatePlayerJetpackFrame(int frame) {
        PlayerObject::updatePlayerJetpackFrame(frame);
        updatePlayerShaders();
    }
};


class GlobedSimplePlayer : public CCNode {};

class $modify(MyPlayLayer, PlayLayer) {

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        schedule(schedule_selector(MyPlayLayer::checkGlobed));
        return true;
    }

    void checkGlobed(float dt) {
        if (CCNode* wrapper = m_progressBar->getChildByID("dankmeme.globed2/progress-bar-wrapper")) {
            if (CCNode* progressIcon = wrapper->getChildByID("dankmeme.globed2/self-player-progress")) {
                if (GlobedSimplePlayer* globedSimplePlayer = progressIcon->getChildByType<GlobedSimplePlayer>(0)) {
                    if (SimplePlayer* player = globedSimplePlayer->getChildByType<SimplePlayer>(0)) {
                        static_cast<MySimplePlayer*>(player)->updatePlayerShaders(false);
                        unschedule(schedule_selector(MyPlayLayer::checkGlobed));
                    }
                }
            }
        }
    }
};

class $modify(MyProfilePage, ProfilePage) {
    struct Fields {
        SEL_MenuHandler m_playerToggle;
        SEL_MenuHandler m_shipToggle;
    };

    void getUserInfoFinished(GJUserScore* p0) {
        ProfilePage::getUserInfoFinished(p0);

        auto fields = m_fields.self();

        if (m_ownProfile) {
            auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
            auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
            CCNode* playerMenu = m_mainLayer->getChildByID("player-menu");
            for (CCNode* menuChild : CCArrayExt<CCNode*>(playerMenu->getChildren())) {
                if (SimplePlayer* player = menuChild->getChildByType<SimplePlayer>(0)) {
                    static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
                }
                if (CCNode* innerNode = menuChild->getChildByType<CCNode>(0)) {
                    if (SimplePlayer* player = innerNode->getChildByType<SimplePlayer>(0)) {
                        static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
                    }
                }
            }

            if (auto twoPToggler = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("left-menu")->getChildByID("2p-toggler"))) {
                fields->m_playerToggle = twoPToggler->m_pfnSelector;
                twoPToggler->m_pfnSelector = menu_selector(MyProfilePage::on2PToggle);
            }

            if (Loader::get()->isModLoaded("weebify.separate_dual_icons") && !Loader::get()->isModLoaded("rynat.better_unlock_info")) {
                if (auto shipToggler = static_cast<CCMenuItemSpriteExtra*>(playerMenu->getChildByID("player-ship"))) {
                    fields->m_shipToggle = shipToggler->m_pfnSelector;
                    shipToggler->m_pfnSelector = menu_selector(MyProfilePage::onShipToggle);
                }
            }
        }
    }

    void toggleShip(CCObject* sender) {
        ProfilePage::toggleShip(sender);
        if (m_ownProfile) {
            CCNode* playerMenu = m_mainLayer->getChildByID("player-menu");
            if (CCNode* shipNode = playerMenu->getChildByID("player-ship")) {
                if (SimplePlayer* player = shipNode->getChildByType<SimplePlayer>(0)) {
                    auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
                    static_cast<MySimplePlayer*>(player)->updatePlayerShaders(sdi && sdi->getSavedValue<bool>("2pselected"));
                }
            }
        }
    }

    void on2PToggle(CCObject* sender) {
        auto fields = m_fields.self();

        (this->*fields->m_playerToggle)(sender);

        if (m_ownProfile) {
            auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
            auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
            CCNode* playerMenu = m_mainLayer->getChildByID("player-menu");
            for (CCNode* menuChild : CCArrayExt<CCNode*>(playerMenu->getChildren())) {
                if (SimplePlayer* player = menuChild->getChildByType<SimplePlayer>(0)) {
                    static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
                }
                if (CCNode* innerNode = menuChild->getChildByType<CCNode>(0)) {
                    if (SimplePlayer* player = innerNode->getChildByType<SimplePlayer>(0)) {
                        static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
                    }
                }
            }
        }
    }

    void onShipToggle(CCObject* sender) {
        auto fields = m_fields.self();

        (this->*fields->m_shipToggle)(sender);
        if (m_ownProfile) {
            CCNode* playerMenu = m_mainLayer->getChildByID("player-menu");
            if (CCNode* shipNode = playerMenu->getChildByID("player-ship")) {
                if (SimplePlayer* player = shipNode->getChildByType<SimplePlayer>(0)) {
                    auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
                    static_cast<MySimplePlayer*>(player)->updatePlayerShaders(sdi && sdi->getSavedValue<bool>("2pselected"));
                }
            }
        }
    }
};

class $modify(MyGJGarageLayer, GJGarageLayer) {

    static void onModify(auto& self) {
        (void)self.setHookPriority("GJGarageLayer::init", -2);
        (void)self.setHookPriority("GJGarageLayer::onSelect", -2);\
    }

    bool init() {
        if (!GJGarageLayer::init()) return false;

        if (m_playerObject) {
            static_cast<MySimplePlayer*>(m_playerObject)->updatePlayerShaders(false);
        }
        if (SimplePlayer* player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
            static_cast<MySimplePlayer*>(player2)->updatePlayerShaders(true);
        }

        return true;
    }

    void onSelect(cocos2d::CCObject* sender) {
        GJGarageLayer::onSelect(sender);
        if (m_playerObject) {
            static_cast<MySimplePlayer*>(m_playerObject)->updatePlayerShaders(false);
        }
        if (SimplePlayer* player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
            static_cast<MySimplePlayer*>(player2)->updatePlayerShaders(true);
        }
    }

};

class OutlineColorPickPopupDelegate : public ColorPickPopupDelegate {
    
    Ref<CCArray> m_icons;
    Ref<CCMenuItemSpriteExtra> m_colorBtn;
    bool m_dual = false;
    public:
    void init(CCArray* icons, CCMenuItemSpriteExtra* btn, bool dual) {
        m_icons = icons;
        m_dual = dual;
        m_colorBtn = btn;
    }

    void updateColor(ccColor4B const& c) {

        if (Mod::get()->getSavedValue<bool>("override-color")) {
            for (CCNode* children : CCArrayExt<CCNode*>(m_icons)) {
                if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(children)) {
                    static_cast<MySimplePlayer*>(player)->setOutlineColor(ccColor3B{c.r, c.g, c.b}, m_dual);
                }
            }
            m_colorBtn->setColor(ccColor3B{c.r, c.g, c.b});
        }

        Mod::get()->setSavedValue<ccColor3B>(m_dual ? "p2-color" : "p1-color", ccColor3B{c.r, c.g, c.b});
    }
};

class $modify(MyCharacterColorPage, CharacterColorPage) {

    static void onModify(auto& self) {
        (void)self.setHookPriority("CharacterColorPage::init", -2);
        (void)self.setHookPriority("CharacterColorPage::toggleShip", -2);
    }

    struct Fields {
        CCMenuItemSpriteExtra* m_outlineColorBtn;
        OutlineColorPickPopupDelegate* m_outlineColorDelegate;
        CCMenuItemToggler* m_outlineTab;
        CCMenuItemToggler* m_customColorToggle;
        CCSprite* m_outlineSelector;
        CCLabelBMFont* m_customColorLabel;
        bool m_disable = false;
        ~Fields() {
            delete m_outlineColorDelegate;
        }
    };

    void updateColor(ccColor3B const& c, bool dual) {
        for (CCNode* children : CCArrayExt<CCNode*>(m_playerObjects)) {
            if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(children)) {
                static_cast<MySimplePlayer*>(player)->setOutlineColor(c, dual);
            }
        }
    }

    void moveToMenu(CCNode* menu, CCMenu* orig, std::string id) {
        if (CCMenuItemSpriteExtra* btn = typeinfo_cast<CCMenuItemSpriteExtra*>(orig->getChildByID(id.c_str()))) {
            CCMenuItemSpriteExtra* dummyBtn = CCMenuItemSpriteExtra::create(CCSprite::create(), btn->m_pListener, btn->m_pfnSelector);
            dummyBtn->setID(btn->getID());
            dummyBtn->setVisible(false);
            btn->getParent()->addChild(dummyBtn);
            btn->removeFromParentAndCleanup(false);
            menu->addChild(btn);
        }
    }

    void setOutlineSelectorPos(int color) {
        auto fields = m_fields.self();

        CCNode* btn = static_cast<CCNode*>(m_colorButtons->objectForKey(color))->getParent();
        CCPoint worldSpace = btn->convertToWorldSpace({0, 0});
        CCPoint nodeSpace = m_mainLayer->convertToNodeSpace(worldSpace);
        fields->m_outlineSelector->setPosition(nodeSpace + btn->getScaledContentSize()/2);
    }

    bool init() {
        if (!CharacterColorPage::init()) return false;
        
        auto fields = m_fields.self();

        if (getID() == "rooot.custom-gamemode-colors/gamemode-colors-page") {
            fields->m_disable = true;
            return true;
        }

        fields->m_outlineColorDelegate = new OutlineColorPickPopupDelegate();

        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
        for (CCNode* children : CCArrayExt<CCNode*>(m_playerObjects)) {
            if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(children)) {
                static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
            }
        }

        int outlineColor = Mod::get()->getSavedValue<int64_t>(dual ? "outline-color-p2" : "outline-color-p1", 15);

        fields->m_outlineSelector = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
        fields->m_outlineSelector->setColor({50, 50, 50});
        fields->m_outlineSelector->setScale(0.7f);
        fields->m_outlineSelector->setZOrder(1);
        fields->m_outlineSelector->setID("cursor-line"_spr);

        m_mainLayer->addChild(fields->m_outlineSelector);
        setOutlineSelectorPos(outlineColor);

        CCSprite* outlineColorSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        outlineColorSpr->setScale(0.65f);

        CCMenu* colorTabsMenu = CCMenu::create();
        colorTabsMenu->setID("color-tabs-menu"_spr);
        colorTabsMenu->setAnchorPoint({1.f, 1.f});
        if (CCNode* bg = m_mainLayer->getChildByID("background")) {
            colorTabsMenu->setPosition(m_mainLayer->getContentSize()/2);
            colorTabsMenu->setPositionX(colorTabsMenu->getPositionX() + 214);
            colorTabsMenu->setPositionY(colorTabsMenu->getPositionY() + 142);
        }

        m_mainLayer->addChild(colorTabsMenu);

        RowLayout* rowLayout = RowLayout::create();
        rowLayout->setGap(0);
        rowLayout->setAutoScale(false);
        rowLayout->setAxisAlignment(AxisAlignment::End);
        rowLayout->ignoreInvisibleChildren(true);

        colorTabsMenu->setLayout(rowLayout);

        fields->m_outlineColorBtn = CCMenuItemSpriteExtra::create(outlineColorSpr, this, menu_selector(MyCharacterColorPage::onOutlineColor));
        fields->m_outlineColorBtn->setVisible(false);
        fields->m_outlineColorBtn->setID("outline-color-button"_spr);
        fields->m_outlineColorBtn->setColor(Mod::get()->getSavedValue<ccColor3B>(dual ? "p2-color" : "p1-color"));

        fields->m_customColorToggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(MyCharacterColorPage::onCustomColorToggle), 0.6f);
        fields->m_customColorToggle->setID("custom-color-toggle"_spr);
        fields->m_customColorToggle->setVisible(false);
        fields->m_customColorToggle->setPosition(m_glowToggler->getPosition());
        fields->m_customColorToggle->toggle(Mod::get()->getSavedValue<bool>("override-color"));

        if (CCMenu* buttonsMenu = typeinfo_cast<CCMenu*>(m_mainLayer->getChildByID("buttons-menu"))) {

            fields->m_outlineColorBtn->setPosition({m_glowToggler->getPosition().x + 73, m_glowToggler->getPosition().y});
            buttonsMenu->addChild(fields->m_outlineColorBtn);
            if (CCMenuItemSpriteExtra* closeButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttonsMenu->getChildByID("close-button"))) {
                closeButton->m_pfnSelector = menu_selector(MyCharacterColorPage::onCloseH);
            }

            moveToMenu(colorTabsMenu, buttonsMenu, "col1-button");
            moveToMenu(colorTabsMenu, buttonsMenu, "col2-button");
            moveToMenu(colorTabsMenu, buttonsMenu, "glow-button");

            buttonsMenu->addChild(fields->m_customColorToggle);
        }

        ButtonSprite* greySpr = ButtonSprite::create("Line", 40, false, "bigFont.fnt", "GJ_button_04.png", 20, 0.4);
        greySpr->setScale(0.85f);
        ButtonSprite* greenSpr = ButtonSprite::create("Line", 40, false, "bigFont.fnt", "GJ_button_01.png", 20, 0.4);
        greenSpr->setScale(0.85f);

        fields->m_outlineTab = CCMenuItemToggler::create(greySpr, greenSpr, this, menu_selector(MyCharacterColorPage::onMode));
        fields->m_outlineTab->setContentSize(greySpr->getContentSize());
        fields->m_outlineTab->setTag(3);
        fields->m_outlineTab->setID("outline-button"_spr);
        colorTabsMenu->addChild(fields->m_outlineTab);
        
        colorTabsMenu->updateLayout();

        fields->m_customColorLabel = CCLabelBMFont::create("Custom", "bigFont.fnt");
        fields->m_customColorLabel->setAnchorPoint({0, 0.5f});
        fields->m_customColorLabel->setScale(0.3f);
        fields->m_customColorLabel->setPosition(m_glowLabel->getPosition());
        fields->m_customColorLabel->setVisible(false);

        m_mainLayer->addChild(fields->m_customColorLabel);
        
        fields->m_outlineTab->toggle(false);

        queueInMainThread([this, fields] {
            if (CCNode* parent = getParent()) {
                if (parent->getID() != "GJGarageLayer") {
                    fields->m_outlineTab->setVisible(false);
                    fields->m_outlineTab->getParent()->updateLayout();
                    fields->m_outlineSelector->setVisible(false);
                }
            }
        });

        return true;
    }

    void onCustomColorToggle(CCObject* sender) {
        bool toggled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
        auto fields = m_fields.self();
        
        Mod::get()->setSavedValue<bool>("override-color", toggled);
        fields->m_outlineColorBtn->setVisible(Mod::get()->getSavedValue<bool>("override-color"));

        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        auto dual = sdi && sdi->getSavedValue<bool>("2pselected");

        if (!Mod::get()->getSavedValue<bool>("override-color")) {
            updateColor(GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>(dual ? "outline-color-p2" : "outline-color-p1", 15)), dual);
        }
        else {
            updateColor(Mod::get()->getSavedValue<ccColor3B>(dual ? "p2-color" : "p1-color"), dual);
        }
    }

    void onOutlineColor(CCObject* sender) {
        auto fields = m_fields.self();

        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
        geode::ColorPickPopup* colorPopup = geode::ColorPickPopup::create(Mod::get()->getSavedValue<ccColor3B>(dual ? "p2-color" : "p1-color"));
        fields->m_outlineColorDelegate->init(m_playerObjects, fields->m_outlineColorBtn, dual);
        colorPopup->setDelegate(fields->m_outlineColorDelegate);
        colorPopup->show();
    }

    void showLocksForTab(int tab) {
        
        switch (tab) {
        case 0:
            for (auto obj : CCDictionaryExt<int, ColorChannelSprite*>(m_colorButtons)) {
                if (CCNode* child = obj.second->getChildByTag(100)) {
                    child->setVisible(!GameManager::get()->isColorUnlocked(obj.first, UnlockType::Col1));
                }
            }
            break;
        case 1:
        case 2:
            for (auto obj : CCDictionaryExt<int, ColorChannelSprite*>(m_colorButtons)) {
                if (CCNode* child = obj.second->getChildByTag(100)) {
                    child->setVisible(!GameManager::get()->isColorUnlocked(obj.first, UnlockType::Col2));
                }
            }
            break;
        case 3:
            for (auto obj : CCDictionaryExt<int, ColorChannelSprite*>(m_colorButtons)) {
                if (CCNode* child = obj.second->getChildByTag(100)) {
                    child->setVisible(false);
                }
            }
            break;
        }
    }

    void onMode(CCObject* sender) {
        CharacterColorPage::onMode(sender);
        auto fields = m_fields.self();
        if (fields->m_disable) return;

        m_colorMode = sender->getTag();
        fields->m_outlineColorBtn->setVisible(m_colorMode == 3 && Mod::get()->getSavedValue<bool>("override-color"));
        fields->m_customColorToggle->setVisible(m_colorMode == 3);
        fields->m_customColorLabel->setVisible(m_colorMode == 3);

        showLocksForTab(m_colorMode);
        
        if (m_colorMode != 3) {
            fields->m_outlineSelector->setColor({50, 50, 50});
            fields->m_outlineTab->toggle(false);
            fields->m_outlineTab->setClickable(true);
            fields->m_outlineSelector->setZOrder(1);
        }
        else {
            fields->m_outlineSelector->setColor({255, 255, 255});
            fields->m_outlineTab->toggle(true);
            fields->m_outlineTab->setClickable(false);
            fields->m_outlineSelector->setZOrder(11);
        }
    }

    void onPlayerColor(cocos2d::CCObject* sender) {
        auto fields = m_fields.self();
        if (fields->m_disable) return CharacterColorPage::onPlayerColor(sender);

        if (m_colorMode == 3) {
            auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
            auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
            Mod::get()->setSavedValue<int64_t>(dual ? "outline-color-p2" : "outline-color-p1", sender->getTag());
            setOutlineSelectorPos(sender->getTag());
            if (!Mod::get()->getSavedValue<bool>("override-color")) {
                updateColor(GameManager::get()->colorForIdx(sender->getTag()), dual);
            }
        }
        else {
            CharacterColorPage::onPlayerColor(sender);
        }
    }

    void toggleShip(CCObject* sender) {
        CharacterColorPage::toggleShip(sender);
        auto fields = m_fields.self();
        if (fields->m_disable) return;
        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
        for (CCNode* children : CCArrayExt<CCNode*>(m_playerObjects)) {
            if (children->getParent()->getID() == "ship-button") {
                if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(children)) {
                    static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
                }
                return;
            }
        }
    }

    void setColorOnGarage() {
        auto fields = m_fields.self();
        if (fields->m_disable) return;
        CCScene* scene = CCDirector::get()->m_pRunningScene;
        if (GJGarageLayer* garage = scene->getChildByType<GJGarageLayer>(0)) {
            if (garage->m_playerObject) {
                if (Mod::get()->getSavedValue<bool>("override-color")) {
                    static_cast<MySimplePlayer*>(garage->m_playerObject)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);	
                }
                else {
                    static_cast<MySimplePlayer*>(garage->m_playerObject)->setOutlineColor(GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>("outline-color-p1", 15)), false);
                }
            }
            if (SimplePlayer* player2 = typeinfo_cast<SimplePlayer*>(garage->getChildByID("player2-icon"))) {
                if (Mod::get()->getSavedValue<bool>("override-color")) {
                    static_cast<MySimplePlayer*>(player2)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p2-color"), true);
                }
                else {
                    static_cast<MySimplePlayer*>(player2)->setOutlineColor(GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>("outline-color-p2", 15)), true);
                }
            }
        }
    }

    #ifndef GEODE_IS_ANDROID
    void keyBackClicked() {
        CharacterColorPage::keyBackClicked();
        setColorOnGarage();
    }
    #endif

    void onCloseH(CCObject* sender) {
        CharacterColorPage::onClose(sender);
        setColorOnGarage();
    }
};

class $modify(MyCommentCell, CommentCell) {

    void loadFromComment(GJComment* p0) {
        CommentCell::loadFromComment(p0);

        if (p0->m_accountID == GJAccountManager::get()->m_accountID){
            if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
                if (Mod::get()->getSavedValue<bool>("override-color")) {
                    static_cast<MySimplePlayer*>(player)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);
                }
                else {
                    static_cast<MySimplePlayer*>(player)->setOutlineColor(GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>("outline-color-p1")), false);
                }

            }
        }
    }
};

class $modify(MyGJScoreCell, GJScoreCell) {

    void loadFromScore(GJUserScore* p0) {

        GJScoreCell::loadFromScore(p0);

        if (p0->m_accountID == GJAccountManager::get()->m_accountID){
            if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
                if (Mod::get()->getSavedValue<bool>("override-color")) {
                    static_cast<MySimplePlayer*>(player)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);
                }
                else {
                    static_cast<MySimplePlayer*>(player)->setOutlineColor(GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>("outline-color-p1")), false);
                }
            }
        }
    }
};