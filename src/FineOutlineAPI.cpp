#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/FineOutline.hpp"
#include <Geode/Geode.hpp>
#include "FineOutline.hpp"

using namespace geode::prelude;

namespace alpha::fine_outline {

    namespace internal {
        void _setOutlineColorPlayerObject(PlayerObject* player, const cocos2d::ccColor3B& color) {
            impl::setOutlineColor(player, color);
        }

        cocos2d::ccColor3B _getOutlineColorPlayerObject(PlayerObject* player) {
            return impl::getOutlineColor(player);
        }

        void _updateOutlinePlayerObject(PlayerObject* player) {
            impl::updateOutline(player);
        }

        void _setOutlineColorSimplePlayer(SimplePlayer* player, const cocos2d::ccColor3B& color) {
            impl::setOutlineColor(player, color);
        }

        cocos2d::ccColor3B _getOutlineColorSimplePlayer(SimplePlayer* player) {
            return impl::getOutlineColor(player);
        }

        void _updateOutlineSimplePlayer(SimplePlayer* player) {
            impl::updateOutline(player);
        }
    }

    bool isSeparate() {
        return impl::isSeparate();
    }

    bool isEditingP2() {
        return impl::isEditingP2();
    }

    bool usesCustomColor(PlayerIcon player) {
        return impl::usesCustomColor(player);
    }

    void useCustomColor(PlayerIcon player, bool use) {
        impl::useCustomColor(player, use);
    }

    cocos2d::ccColor3B getCustomColor(PlayerIcon player) {
        return impl::getCustomColor(player);
    }

    void setCustomColor(PlayerIcon player, const cocos2d::ccColor3B& color) {
        impl::setCustomColor(PlayerIcon::ONE, color);
    }

    int getColorIndex(PlayerIcon player) {
        return impl::getColorIndex(player);
    }

    void setColorIndex(PlayerIcon player, int color) {
        impl::setColorIndex(player, color);
    }

    cocos2d::ccColor3B getColor(PlayerIcon player) {
        return impl::getColor(player);    
    }
}