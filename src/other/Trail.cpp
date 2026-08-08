#include "../globals.hpp"
#include "../config/ConfigManager.hpp"
#include "Trail.hpp"

#include <algorithm>
#include <chrono>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/debug/log/Logger.hpp>
#include <hyprland/src/animation/AnimationManager.hpp>
#include <hyprland/src/managers/EventManager.hpp>
#include <hyprutils/animation/AnimationConfig.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/event/EventBus.hpp>

void CTrail::push(Vector2D pos) {
    //determine if it's the rigth time to spawn a new cursor (according to rate)
    req_counter += 1;
    if(req_counter >= CONFIG(trailRate)) {
        req_counter = 0;

        max = CONFIG(trailLength);
        samples.resize(max);

        if(index >= max) {
            index = 0;
        }

        samples[index] = pos;
        index = (index + 1) % max;
    }
}

void CTrail::warp(void) {
    samples.clear();
    index = 0;
}
