#include "average.h"

#include <iostream>

Fits IntegrateAverage(const std::vector<Fits>& frames)
{
    std::cout << "Starting average integration...\n";

    int framesCount = frames.size();

    int pixelsCount = frames[0].pixels.size();

    Fits result = frames[0];
    result.pixels.clear();
    result.pixels.reserve(pixelsCount);

    for (int i = 0; i < pixelsCount; i++)
    {
        float sum = 0.0f;

        for (const Fits& frame : frames)
        {
            sum += frame.pixels[i];
        }

        float value = sum / framesCount;

        result.pixels.push_back(value);
    }

    std::cout << "Frames stacked successfully!\n";

    return result;
}
