//
// Created by EunwooSong on 2020-11-06.
//

#include "Button.h"
#include "../GameObject/GameObject.h"
#include "../../SharkSystem.hpp"
#include "Transform.h"
#include "Sprite2DRenderer.h"

SharkEngine::Core::Button::~Button() {
}
void SharkEngine::Core::Button::Start() {
    Component::Start();
}
void SharkEngine::Core::Button::Update() {
    Component::Update();

    Vec2 currentCursorPos = SHARK_INPUT_MGR->GetCursorPos();

    if(boundaryRect.offset(GetOwner()->transform->GetLocalPos()).Contain(currentCursorPos)) {
        isOnCursor = true;
        isEndCursor = true;
    }
    else {
        isOnCursor = false;
        isEndCursor = false;
    }

    if(isOnCursor) {
        if(SHARK_INPUT_MGR->GetKeyState(VK_LBUTTON) == KEY_UP)
            isOnClick = true;
        else
            isOnClick = false;
    }

    if(isOnClick) {
        onClick();
    }

    if(isEnableAnimation) {
        if(isOnCursor) {
            GetOwner()->transform->SetScale(
                    Lerp(GetOwner()->transform->GetScale(),
                         upScale,
                         upSpeed * SHARK_TIME_MGR->GetDeltaTime()));
        }
        else {
            GetOwner()->transform->SetScale(
                    Lerp(GetOwner()->transform->GetScale(),
                         Vec2(1.0f, 1.0f),
                         upSpeed * SHARK_TIME_MGR->GetDeltaTime()));

        }
    }
}
void SharkEngine::Core::Button::SetTextureImage(const char *path) {
    Sprite2DRenderer* renderer = GetOwner()->GetComponent<Sprite2DRenderer>();
    renderer->SetTexture(path);

    boundaryRect = Rect(0, 0, renderer->GetWidth(), renderer->GetHeight());
    GetOwner()->transform->SetScaleCenter(Vec2(renderer->GetWidth() / 2, renderer->GetHeight() / 2));
    GetOwner()->transform->SetRotationCenter(Vec2(renderer->GetWidth() / 2, renderer->GetHeight() / 2));
}