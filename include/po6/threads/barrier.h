// Copyright (c) 2011, Robert Escriva
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of po6 nor the names of its contributors may be used
//       to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef po6_threads_barrier_h_
#define po6_threads_barrier_h_

// POSIX
#include <errno.h>
#include <pthread.h>

// po6
#include <po6/error.h>
#include <po6/noncopyable.h>

namespace po6
{
namespace threads
{

class barrier
{
    public:
        barrier(size_t count);
        ~barrier() throw ();

    public:
        bool wait();

    private:
        PO6_NONCOPYABLE(barrier);

    private:
        pthread_barrier_t m_barrier;
};

inline
barrier :: barrier(size_t count)
    : m_barrier()
{
    int ret = pthread_barrier_init(&m_barrier, NULL, count);

    if (ret != 0)
    {
        throw po6::error(ret);
    }
}

inline
barrier :: ~barrier() throw ()
{
    int ret = pthread_barrier_destroy(&m_barrier);

    if (ret != 0)
    {
        try
        {
            PO6_DTOR_ERROR("Could not destroy barrier.");
        }
        catch (...)
        {
        }
    }
}

inline bool
barrier :: wait()
{
    int ret = pthread_barrier_wait(&m_barrier);

    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD)
    {
        throw po6::error(ret);
    }

    return ret == PTHREAD_BARRIER_SERIAL_THREAD;
}

} // namespace threads
} // namespace po6

#endif // po6_threads_barrier_h_
