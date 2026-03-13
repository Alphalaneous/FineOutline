#include <Geode/Geode.hpp>
#include "ShaderCache.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    if (Mod::get()->hasSavedValue("p1-color") && !Mod::get()->hasSavedValue("override-color")) {
        Mod::get()->setSavedValue("override-color", true);
    }

    alpha::fine_outline::loadShaders();
    listenForSettingChanges<bool>("alternative-shader", [](bool) {
        alpha::fine_outline::loadShaders();
    });
}

$on_game(TexturesLoaded) {
    ShaderCache::get()->clearShaders();
    alpha::fine_outline::loadShaders();
}