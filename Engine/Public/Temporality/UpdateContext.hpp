#ifndef TEMPORALITY_UPDATE_CONTEXT_HPP
#define TEMPORALITY_UPDATE_CONTEXT_HPP

#include <Temporality/Config.hpp>

namespace Temporality {

class TEMPORALITY_ENGINE_API UpdateContext
{
public:
    DISALLOW_COPY_AND_ASSIGN(UpdateContext)

    UpdateContext() = default;

    virtual ~UpdateContext() = default;

    // Target Frames/Second
    virtual inline float GetTargetFPS() const {
        return _mTargetFPS;
    }

    virtual inline void SetTargetFPS(float fps) {
        _mTargetFPS = fps;
    }

    // Current Frames/Second
    virtual inline float GetCurrentFPS() const {
        return _mCurrentFPS;
    }

    virtual inline void SetCurrentFPS(float fps) {
        _mCurrentFPS = fps;
        _mFpsRatio = _mTargetFPS / _mCurrentFPS;
    }

    // Ratio between Target FPS and Current FPS
    // > 1.0 means Current is less than Target
    // < 1.0 means Current is greater than Target
    virtual inline float GetFPSRatio() const {
        return _mFpsRatio;
    }

    // Time since last update in Milliseconds
    virtual inline uintmax_t GetTotalTime() const {
        return _mTotalTime;
    }

    virtual inline void AddElapsedTime(uintmax_t time) {
        _mElapsedTime = time;
        _mTotalTime += time;
    }

private:
    float _mTargetFPS = 0.0f;

    float _mCurrentFPS = 0.0f;

    float _mFpsRatio = 0.0f;

    uintmax_t _mElapsedTime = 0;

    uintmax_t _mTotalTime = 0;

}; // class UpdateContext

} // namespace Temporality

#endif // TEMPORALITY_UPDATE_CONTEXT_HPP