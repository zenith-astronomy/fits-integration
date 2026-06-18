#include "average.h"

#include <iostream>
#include <thread>
#include <functional>

void IntegrateAverageRange(const std::vector<Fits>& frames, Fits& result, int begin, int end)
{
    int framesCount = frames.size();

    for (int i = begin; i < end; i++)
    {
        float sum = 0.0f;

        for (const Fits& frame : frames)
        {
            sum += frame.pixels[i];
        }

        float value = sum / framesCount;

        result.pixels[i] = value;
    }
}

Fits IntegrateAverage(const std::vector<Fits>& frames)
{
    std::cout << "Starting average integration...\n";

    int pixelsCount = frames[0].pixels.size();

    Fits result = frames[0];
    result.pixels.clear();
    result.pixels.resize(pixelsCount);

    int threadCount = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    int pixelsPerThread = (pixelsCount + threadCount - 1) / threadCount;

    for (int threadIndex = 0; threadIndex < threadCount; threadIndex++)
    {
        int begin = threadIndex * pixelsPerThread;
        int end = std::min(begin + pixelsPerThread, pixelsCount);

        threads.emplace_back(IntegrateAverageRange, std::cref(frames), std::ref(result), begin, end);
    }

    for (std::thread& thread : threads)
    {
        thread.join();
    }

    return result;
}