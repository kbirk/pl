#include "plThreadPool.h"

plThreadPool::plThreadPool( size_t threads )
    :   stop(false)
{
    for(size_t i = 0; i<threads; ++i)
    {
        workers.emplace_back(
            [this]
            {
                while(true)
                {
                    std::unique_lock< std::mutex > lock( this->queue_mutex );
                    
                    while( !this->stop && this->jobs.empty() )
                        this->condition.wait( lock );
                        
                    if( this->stop && this->jobs.empty() )
                        return;
                        
                    std::function< void() > task( this->jobs.front() );
                    this->jobs.pop();
                    lock.unlock();
                    task();
                }
            }
        );
    }
}


// the destructor joins all threads
plThreadPool::~plThreadPool()
{
    {
        std::unique_lock< std::mutex > lock( queue_mutex );
        stop = true;
    }
    condition.notify_all();
    for(size_t i = 0; i<workers.size(); ++i)
        workers[i].join();
}

	

