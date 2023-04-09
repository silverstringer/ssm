#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <chrono>
#include <atomic>
#include <iostream>

#define MAX_THREADS std::thread::hardware_concurrency()-1;

class ThreadPool   {

public:
    ThreadPool() = delete;
    ThreadPool operator = (const ThreadPool & th) = delete;

    ~ThreadPool() {
        std::unique_lock<std::mutex> lock(m_JobMutex);
        cv.notify_all();
        m_stop = true;
        std::cout<< " \n ~Thread Pool";
    }

    inline ThreadPool(uint8_t numThreads) : numThreads(numThreads), m_stop(false), m_join(false) {

        reserveThreadPool();
    }

    inline void reserveThreadPool()
    {
        int tmp = MAX_THREADS;
        if(numThreads > tmp) {
            numThreads = tmp;
        }

        Pool.reserve(numThreads);

//        stopThreadPool();
        for(auto i = 0; i != numThreads; ++i) {
            Pool.emplace_back(std::thread(&ThreadPool::run, this));
            if(m_join) {
                Pool.back().join();
            }
            else {
                Pool.back().detach();
            }
        }
    }

    auto getCounterTask ()
    {
        return m_JobQueue.size();
    }

    template<typename Func, typename... Args>
    inline auto addTask(Func &&f, Args &&... args)-> std::future<decltype(f(args...))> {

        using retType = typename std::result_of<Func(Args ...)>::type;
        using Task  = std::packaged_task<retType()>;

        Task task(std::move(std::bind(f, args...)));
        std::unique_lock<std::mutex> lock(m_JobMutex);
        std::future<retType> future = task.get_future();

        if (m_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        m_JobQueue.emplace(std::make_shared<AnyJob<retType>> (std::move(task)));
        cv.notify_one();

        return future;
    }


    inline uint8_t getThreadCount() {
        return numThreads;
    }

    void setJoin(bool mode) {
        m_join = mode;
    }

private:
    class Job {
    private:
        std::packaged_task<void()> func;
    public:
        virtual ~Job() {}
        virtual void execute() = 0;
    };

    template<typename RetType>
    class AnyJob : public Job {
    private:
        std::packaged_task<RetType()> func;
    public:
        AnyJob(std::packaged_task<RetType()> func) : func(std::move(func)) {}

        void execute() {
            func();
        }
    };

    uint8_t numThreads;
    std::vector<std::thread> Pool;
    std::queue<std::shared_ptr<Job>> m_JobQueue;
    std::condition_variable cv;
    std::mutex m_JobMutex;
    std::atomic<bool> m_stop;
    bool m_join;

    inline void run() {
        while (true)
//        while (true && !m_stop)
        {

            std::unique_lock<std::mutex> lock(m_JobMutex);
            cv.wait(lock, [this] { return !m_JobQueue.empty() || !m_stop; });


            if( m_stop || m_JobQueue.empty())
            {
//                qDebug() <<"QUEUE IS EMPTY! \n ";
//                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                continue;
//                return;
            }
//            if(m_JobQueue.size() > 1 )
//            {
                (*m_JobQueue.front()).execute();
                m_JobQueue.pop();
                std::cout<<" TASK RUNNING" <<"\n";

//            }
        }
    }

    void stopThread()
    {
        m_stop = true;
    }
};

