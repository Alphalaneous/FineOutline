#include <Geode/Geode.hpp>
#include "FineOutline.hpp"

using namespace geode::prelude;

$on_game(TexturesLoaded) {
    alpha::fine_outline::shaders::reloadShaders();
}