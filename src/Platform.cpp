//
// Created by Bartosz Budnik on 09.06.2024.
//

#include "includes/Platform.h"

Platform::Platform(const char *title, int width, int height, int textureWidth, int textureHeight) {

}

Platform::~Platform() {

}

void Platform::Update(const void *buffer, int pitch) {

}

bool Platform::ProcessInput(uint8_t *keys) {
    return false;
}
