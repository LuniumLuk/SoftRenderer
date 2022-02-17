#ifndef __MISC_HPP__
#define __MSIC_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

namespace Lurdr
{

class Timer
{
public:
    Timer(): m_timer_start(clock())
    {
        m_timer_name = new char[8];
        strcpy(m_timer_name, "Default");
    }
    Timer(const char* name): m_timer_start(clock())
    {
        m_timer_name = new char[strlen(name) + 1];
        strcpy(m_timer_name, name);
    }

    ~Timer()
    {
        if (m_timer_name)
        {
            stop();
        }
        delete[] m_timer_name;
    }

    void stop() {
        float duration = (float)(clock() - m_timer_start) / (CLOCKS_PER_SEC * 1000);
        printf("Timer (%s) : %.6fs (%.4fms)\n", m_timer_name, duration, duration * 1000);
        m_timer_start = 0;
    }

private:
    clock_t m_timer_start;
    char * m_timer_name;
};
    
}

#endif