#include <hyprland/src/helpers/AnimatedVariable.hpp>
#include <hyprutils/animation/AnimatedVariable.hpp>
#include <hyprutils/math/Vector2D.hpp>
#include <vector>

using namespace Hyprutils::Math;
using namespace Hyprutils::Animation;
using namespace std::chrono;

class CTrail {
  public:       
    void push(Vector2D pos);
    const std::vector<Vector2D>& get() const {return samples;}

    /* called when a cursor warp has happened (to avoid magnifying on warps) */
    void   warp(void);

  private:
        int index = 0;
        int max = 15;
        int req_counter = 0;

    /* ringbuffer for last samples */
    std::vector<Vector2D> samples;
};
