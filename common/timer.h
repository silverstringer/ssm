#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <string>
#include <sstream>

namespace ch = std::chrono;

template <typename duration = ch::seconds, typename clock = ch::high_resolution_clock>
class timer
{
    typename clock::time_point m_start, m_stop;

    typename clock::rep get_time() const
    {
        return ch::duration_cast<duration>(m_stop - m_start).count();
    }

public:
    void         start() { m_start = clock::now(); }
    const timer& stop()  { m_stop = clock::now(); return *this; }

    std::ostream& print() const
    {
        return std::cout << "Time running: [" << get_time() << "]";
    }
};


#endif // TIMER_H
