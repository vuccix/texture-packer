#include <Window/Window.h>
#include "View.h"

int runApp() {
    const Window window(600, 255, "Texture Packer");
    View ui;

    while (!window.shouldClose()) {
        window.waitEvents();

        ui.draw();

        window.swapBuffers();
        window.clear();
    }

    return EXIT_SUCCESS;
}

#ifdef _WIN32

#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return runApp();
}

#else

int main() {
    return runApp();
}

#endif
