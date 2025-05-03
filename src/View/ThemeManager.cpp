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

void ThemeManager::SetDarkTheme(bool darkThemeEnabled) {
    isDarkTheme = darkThemeEnabled;
}

const ThemeColors &ThemeManager::GetCurrentTheme() const {
    return isDarkTheme ? darkTheme : lightTheme;
}
