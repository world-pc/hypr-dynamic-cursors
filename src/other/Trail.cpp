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

using namespace std::chrono;

bool CTrail::push(Vector2D pos, const Pointer::CPointerManager::SCursorImage& img, double& rotation, double& scale) {
    /* returns true if an element was actually pushed or updated */

    //remove oldest cursor first if its lifetime is exceeded.
    std::erase_if(samples, [](CTrail::TrailPoint& x) { return x.age() >= CONFIG(trailLifetime); });

    //don't stack cursors, but keep an idle cursor young (for alpha calcs)
    if (!samples.empty()) {
        if (samples.back().pos == pos) {
            samples.back().timestamp = high_resolution_clock::now();
        }
    }

    //we'll get a stuttering lead cursor if we don't do this outside of the tick_counter conditional below.
    if (samples.empty()) {
        samples.push_back({pos, img.bufferTex, rotation, scale, img.size, img.hotspot, high_resolution_clock::now()});
    }

    //determine if it's the right time to spawn a new cursor (according to rate)
    tick_counter += 1;
    if (tick_counter >= CONFIG(trailRate)) {
        //if we've exceeded the max trail length, remove earliest trail point
        if (samples.size() >= CONFIG(trailLength)) {
            samples.pop_front();
        }

        tick_counter = 0; //reset tick counter

        /* push onto the trail if it's empty or position/rotation/scale have changed */
        if (samples.empty() || pos != samples.back().pos) {
            samples.push_back({pos, img.bufferTex, rotation, scale, img.size, img.hotspot, high_resolution_clock::now()});
            return true;
        } else if (!samples.empty() && samples.back().rotation != rotation) {
            samples.back().rotation = rotation;
            return true;
        } else if (!samples.empty() && samples.back().scale) {
            samples.back().scale = scale;
        }
    }

    return false;
}

void CTrail::warp(void) {
    samples.clear();
    tick_counter = 0;
}
