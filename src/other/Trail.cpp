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

void CTrail::updateAlpha(void) {
    for(int i = 0; i < samples.size(); i += 1) {
        samples.at(i).alpha -= 0.025;
    }
}

void CTrail::push(Vector2D pos) {
    tick_counter += 1;

    //determine if it's the right time to spawn a new cursor (according to rate)
    if(tick_counter >= CONFIG(trailRate)) {
        tick_counter = 0; //reset tick counter

        //make sure we have the latest user-defined trail length
        max = CONFIG(trailLength);
        samples.resize(max);

        //just in case we size down the trail length
        if(index >= max) {
            index = 0;
        }

        //add position to trail ring buffer
        samples[index].pos = pos;
        samples[index].alpha = 1.0f;

        index = (index + 1) % max;
    }
}

void CTrail::warp(void) {
    samples.clear();
    index = 0;
    tick_counter = 0;
}
