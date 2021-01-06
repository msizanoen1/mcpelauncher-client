#include "core_patches.h"

#include <mcpelauncher/linker.h>
#include <mcpelauncher/patch_utils.h>
#include <log.h>

std::shared_ptr<GameWindow> CorePatches::currentGameWindow;

void CorePatches::hideMousePointer() {
    currentGameWindow->setCursorDisabled(true);
}

void CorePatches::showMousePointer() {
    currentGameWindow->setCursorDisabled(false);
}

void CorePatches::install(void *handle) {
    void* ptr;
    ptr = linker::dlsym(handle, "_ZN11AppPlatform16hideMousePointerEv");
    PatchUtils::patchCallInstruction(ptr, (void*) &hideMousePointer, true);
    ptr = linker::dlsym(handle, "_ZN11AppPlatform16showMousePointerEv");
    PatchUtils::patchCallInstruction(ptr, (void*) &showMousePointer, true);

}

void CorePatches::setGameWindow(std::shared_ptr<GameWindow> gameWindow) {
    currentGameWindow = gameWindow;
}
