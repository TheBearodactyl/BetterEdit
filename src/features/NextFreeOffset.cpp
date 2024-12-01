
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <utils/NextFreeOffsetInput.hpp>
#include <utils/Warn.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/EffectGameObject.hpp>
#include <unordered_set>
#include <type_traits>

using namespace geode::prelude;

struct GroupIDSource final {
    using ValueType = short;

    static constexpr short MIN_VALUE = 1;
    static constexpr short MAX_VALUE = 9999;

    static void getUsedIDs(GameObject* obj, std::unordered_set<short>& used) {
        auto insertFromArray = [&used](auto* array, short count) {
            if (array && count > 0) {
                for (short i = 0; i < count; ++i) {
                    used.insert((*array)[i]);
                }
            }
        };

        insertFromArray(obj->m_groups, obj->m_groupCount);
        insertFromArray(obj->m_colorGroups, obj->m_colorGroupCount);
        insertFromArray(obj->m_opacityGroups, obj->m_opacityGroupCount);

        if (auto* eobj = typeinfo_cast<EffectGameObject*>(obj)) {
            if (eobj->m_centerGroupID != 0) {
                used.insert(eobj->m_centerGroupID);
            }

            if (eobj->m_targetGroupID != 0) {
                used.insert(eobj->m_targetGroupID);
            }
        }
    }
};

class $modify(SetGroupIDLayer) {
    $override
    bool init(GameObject* obj, CCArray* objs) {
        if (!SetGroupIDLayer::init(obj, objs))
            return false;
        
        auto nextFreeMenu = m_mainLayer->getChildByID("next-free-menu");
        auto nextFreeOffset = NextFreeOffsetInput<GroupIDSource>::create();
        nextFreeOffset->setID("next-free-offset-input"_spr);
        nextFreeMenu->addChild(nextFreeOffset);
        nextFreeMenu->updateLayout();

        return true;
    }

    $override
    void onNextGroupID1(CCObject*) {
        m_groupIDValue = NextFreeOffsetInput<GroupIDSource>::getNextFreeID();
        this->updateGroupIDLabel();

        // gammal allokerande version
        // auto _ = NextFreeOffsetInput<GroupIDAllocator>::addFakeObjects();
        // SetGroupIDLayer::onNextGroupID1(sender);
    }
};
