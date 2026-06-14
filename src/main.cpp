#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include "fits/fits.h"
#include "fits/dir.h"

#include "algorithms/average.h"
#include "algorithms/median.h"

int main()
{
    std::string input;
    std::string command;

    while (std::getline(std::cin, input))
    {
        command.clear();

        std::istringstream stream(input);
        stream >> command;

        if(command == "stack" ||
           command == "integrate" ||
           command == "s")
        {
            auto start = std::chrono::steady_clock::now();

            std::string algorithm;

            if (!(stream >> algorithm))
            {
                std::cerr << "Usage: stack <algorithm> <directory path>\n";
                continue;
            }

            std::string dirPath;
            std::getline(stream >> std::ws, dirPath);

            if (dirPath.empty())
            {
                std::cerr << "Usage: stack <algorithm> <directory path>\n";
                continue;
            }

            if (dirPath.size() >= 2 && dirPath.front() == '"' && dirPath.back() == '"')
            {
                dirPath = dirPath.substr(1, dirPath.size() - 2);
            }

            std::vector<fs::path> paths = GetFitsInDir(dirPath);

            std::vector<Fits> frames = {};
            frames.reserve(paths.size());

            for (fs::path& path : paths)
            {
                Fits currentFits;

                std::ifstream file(path, std::ios::binary);

                if (!ParseFitsHeader(currentFits, file))
                {
                    std::cerr << "Couldn't parse FITS header\n";
                    break;
                }

                if (!ParseFitsPixels(currentFits, file))
                {
                    std::cerr << "Couldn't parse FITS pixels\n";
                    break;
                }

                frames.push_back(currentFits);
            }

            std::cout << "Successfully opened " << frames.size() << " FITS files!\n";

            auto end = std::chrono::steady_clock::now();

            double elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000);

            std::cout << "Parsed " << frames.size() << " FITS files in " << elapsed << "ms\n";

            start = std::chrono::steady_clock::now();

            if (algorithm == "average")
            {
                Fits result = IntegrateAverage(frames);
                WriteToDir(result, dirPath);
            }
            else if (algorithm == "median")
            {
                Fits result = IntegrateMedian(frames);
                WriteToDir(result, dirPath);
            }
            else
            {
                std::cerr << "The selected integration algorithm is not available\n";
                continue;
            }

            end = std::chrono::steady_clock::now();

            elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000);

            std::cout << "Stacked " << frames.size() << " FITS files in " << elapsed << "ms\n";
        }
        else if (command == "quit" ||
                 command == "exit" ||
                 command == "q")
        {
            break;
        }
    }
}
