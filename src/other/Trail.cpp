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

    //make sure we have the latest user-defined trail length
    max = CONFIG(trailLength);
    samples.resize(max);

    //just in case we size down the trail length
    if(index >= max) {
        index = 0;
    }

    tick_counter += 1;

    //don't stack cursors, but keep an idle cursor young (for alpha calcs)
    int last_pushed_index = (index > 0) ? (index - 1) : (samples.size()-1);

    if(pos == samples[last_pushed_index].pos) {
        samples[last_pushed_index].timestamp =
            std::chrono::high_resolution_clock::now();
        return;
    }

    //determine if it's the right time to spawn a new cursor (according to rate)
    if(tick_counter >= CONFIG(trailRate)) {
        tick_counter = 0; //reset tick counter

        //add position to trail ring buffer
        samples[index].pos = pos;
        samples[index].timestamp = std::chrono::high_resolution_clock::now();

        index = (index + 1) % max;
    }
}

void CTrail::warp(void) {
    samples.clear();
    index = 0;
    tick_counter = 0;
}
