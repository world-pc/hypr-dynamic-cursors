#include <hyprland/src/helpers/AnimatedVariable.hpp>
#include <hyprutils/animation/AnimatedVariable.hpp>
#include <hyprutils/math/Vector2D.hpp>
#include <vector>

class CTrail {
  public:

    struct TrailPoint {
        Vector2D pos;
        double alpha;
    };

    void updateAlpha(void);

    void push(Vector2D pos);
    const std::vector<TrailPoint>& get() const {return samples;}

    /* called when a cursor warp has happened (to avoid magnifying on warps) */
    void   warp(void);

  private:
        int index = 0;
        int max = 15;
        int tick_counter = 0;

    /* ringbuffer for last samples */
    std::vector<TrailPoint> samples;
};
