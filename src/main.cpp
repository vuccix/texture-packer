#include <Window/Window.h>
#include "View.h"

int main() {
    const Window window(600, 300, "Texture Packer");
    View ui;

    while (!window.shouldClose()) {
        window.waitEvents();

        ui.draw();

        window.swapBuffers();
        window.clear();
    }
}
