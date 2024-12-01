#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/binding/GameManager.hpp>

using namespace geode::prelude;

static bool isProbablyObjectString(std::string_view str) {
    // check if it starts with [0-9]+,
    if (str.empty()) return false;
    if (str[0] < '0' || str[0] > '9') return false;

    auto comma_pos = str.find(',');

    return comma_pos != std::string_view::npos &&
        std::all_of(str.begin(), str.begin() + (long long)comma_pos + 1,
            [](char c) {
                return (c >= '0' && c <= '9') || c == ',';
            }
        );
}

class $modify(EditorUI) {
    // Hook these instead of copyObjects and pasteObjects so something like 
    // onDuplicate doesn't get overwritten
    void doCopyObjects(bool idk) {
        EditorUI::doCopyObjects(idk);
        if (Mod::get()->template getSettingValue<bool>("copy-paste-from-clipboard")) {
            clipboard::write(GameManager::get()->m_editorClipboard);
        }
    }
    void doPasteObjects(bool idk) {
        if (
            Mod::get()->template getSettingValue<bool>("copy-paste-from-clipboard") &&
            isProbablyObjectString(clipboard::read())
        ) {
            GameManager::get()->m_editorClipboard = clipboard::read();
            EditorUI::doPasteObjects(idk);
            Notification::create("Pasted Objects from Clipboard", NotificationIcon::Info)->show();
        }
        else {
            EditorUI::doPasteObjects(idk);
        }
    }
};
