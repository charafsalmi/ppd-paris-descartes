#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include "Control.hpp"

namespace gui{
    class ProgressBar : public Control{
    public:
        ProgressBar(const ControlID id, const ControlPos pos);
    };
}

#endif
