#include <hyprland/src/helpers/AnimatedVariable.hpp>
#include <hyprutils/animation/AnimatedVariable.hpp>
#include <hyprland/src/render/Texture.hpp>
#include <hyprland/src/pointer/PointerManager.hpp>
#include <hyprutils/math/Vector2D.hpp>
#include <deque>
#include <chrono>

using namespace std::chrono;

class CTrail {
  public:
    struct TrailPoint {
        Vector2D                          pos;
        SP<Render::ITexture>              tex;
        double                            rotation;
        double                            scale;
        Vector2D                          size;
        Vector2D                          hotspot;
        high_resolution_clock::time_point timestamp;

        float age(void) const {
            return duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - timestamp).count();
        }
    };

    bool                          push(Vector2D pos, const Pointer::CPointerManager::SCursorImage& img, double& rotation, double& scale);
    const std::deque<TrailPoint>& get() const {
        return samples;
    }

    /* called when a cursor warp has happened (avoids inaccurate damage bounds / trails) */
    void warp(void);

  private:
    int                    tick_counter = 0;
    std::deque<TrailPoint> samples;
};
