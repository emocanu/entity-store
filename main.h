#pragma once
#include <chrono>
#include <iostream>

struct TimeToRun
{
    typedef std::chrono::high_resolution_clock::time_point moment;

    TimeToRun(char const* string)
    {
        str = string;
        t_start = std::chrono::high_resolution_clock::now();
    }
    ~TimeToRun()
    {
        using namespace std::chrono;
        moment t_end = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
        std::cout << str << "\t= " << time_span.count() << " seconds.\n";
    }
private:
    moment t_start;
    char const* str = nullptr;
};

struct Properties {
    std::string title = "";
    std::string description = "";
    double timestamp = 0.0;
};
