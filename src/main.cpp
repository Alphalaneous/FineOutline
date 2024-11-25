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
#include "CCSpriteBatchNode.h"
#include "ShaderCache.h"

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
		m_fields->m_isShaderSpr = false;
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
		m_fields->m_isShaderSpr = true;
		m_fields->m_shaderSprDual = dual;
		ccColor3B outlineColor = Mod::get()->getSavedValue<ccColor3B>(dual ? "p2-color" : "p1-color");

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

		if (m_fields->m_isShaderSpr) {
			updatePlayerShaders(m_fields->m_shaderSprDual);
		}
	}
};

class $modify(MyPlayerObject, PlayerObject) {

	static void onModify(auto& self) {
        (void)self.setHookPriority("PlayerObject::playSpawnEffect", -2);
    }

	void updatePlayerShaders() {

		if (!m_gameLayer || !(m_gameLayer->m_player1 == this || m_gameLayer->m_player2 == this)) return;

		ccColor3B outlineColor = Mod::get()->getSavedValue<ccColor3B>(
			Loader::get()->isModLoaded("weebify.separate_dual_icons") && m_gameLayer->m_player2 == this ? "p2-color" : "p1-color");

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

    void createRobot(int frame) {
        PlayerObject::createRobot(frame);

		static_cast<MyCCSpriteBatchNode*>(m_robotBatchNode)->setFake(true);

		if (m_robotSprite && m_robotSprite->m_paSprite) {
			for(CCSpritePart* part : CCArrayExt<CCSpritePart*>(m_robotSprite->m_paSprite->m_spriteParts)) {
				updateSprite(part);
			}
		}
    }

    void createSpider(int frame) {
        PlayerObject::createSpider(frame);

		static_cast<MyCCSpriteBatchNode*>(m_spiderBatchNode)->setFake(true);

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
				m_fields->m_playerToggle = twoPToggler->m_pfnSelector;
				twoPToggler->m_pfnSelector = menu_selector(MyProfilePage::on2PToggle);
			}

			if (Loader::get()->isModLoaded("weebify.separate_dual_icons") && !Loader::get()->isModLoaded("rynat.better_unlock_info")) {
				if (auto shipToggler = static_cast<CCMenuItemSpriteExtra*>(playerMenu->getChildByID("player-ship"))) {
					m_fields->m_shipToggle = shipToggler->m_pfnSelector;
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
		(this->*m_fields->m_playerToggle)(sender);

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
		(this->*m_fields->m_shipToggle)(sender);
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
	bool m_dual = false;
	public:
	void init(CCArray* icons, bool dual) {
		m_icons = icons;
		m_dual = dual;
	}

	void updateColor(ccColor4B const& c) {

		for (CCNode* children : CCArrayExt<CCNode*>(m_icons)) {
			if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(children)) {
				static_cast<MySimplePlayer*>(player)->setOutlineColor(ccColor3B{c.r, c.g, c.b}, m_dual);
			}
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

		~Fields() {
			delete m_outlineColorDelegate;
		}
	};

    bool init() {

		if (!CharacterColorPage::init()) return false;

 		m_fields->m_outlineColorDelegate = new OutlineColorPickPopupDelegate();

		auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
		auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
		for (CCNode* children : CCArrayExt<CCNode*>(m_playerObjects)) {
			if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(children)) {
				static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
			}
		}

		CCSprite* outlineColorSpr = CCSprite::createWithSpriteFrameName("GJ_paintBtn_001.png");
		outlineColorSpr->setScale(0.5);

		m_fields->m_outlineColorBtn = CCMenuItemSpriteExtra::create(outlineColorSpr, this, menu_selector(MyCharacterColorPage::onOutlineColor));
		m_fields->m_outlineColorBtn->setVisible(false);
		if (CCMenu* buttonsMenu = typeinfo_cast<CCMenu*>(m_mainLayer->getChildByID("buttons-menu"))) {

			m_fields->m_outlineColorBtn->setPosition({m_glowToggler->getPosition().x + 74, m_glowToggler->getPosition().y});
			buttonsMenu->addChild(m_fields->m_outlineColorBtn);
			if (CCMenuItemSpriteExtra* closeButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttonsMenu->getChildByID("close-button"))) {
				closeButton->m_pfnSelector = menu_selector(MyCharacterColorPage::onCloseH);
			}
		}
		
		return true;
	}

	void onOutlineColor(CCObject* sender) {
		auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
		auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
		geode::ColorPickPopup* colorPopup = geode::ColorPickPopup::create(Mod::get()->getSavedValue<ccColor3B>(dual ? "p2-color" : "p1-color"));
		m_fields->m_outlineColorDelegate->init(m_playerObjects, dual);
		colorPopup->setDelegate(m_fields->m_outlineColorDelegate);
		colorPopup->show();
	}

    void onMode(CCObject* sender) {
		CharacterColorPage::onMode(sender);
		m_fields->m_outlineColorBtn->setVisible(m_colorMode == 2);
	}


    void toggleShip(CCObject* sender) {
		CharacterColorPage::toggleShip(sender);
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
		CCScene* scene = CCDirector::get()->m_pRunningScene;
		if (GJGarageLayer* garage = scene->getChildByType<GJGarageLayer>(0)) {
			if (garage->m_playerObject) {
				static_cast<MySimplePlayer*>(garage->m_playerObject)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);
			}
			if (SimplePlayer* player2 = typeinfo_cast<SimplePlayer*>(garage->getChildByID("player2-icon"))) {
				static_cast<MySimplePlayer*>(player2)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p2-color"), true);
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
				static_cast<MySimplePlayer*>(player)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);
			}
		}
	}
};

class $modify(MyGJScoreCell, GJScoreCell) {

    void loadFromScore(GJUserScore* p0) {

		GJScoreCell::loadFromScore(p0);

		if (p0->m_accountID == GJAccountManager::get()->m_accountID){
			if (SimplePlayer* player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
				static_cast<MySimplePlayer*>(player)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);
			}
		}
	}
};