#include "ThemeManager.h"

#include "MainFrame.h"

ThemeManager &ThemeManager::GetInstance() {
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager() {
    isDarkTheme = true;

    // Tema chiaro
    lightTheme.background = wxColour(255, 255, 255);
    lightTheme.foreground = wxColour(0, 0, 0);
    lightTheme.buttonBackground = wxColour(230, 230, 230);
    lightTheme.buttonForeground = wxColour(0, 0, 0);
    lightTheme.buttonSelected = wxColour(200, 200, 255);
    lightTheme.buttonAdd = wxColour(100, 200, 100);
    lightTheme.buttonDelete = wxColour(255, 100, 100);

    // Tema scuro
    darkTheme.background = wxColour(45, 45, 45);
    darkTheme.foreground = wxColour(255, 255, 255);
    darkTheme.buttonBackground = wxColour(60, 60, 60);
    darkTheme.buttonForeground = wxColour(255, 255, 255);
    darkTheme.buttonSelected = wxColour(100, 100, 100);
    darkTheme.buttonAdd = wxColour(50, 150, 50);
    darkTheme.buttonDelete = wxColour(200, 50, 50);
}

void ThemeManager::SetDarkTheme(bool enable) {
    isDarkTheme = enable;
}

const ThemeColors &ThemeManager::GetCurrentTheme() const {
    return isDarkTheme ? darkTheme : lightTheme;
}

void ThemeManager::ApplyThemeToWindowRecursive(wxWindow *window, const ThemeColors &theme) {
    if (auto filePicker = wxDynamicCast(window, wxFilePickerCtrl)) {
        filePicker->SetOwnBackgroundColour(theme.background);
        filePicker->SetOwnForegroundColour(theme.foreground);

    } else if (auto staticBox = wxDynamicCast(window, wxStaticBox)) {
        staticBox->SetOwnBackgroundColour(theme.background);
        staticBox->SetOwnForegroundColour(theme.foreground);
        staticBox->Reparent(window->GetParent()); // forza il repaint altrimenti segue sempre il tema di sistema.

    } else if (auto item = wxDynamicCast(window, wxButton)) {
        if (item->GetId() == DELETE_BUTTON) {
            item->SetBackgroundColour(theme.buttonDelete);
        } else if (item->GetId() == ADD_TASK) {
            item->SetBackgroundColour(theme.buttonAdd);
        } else {
            if (item->GetId() != TASK_CHECKMARK)
                item->SetBackgroundColour(theme.buttonBackground);
        }

        item->SetForegroundColour(theme.buttonForeground);
    } else {
        window->SetBackgroundColour(theme.background);
        window->SetForegroundColour(theme.foreground);
    }

    window->Update();
    window->Refresh();

    auto children = window->GetChildren();
    for (auto child: children) {
        ApplyThemeToWindowRecursive(child, theme);
    }

}


void ThemeManager::ApplyThemeWindow(wxWindow *w) {
    auto frame = wxDynamicCast(w, MainFrame);
    if (!frame) return;

    const ThemeColors &theme = GetCurrentTheme();

    ApplyThemeToWindowRecursive(frame, theme);
}
