#pragma once

#include <vector>
#include <utility>
#include <string>
#include <functional>

using YogaPose = std::pair<std::string, int>;
using PoseChangeCb = std::function<void (YogaPose)>;
using Sequence = std::vector<YogaPose>;

class YogaSequence {
public:
    YogaSequence() {
        current_ = sequence_.begin();
    };
    YogaSequence(
        PoseChangeCb pose_change_cb
    ) : pose_change_cb_{pose_change_cb} {
        current_ = sequence_.begin();
    }

    void StartMorningSalutation() {
        static Sequence morning_salutation = {
            {"mountain", 3000},
            {"extended_mountain", 3000},
            {"half_forward_fold", 30000},
            {"forward_fold", 30000},
            {"four_limbed_staff", 60000},
            {"up_dog", 60000},
            {"down_dog", 30000},
            {"low_lunge", 30000},
            {"four_limbed_staff", 20000},
            {"up_dog", 30000},
            {"down_dog", 60000},
            {"low_lunge", 30000},
            {"four_limbed_staff", 20000},
            {"up_dog",60000},
            {"down_dog", 60000},
            {"forward_fold", 30000},
            {"extended_mountain", 3000},
            {"mountain", 3000}
        };
        sequence_ = morning_salutation;
        Start();
    }

    void Start() {
        if (sequence_.size() > 0) {
            current_ = sequence_.begin();
            ChangePose();
        }
    }

    void OnTimer() {
        timer_next_ok_ = true;
        if (audio_next_ok_) Next();
    }
    void OnAudioFinished() {
        audio_next_ok_ = true;
        if (timer_next_ok_) Next();
    }

    void Next() {
        if (HasNext()) {
            ++current_;
            ChangePose();
        }
    }

    void Previous() {
        if (HasPrevious()) {
            --current_;
            ChangePose();
        }
    }

    const YogaPose& GetCurrent() {
        if (IsInProgress()) {
            return *current_;
        }

        static YogaPose empty = std::pair{"", 0};

        return empty;
    }

    bool IsInProgress() {
        return current_ != sequence_.end();
    }

    bool HasNext() {
        return current_ + 1 != sequence_.end();
    }

    bool HasPrevious() {
        return current_ != sequence_.begin();
    }

private:
    void ChangePose() {
        pose_change_cb_(*current_);
        audio_next_ok_ = false;
        timer_next_ok_ = false;
    }

    Sequence sequence_{};
    Sequence::iterator current_{};

    bool timer_next_ok_{false};
    bool audio_next_ok_{false};

    PoseChangeCb pose_change_cb_;
};
