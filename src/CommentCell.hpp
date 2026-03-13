#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

class $modify(MyCommentCell, CommentCell) {
    void loadFromComment(GJComment* p0);
};