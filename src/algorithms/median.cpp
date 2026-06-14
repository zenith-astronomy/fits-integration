#include "median.h"

#include <iostream>
#include <algorithm>

Fits IntegrateMedian(const std::vector<Fits>& frames)
{
    std::cout << "Starting median integration...\n";

    int framesCount = frames.size();

    int pixelsCount = frames[0].pixels.size();

    Fits result = frames[0];
    result.pixels.clear();
    result.pixels.reserve(pixelsCount);

    std::vector<float> values = {};
    values.reserve(framesCount);

    for (int i = 0; i < pixelsCount; i++)
    {
        values.clear();

        for (const Fits& frame : frames)
        {
            values.push_back(frame.pixels[i]);
        }

        std::sort(values.begin(), values.end());

        float median = 0.0f;

        if (framesCount % 2 == 1)
        {
            median = values[framesCount / 2];
        }
        else
        {
            float lower = values[(framesCount / 2) - 1];
            float upper = values[framesCount / 2];

            median = (lower + upper) / 2.0f;
        }

        result.pixels.push_back(median);
    }

    return result;
}
