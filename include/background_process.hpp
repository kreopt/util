#ifndef BPUTIL_BACKGROUND_PROCESS_HPP
#define BPUTIL_BACKGROUND_PROCESS_HPP

#include <thread>
#include <atomic>
#include <mutex>

namespace bp {
    class background_process {
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void wait_until_stopped() = 0;

        virtual ~background_process() {}
    };

    class background_loop : public background_process {
        std::atomic_bool    stopped_;
        std::thread         background_task_;
        std::mutex          stop_mutex_;
    public:
        virtual ~background_loop(){
            stop();
            wait_until_stopped();
        }
        virtual void thread_func() = 0;
        inline virtual void start() override {
            stopped_ = false;
            background_task_ = std::thread([this]{
                while (!stopped_) {
                    thread_func();
                }
            });
        };
        inline virtual void stop() override {stopped_ = true;};
        inline virtual void wait_until_stopped() override {
            std::lock_guard<std::mutex>  lck(stop_mutex_);
            if (background_task_.joinable()) {
                background_task_.join();
            }
        }
    };
}
#endif //BPUTIL_BACKGROUND_PROCESS_HPP
