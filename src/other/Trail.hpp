#include <hyprland/src/helpers/AnimatedVariable.hpp>
#include <hyprutils/animation/AnimatedVariable.hpp>
#include <hyprutils/math/Vector2D.hpp>
#include <deque>
#include <chrono>

using namespace std::chrono;

class CTrail {
  public:

    struct TrailPoint {
        Vector2D pos;
        high_resolution_clock::time_point timestamp;

        float age(void) const {
            return duration_cast<std::chrono::milliseconds>
                   (high_resolution_clock::now() - timestamp)
                   .count();
        }
    };

    void push(Vector2D pos);
    const std::deque<TrailPoint>& get() const {
        return samples;
    }

    /* called when a cursor warp has happened (to avoid magnifying on warps) */
    void   warp(void);

  private:
        int tick_counter = 0;

    std::deque<TrailPoint> samples;
};
