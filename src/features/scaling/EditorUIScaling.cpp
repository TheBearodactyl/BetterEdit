#include <Geode/Geode.hpp>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/binding/EditorUI.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <utils/Pro.hpp>

using namespace geode::prelude;

class $modify(ScaledUI, EditorUI) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("EditorUI::init", -100);
    }

    $override
    bool init(LevelEditorLayer* p0) {
        if (!EditorUI::init(p0))
            return false;

        // todo: pro compatability
        
        float scale = Mod::get()->getSettingValue<double>("scale-factor");
        auto size = CCDirector::get()->getWinSize();

        if (auto slider = getChildOfType<Slider>(this, 0)) {
            slider->ignoreAnchorPointForPosition(false);
            slider->setContentSize(ccp(0, 0));
            slider->setScale(scale);
            slider->setPositionY(size.height - (size.height - slider->getPositionY()) * scale);
        }
        
        if (auto settingsMenu = this->getChildByID("settings-menu")) {
            settingsMenu->setPosition(size);
            settingsMenu->setAnchorPoint(ccp(1, 1));
            settingsMenu->setScale(scale);
        }

        if (auto buttonsMenu = this->getChildByID("editor-buttons-menu")) {
            buttonsMenu->setPositionX((buttonsMenu->getPositionX() + buttonsMenu->getContentSize().width / 2));
            buttonsMenu->setAnchorPoint(ccp(1, 0.5f));
            buttonsMenu->setScale(scale);

            if (auto layerMenu = this->getChildByID("layer-menu")) {
                layerMenu->setPositionX(layerMenu->getPositionX() + layerMenu->getContentSize().width / 2);
                layerMenu->setPositionY(buttonsMenu->getPositionY() - (buttonsMenu->getPositionY() - layerMenu->getPositionY()) * scale);
                layerMenu->setAnchorPoint(ccp(1, 0.5f));
                layerMenu->setScale(scale);
            }
        }

        if (auto undoMenu = this->getChildByID("undo-menu")) {
            undoMenu->setPosition(ccp((undoMenu->getPositionX() - undoMenu->getContentSize().width / 2) * scale, size.height));
            undoMenu->setAnchorPoint(ccp(0, 1));
            undoMenu->setScale(scale);
        }

        if (auto testMenu = this->getChildByID("playtest-menu")) {
            testMenu->setPosition(ccp((testMenu->getPositionX() - testMenu->getContentSize().width / 2) * scale, testMenu->getPositionY()));
            testMenu->setAnchorPoint(ccp(0, 0.5f));
            testMenu->setScale(scale);

            if (auto playMenu = this->getChildByID("playback-menu")) {
                playMenu->setPosition(ccp((playMenu->getPositionX() - playMenu->getContentSize().width / 2) * scale, testMenu->getPositionY() + (playMenu->getPositionY() - testMenu->getPositionY()) * scale));
                playMenu->setAnchorPoint(ccp(0, 0.5f));
                playMenu->setScale(scale);
            }

            if (auto zoomMenu = this->getChildByID("zoom-menu")) {
                zoomMenu->setPosition(ccp((zoomMenu->getPositionX() - zoomMenu->getContentSize().width / 2) * scale, testMenu->getPositionY() + (zoomMenu->getPositionY() - testMenu->getPositionY()) * scale));
                zoomMenu->setAnchorPoint(ccp(0, 0.5f));
                zoomMenu->setScale(scale);
            }

            if (auto linkMenu = this->getChildByID("link-menu")) {
                linkMenu->setPosition(ccp((linkMenu->getPositionX() - linkMenu->getContentSize().width / 2) * scale, testMenu->getPositionY() + (linkMenu->getPositionY() - testMenu->getPositionY()) * scale));
                linkMenu->setAnchorPoint(ccp(0, 0.5f));
                linkMenu->setScale(scale);
            }
        }

        // i am very sorry for using object indexes but no id
        // doggo for the love of god at least use getChildOfType

        if (auto objBG = getChildOfType<CCSprite>(this, 0)) {
            objBG->setScaleY(scale);
        }

        if (auto objTabs = this->getChildByID("build-tabs-menu")) {
            objTabs->setAnchorPoint(ccp(0.5f, 0));
            objTabs->setPositionY(objTabs->getPositionY() - objTabs->getContentSize().height / 2);
            objTabs->setPositionY(objTabs->getPositionY() * scale);
            objTabs->setPositionY(objTabs->getPositionY() - 1);
            
            if (Mod::get()->getSettingValue<bool>("scale-build-tabs")) {
                objTabs->setScale(scale);
            }

            m_toolbarHeight = objTabs->getPositionY();
        }

        if (auto leftTabs = this->getChildByID("toolbar-categories-menu")) {
            leftTabs->setAnchorPoint(ccp(0, 0));
            leftTabs->setPosition(ccp(leftTabs->getPositionX() - leftTabs->getContentSize().width / 2, 0));
            leftTabs->setScale(scale);
        }

        if (auto rightTabs = this->getChildByID("toolbar-toggles-menu")) {
            rightTabs->setAnchorPoint(ccp(1, 0));
            rightTabs->setPosition(ccp(rightTabs->getPositionX() + rightTabs->getContentSize().width / 2, 0));
            rightTabs->setScale(scale);
        }

        if (auto leftLine = getChildOfType<CCSprite>(this, 1)) {
            leftLine->setScale(scale);
            leftLine->setPosition(leftLine->getPosition() * scale);
        }

        if (auto rightLine = getChildOfType<CCSprite>(this, 2)) {
            rightLine->setScale(scale);
            rightLine->setPosition(ccp(size.width - (size.width - rightLine->getPositionX()) * scale, rightLine->getPositionY() * scale));
        }

        if (auto deleteTabs = this->getChildByID("delete-category-menu")) {
            deleteTabs->setContentSize(ccp(0, 0));
            deleteTabs->setPositionY(deleteTabs->getPositionY() * scale);
            deleteTabs->setScale(scale);
        }

        // The EditButtonBar::loadFromItems hook below makes this work!
        for (auto c : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto bar = typeinfo_cast<EditButtonBar*>(c)) {
                bar->setScale(scale);
            }
        }

        // Make the builds tabs be center-aligned
        auto winSize = CCDirector::get()->getWinSize();
        this->getChildByID("build-tabs-menu")->setPositionX(winSize.width / 2);

        // Reload EditButtonBars to recenter
        for (auto c : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto bar = typeinfo_cast<EditButtonBar*>(c)) {
                bar->reloadItems(
                    GameManager::get()->getIntGameVariable("0049"),
                    GameManager::get()->getIntGameVariable("0050")
                );
            }
        }

        return true;
    }
};

class $modify(BetterEditButtonBar, EditButtonBar) {

    $override
    void loadFromItems(CCArray* items, int r, int c, bool unkBool) {

        EditButtonBar::loadFromItems(items, r, c, unkBool);

        //fix visible pages when opening editor, can be assumed as 0 as loadFromItems resets the page to 0
        for (auto barPages : CCArrayExt<CCNode*>(m_pagesArray)) {
            barPages->setVisible(false);
        }
        if (CCNode* firstPage = typeinfo_cast<CCNode*>(m_pagesArray->objectAtIndex(0))){
            firstPage->setVisible(true);
        }

        if (auto ui = typeinfo_cast<EditorUI*>(getParent())) {
            auto winSize = CCDirector::get()->getWinSize();

            setPositionX(winSize.width / 2);

            if (auto scrollLayer = getChildOfType<BoomScrollLayer>(this, 0)) {
                scrollLayer->setPositionX(-winSize.width / 2 + 5);
            }

            if (auto menu = getChildOfType<CCMenu>(this, 0)) {
                menu->setVisible(false);
            
                //easier to create a new menu than work with the old one
                CCMenu* navMenu = CCMenu::create();

                navMenu->setPosition({-winSize.width / 2, 0});
                navMenu->setContentSize(menu->getContentSize());
                navMenu->setScale(menu->getScale());

                float xOffset = 180 / getScale();
                float yOffset = 2;

                CCSprite* prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
                prevSpr->setScale(0.6f);
                CCSprite* nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
                nextSpr->setFlipX(true);
                nextSpr->setScale(0.6f);

                CCMenuItemSpriteExtra* prevButton = CCMenuItemSpriteExtra::create(prevSpr, this, menu_selector(EditButtonBar::onLeft));
                CCMenuItemSpriteExtra* nextButton = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(EditButtonBar::onRight));

                prevButton->setPositionX(menu->getContentWidth()/2 - xOffset);
                prevButton->setPositionY((ui->m_toolbarHeight/2 + yOffset) / getScale());
                
                nextButton->setPositionX(menu->getContentWidth()/2 + xOffset);
                nextButton->setPositionY((ui->m_toolbarHeight/2 + yOffset) / getScale());

                navMenu->addChild(prevButton);
                navMenu->addChild(nextButton);

                addChild(navMenu);
            }
        }
    }
};