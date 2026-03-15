#include "CharacterColorPage.hpp"
#include "../FineOutline.hpp"

void MyCharacterColorPage::updateColor(const ccColor3B& c) {
    for (auto children : m_playerObjects->asExt()) {
        if (auto player = typeinfo_cast<SimplePlayer*>(children)) {
            alpha::fine_outline::impl::setOutlineColor(player, c);
        }
    }
}

void MyCharacterColorPage::moveToMenu(CCNode* menu, CCMenu* orig, std::string_view id) {
    if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(orig->getChildByID(id))) {
        auto dummyBtn = CCMenuItemSpriteExtra::create(CCSprite::create(), btn->m_pListener, btn->m_pfnSelector);
        dummyBtn->setID(btn->getID());
        dummyBtn->setVisible(false);
        btn->getParent()->addChild(dummyBtn);
        btn->removeFromParentAndCleanup(false);
        menu->addChild(btn);
    }
}

void MyCharacterColorPage::setOutlineSelectorPos(int color) {
    auto fields = m_fields.self();

    auto btn = static_cast<CCNode*>(m_colorButtons->objectForKey(color))->getParent();
    auto worldSpace = btn->convertToWorldSpace({0, 0});
    auto nodeSpace = m_mainLayer->convertToNodeSpace(worldSpace);
    fields->m_outlineSelector->setPosition(nodeSpace + btn->getScaledContentSize()/2);
}

bool MyCharacterColorPage::init() {
    if (!CharacterColorPage::init()) return false;
    
    auto fields = m_fields.self();

    if (typeinfo_cast<CustomCharacterColorPage*>(this)) {
        fields->m_disable = true;
        return true;
    }

    for (auto children : m_playerObjects->asExt<CCNode>()) {
        if (auto player = typeinfo_cast<SimplePlayer*>(children)) {
            alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor());
        }
    }

    fields->m_outlineSelector = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
    fields->m_outlineSelector->setColor({50, 50, 50});
    fields->m_outlineSelector->setScale(0.7f);
    fields->m_outlineSelector->setZOrder(1);
    fields->m_outlineSelector->setID("cursor-line"_spr);

    m_cursors->addObject(fields->m_outlineSelector);

    m_mainLayer->addChild(fields->m_outlineSelector);
    setOutlineSelectorPos(alpha::fine_outline::impl::getColorIndex());

    auto outlineColorSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    outlineColorSpr->setScale(0.65f);

    auto colorTabsMenu = CCMenu::create();
    colorTabsMenu->setID("color-tabs-menu"_spr);
    colorTabsMenu->setAnchorPoint({1.f, 1.f});
    if (auto bg = m_mainLayer->getChildByID("background")) {
        colorTabsMenu->setPosition(m_mainLayer->getContentSize()/2);
        colorTabsMenu->setPositionX(colorTabsMenu->getPositionX() + 214);
        colorTabsMenu->setPositionY(colorTabsMenu->getPositionY() + 142);
    }

    m_mainLayer->addChild(colorTabsMenu);

    auto rowLayout = RowLayout::create();
    rowLayout->setGap(0);
    rowLayout->setAutoScale(false);
    rowLayout->setAxisAlignment(AxisAlignment::End);
    rowLayout->ignoreInvisibleChildren(true);

    colorTabsMenu->setLayout(rowLayout);


    fields->m_outlineColorBtn = CCMenuItemSpriteExtra::create(outlineColorSpr, this, menu_selector(MyCharacterColorPage::onOutlineColor));
    fields->m_outlineColorBtn->setVisible(false);
    fields->m_outlineColorBtn->setID("outline-color-button"_spr);
    fields->m_outlineColorBtn->setColor(alpha::fine_outline::impl::getCustomColor());

    fields->m_customColorToggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(MyCharacterColorPage::onCustomColorToggle), 0.6f);
    fields->m_customColorToggle->setID("custom-color-toggle"_spr);
    fields->m_customColorToggle->setVisible(false);
    fields->m_customColorToggle->setPosition(m_glowToggler->getPosition());
    fields->m_customColorToggle->toggle(alpha::fine_outline::impl::usesCustomColor());

    if (auto buttonsMenu = typeinfo_cast<CCMenu*>(m_mainLayer->getChildByID("buttons-menu"))) {

        fields->m_outlineColorBtn->setPosition({m_glowToggler->getPosition().x + 73, m_glowToggler->getPosition().y});
        buttonsMenu->addChild(fields->m_outlineColorBtn);
        if (auto closeButton = typeinfo_cast<CCMenuItemSpriteExtra*>(buttonsMenu->getChildByID("close-button"))) {
            closeButton->m_pfnSelector = menu_selector(MyCharacterColorPage::onCloseH);
        }

        moveToMenu(colorTabsMenu, buttonsMenu, "col1-button");
        moveToMenu(colorTabsMenu, buttonsMenu, "col2-button");
        moveToMenu(colorTabsMenu, buttonsMenu, "glow-button");

        buttonsMenu->addChild(fields->m_customColorToggle);
    }

    auto greySpr = ButtonSprite::create("Line", 40, false, "bigFont.fnt", "GJ_button_04.png", 20, 0.4);
    greySpr->setScale(0.85f);
    auto greenSpr = ButtonSprite::create("Line", 40, false, "bigFont.fnt", "GJ_button_01.png", 20, 0.4);
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

    runAction(CallFuncExt::create([this, fields] {
        if (auto parent = getParent()) {
            if (parent->getID() != "GJGarageLayer") {
                fields->m_outlineTab->setVisible(false);
                fields->m_outlineTab->getParent()->updateLayout();
                fields->m_outlineSelector->setVisible(false);
            }
        }
    }));

    return true;
}

void MyCharacterColorPage::onCustomColorToggle(CCObject* sender) {
    bool toggled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    auto fields = m_fields.self();

    alpha::fine_outline::impl::useCustomColor(alpha::fine_outline::PlayerIcon::SELECTED, toggled);
    fields->m_outlineColorBtn->setVisible(toggled);

    updateColor(alpha::fine_outline::impl::getColor());
}

void MyCharacterColorPage::onOutlineColor(CCObject* sender) {
    auto fields = m_fields.self();

    auto colorPopup = geode::ColorPickPopup::create(alpha::fine_outline::impl::getCustomColor());
    colorPopup->setCallback([this, fields] (ccColor4B const& c) {

        if (alpha::fine_outline::impl::isSeparate()) {
            for (auto children : m_playerObjects->asExt<CCNode>()) {
                if (auto player = typeinfo_cast<SimplePlayer*>(children)) {
                    alpha::fine_outline::impl::setOutlineColor(player, ccColor3B{c.r, c.g, c.b});
                }
            }
            fields->m_outlineColorBtn->setColor(ccColor3B{c.r, c.g, c.b});
        }

        alpha::fine_outline::impl::setCustomColor(alpha::fine_outline::PlayerIcon::SELECTED, ccColor3B{c.r, c.g, c.b});
    });
    colorPopup->show();
}

void MyCharacterColorPage::showLocksForTab(int tab) {
    
    switch (tab) {
    case 0:
        for (auto [k, v] : m_colorButtons->asExt<int, ColorChannelSprite>()) {
            if (auto child = v->getChildByTag(100)) {
                child->setVisible(!GameManager::get()->isColorUnlocked(k, UnlockType::Col1));
            }
        }
        break;
    case 1:
    case 2:
        for (auto [k, v] : m_colorButtons->asExt<int, ColorChannelSprite>()) {
            if (auto child = v->getChildByTag(100)) {
                child->setVisible(!GameManager::get()->isColorUnlocked(k, UnlockType::Col2));
            }
        }
        break;
    case 3:
        for (auto [k, v] : m_colorButtons->asExt<int, ColorChannelSprite>()) {
            if (auto child = v->getChildByTag(100)) {
                child->setVisible(false);
            }
        }
        break;
    }
}

void MyCharacterColorPage::onMode(CCObject* sender) {
    CharacterColorPage::onMode(sender);
    auto fields = m_fields.self();
    if (fields->m_disable) return;

    m_colorMode = sender->getTag();
    fields->m_outlineColorBtn->setVisible(m_colorMode == 3 && alpha::fine_outline::impl::usesCustomColor());
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

void MyCharacterColorPage::onPlayerColor(cocos2d::CCObject* sender) {
    auto fields = m_fields.self();
    if (fields->m_disable) return CharacterColorPage::onPlayerColor(sender);

    if (m_colorMode == 3) {
        alpha::fine_outline::impl::setColorIndex(alpha::fine_outline::PlayerIcon::SELECTED, sender->getTag());

        setOutlineSelectorPos(sender->getTag());

        if (!alpha::fine_outline::impl::usesCustomColor()) {
            updateColor(GameManager::get()->colorForIdx(sender->getTag()));
        }
    }
    else {
        CharacterColorPage::onPlayerColor(sender);
    }
}

void MyCharacterColorPage::toggleShip(CCObject* sender) {
    CharacterColorPage::toggleShip(sender);
    auto fields = m_fields.self();
    if (fields->m_disable) return;

    for (auto children : m_playerObjects->asExt<CCNode>()) {
        if (children->getParent()->getID() == "ship-button") {
            if (auto player = typeinfo_cast<SimplePlayer*>(children)) {
                alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor());
            }
            return;
        }
    }
}

void MyCharacterColorPage::setColorOnGarage() {
    auto fields = m_fields.self();
    if (fields->m_disable) return;

    auto scene = CCDirector::get()->m_pRunningScene;
    if (auto garage = scene->getChildByType<GJGarageLayer>(0)) {
        if (garage->m_playerObject) {
            alpha::fine_outline::impl::setOutlineColor(garage->m_playerObject, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
        }
        if (auto player2 = typeinfo_cast<SimplePlayer*>(garage->getChildByID("player2-icon"))) {
            alpha::fine_outline::impl::setOutlineColor(player2, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
        }
    }
}

#ifndef GEODE_IS_ANDROID
void MyCharacterColorPage::keyBackClicked() {
    CharacterColorPage::keyBackClicked();
    setColorOnGarage();
}
#endif

void MyCharacterColorPage::onCloseH(CCObject* sender) {
    CharacterColorPage::onClose(sender);
    setColorOnGarage();
}