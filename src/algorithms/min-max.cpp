#include "min-max.h"

#include <iostream>
#include <algorithm>

Fits IntegrateMaximum(const std::vector<Fits>& frames)
{
    std::cout << "Starting maximum integration...\n";

    int framesCount = frames.size();

    int pixelsCount = frames[0].pixels.size();

    Fits result = frames[0];
    result.pixels.clear();
    result.pixels.reserve(pixelsCount);

    float value;

    for (int i = 0; i < pixelsCount; i++)
    {
        value = frames[0].pixels[i];

        for (const Fits& frame : frames)
        {
            value = std::max(value, frame.pixels[i]);
        }

        result.pixels.push_back(value);
    }

    return result;
}

Fits IntegrateMinimum(const std::vector<Fits>& frames)
{
    std::cout << "Starting minimum integration...\n";

    int framesCount = frames.size();

    int pixelsCount = frames[0].pixels.size();

    Fits result = frames[0];
    result.pixels.clear();
    result.pixels.reserve(pixelsCount);

    float value;

    for (int i = 0; i < pixelsCount; i++)
    {
        value = frames[0].pixels[i];

        for (const Fits& frame : frames)
        {
            value = std::min(value, frame.pixels[i]);
        }

        result.pixels.push_back(value);
    }

    return result;
}
