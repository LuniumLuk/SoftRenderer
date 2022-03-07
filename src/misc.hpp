#ifndef __MISC_HPP__
#define __MISC_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

namespace Lurdr
{

#ifdef __OVERRIDE_NEW__

class MemoryCounter
{
private:
    size_t m_newed_size;
    size_t m_deleted_size;
public:
    MemoryCounter(): m_newed_size(0),
                     m_deleted_size(0) {}
    
    void newed(size_t size)
    {
        m_newed_size += size;
    }
    void deleted(size_t size) 
    {
        m_deleted_size += deleted;
    }

    size_t currentAllocatedMemory()
    {
        return m_newed_size - m_deleted_size;
    }
    size_t totalNewedMemory()
    {
        return m_newed_size;
    }
    size_t totalDeletedMemory()
    {
        return m_deleted_size;
    }
};

static MemoryCounter g_memory_counter = MemoryCounter();

void* operator new(size_t size)
{
    g_memory_counter.newed(size);
    printf("Current Allocated Memory (new) : %ld Bytes\n", g_memory_counter.currentAllocatedMemory());

    return malloc(size);
}

#endif

class Timer
{
private:
    clock_t m_timer_start;
    char*   m_timer_name;

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
};
    
}

#endif