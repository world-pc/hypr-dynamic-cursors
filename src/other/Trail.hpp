#include <hyprland/src/helpers/AnimatedVariable.hpp>
#include <hyprutils/animation/AnimatedVariable.hpp>
#include <hyprutils/math/Vector2D.hpp>
#include <vector>

using namespace Hyprutils::Math;
using namespace Hyprutils::Animation;
using namespace std::chrono;

class CTrail {
  public:
    CTrail();
        
    void push(Vector2D pos);
    const std::vector<Vector2D>& get() const {return samples;}

    /* called when a cursor warp has happened (to avoid magnifying on warps) */
    void   warp(void);

    /* force magnification regardless of speed now */
    void force(std::optional<int> duration, std::optional<float> size);

  private:
        int index = 0;
        int max = 15;
    /* tracks whether the current shake has already been announced in the ipc */
    bool ipc = false;

    bool                     started = false;
    PHLANIMVAR<float>        zoom;
    steady_clock::time_point end;

    /* ringbuffer for last samples */
    std::vector<Vector2D> samples;
    /* we also store the distance for each sample to the last, so we do only compute this once */
    std::vector<double>   samples_distance;
    int                   samples_index = 0;
};
