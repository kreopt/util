#ifndef BPUTIL_BACKGROUND_PROCESS_HPP
#define BPUTIL_BACKGROUND_PROCESS_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include <deque>
#include <condition_variable>

#include <binelpro/log.hpp>

namespace bp {
    using bp::Log;

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
    protected:
        virtual void thread_func() = 0;
    public:
        background_loop(): stopped_(false) {}
        virtual ~background_loop(){
            stop();
            wait_until_stopped();
        }
        inline virtual void start() override {
            std::lock_guard<std::mutex>  lck(stop_mutex_);
            stopped_ = false;
            background_task_ = std::thread([this]{
                while (running()) {
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
        inline bool running() const { return !stopped_; }
    };

    template <typename item_type>
    class processing_queue : public background_loop {
        std::deque<item_type>                queue_;
        std::condition_variable              queue_cv_;
        std::mutex                           queue_mutex_;
        bool                                 notified_;
        const size_t                         max_size_;
        std::atomic_ulong                    current_size_;
        std::atomic_bool                     stop_on_queue_end_;

    protected:
        virtual size_t  get_item_size(const item_type &buf) const {
            return 1;
        }

        virtual void thread_func() override final {
            std::unique_lock<std::mutex> lck(queue_mutex_);
            if (!queue_.size()) {
                if (stop_on_queue_end_ && running()) {
                    stop();
                    return;
                }
                queue_cv_.wait(lck, [this]() { return notified_; });
            }
            if (!running()) {
                return;
            }
            notified_=false;
            auto &buf = queue_.front();
            lck.unlock();
            if (queue_func(buf)) {
                lck.lock();
                current_size_ -= get_item_size(buf);
                queue_.pop_front();
                if (!queue_.size()) {
                    if (stop_on_queue_end_ && running()) {
                        stop();
                    }
                }
                notified_=false;
            }
        }

    public:
        processing_queue() = delete;
        explicit processing_queue(const size_t _queue_size): notified_(false),
                                                             max_size_(_queue_size),
                                                             current_size_(0),
                                                             stop_on_queue_end_(false)
        {
        }

        virtual bool queue_func(item_type & _item) = 0; // TODO: non-const reference sucks

        virtual void stop() override {
            background_loop::stop();
            notified_=true;
            queue_cv_.notify_all();
        }

        virtual void start() override {
            notified_ = false;
            background_loop::start();
        }
        void enqueue(item_type &&_buf) {
            auto buf_size = get_item_size(_buf);
            if ((current_size_+buf_size < max_size_) || !max_size_) {
                std::unique_lock<std::mutex> lck(queue_mutex_);
                current_size_ += buf_size;
                queue_.emplace_back(std::forward<item_type>(_buf));
                notified_ = true;
                queue_cv_.notify_one();
            } else {
                Log::w(std::string("queue is full. dropping buffer"));
            }
        }
        void stop_on_queue_end() {
            std::lock_guard<std::mutex> lck(queue_mutex_);
            if (!queue_.size()) {
                stop();
            } else {
                stop_on_queue_end_=true;
            }
        }

        void flush() {
            std::lock_guard<std::mutex> lck(queue_mutex_);
            queue_.clear();
        }

        virtual ~processing_queue(){
            stop();
        }
    };
}
#endif //BPUTIL_BACKGROUND_PROCESS_HPP
