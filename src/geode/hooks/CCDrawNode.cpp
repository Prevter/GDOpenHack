#include "../pch.hpp"

#include <Geode/modify/CCDrawNode.hpp>
#include "../../shared/hacks/hitboxes/hitboxes.hpp"

namespace openhack::hooks {
    struct CCDrawNodeHook : geode::Modify<CCDrawNodeHook, cocos2d::CCDrawNode> {
        bool drawPolygon(cocos2d::CCPoint *verts, unsigned int count, const cocos2d::ccColor4F &fillColor,
                         float borderWidth, const cocos2d::ccColor4F &borderColor) {
            hacks::Hitboxes::modifyDraw((gui::Color &) fillColor, borderWidth, (gui::Color &) borderColor);
            return cocos2d::CCDrawNode::drawPolygon(verts, count, fillColor, borderWidth, borderColor);
        }

        bool drawCircle(cocos2d::CCPoint const &position, float radius, cocos2d::_ccColor4F const &color,
                        float borderWidth, cocos2d::_ccColor4F const &borderColor, unsigned int segments) {
            hacks::Hitboxes::modifyDraw((gui::Color &) color, borderWidth, (gui::Color &) borderColor);
            return cocos2d::CCDrawNode::drawCircle(position, radius, color, borderWidth, borderColor, segments);
        }
    };
}