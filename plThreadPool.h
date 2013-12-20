#ifndef PL_THREAD_POOL_H
#define PL_THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>


class plThreadPool
{
    public:
    
        plThreadPool( size_t threads );
        ~plThreadPool();
        
        template< typename F, typename... Args >
        std::future<typename std::result_of< F( Args... ) >::type> enqueue( F&& f, Args&&... args );
    
    private:
        // need to keep track of threads so we can join them
        std::vector< std::thread >          workers;
        // the task queue
        std::queue< std::function<void()> > jobs;
        
        // synchronization
        std::mutex              queue_mutex;
        std::condition_variable condition;
        bool                    stop;
};


// add new work item to the pool
template< typename Func, typename... Args >
std::future<typename std::result_of< Func( Args... ) >::type> plThreadPool::enqueue( Func&& f, Args&&... args )
{
    typedef typename std::result_of<Func(Args...)>::type return_type;
    
    // don't allow enqueueing after stopping the pool
    if( stop )
        throw std::runtime_error("enqueue on stopped ThreadPool");

    auto job = std::make_shared< std::packaged_task< return_type() > > ( std::bind( std::forward< Func >( f ), std::forward< Args >( args )... ) );
        
    std::future<return_type> res = job->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push( [job](){ (*job)(); } );
    }
    condition.notify_one();
    return res;
}


#endif
