#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <wx/colour.h>
#include <wx/window.h>

#include "TaskPanel.h"

// Questa classe segue il pattern del Singleton. Penso sia meglio
// ci sia una singola e unica istanza accessibile da
// un qualsiasi punto del programma, evitando la creazione di molteplici istanze e
// evitando di passare manualmente puntatori e istanze nelle varie funzioni.

struct ThemeColors {
    wxColour background;
    wxColour foreground;
    wxColour buttonBackground;
    wxColour buttonForeground;
    wxColour buttonSelected;
    wxColour buttonAdd;
    wxColour buttonDelete;
    wxColour checkMark;
};

class ThemeManager {
public:
    static ThemeManager& GetInstance();

    void SetDarkTheme(bool enable);
    void ApplyThemeWindow(wxWindow *w);

    const ThemeColors& GetCurrentTheme() const;

private:
    ThemeManager();
    void ApplyThemeToWindowRecursive(wxWindow* window, const ThemeColors& theme);

    bool isDarkTheme;
    ThemeColors lightTheme;
    ThemeColors darkTheme;
};


#endif //THEMEMANAGER_H
