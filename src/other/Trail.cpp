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

void CTrail::push(Vector2D pos) {

    //remove oldest cursor first if its lifetime is exceeded.
    std::erase_if(samples, [](CTrail::TrailPoint& x) {return x.age() >= CONFIG(trailLifetime); });

    //don't stack cursors, but keep an idle cursor young (for alpha calcs)
    if(samples.size() > 0) {
        if(samples.back().pos == pos) {
            samples.back().timestamp = high_resolution_clock::now();
             return;
        }
    }

    //determine if it's the right time to spawn a new cursor (according to rate)
    tick_counter += 1;
    if(tick_counter >= CONFIG(trailRate)) {
        //if we've exceeded the max trail length, remove earliest trail point
        if(samples.size() >= CONFIG(trailLength)) {
            samples.pop_front();
        }

        tick_counter = 0; //reset tick counter

        //add position to trail ring buffer
        samples.push_back({pos, high_resolution_clock::now()});
    }
}

void CTrail::warp(void) {
    samples.clear();
    tick_counter = 0;
}
