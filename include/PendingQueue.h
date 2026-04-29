#ifndef PENDING_QUEUE_H
#define PENDING_QUEUE_H

#include <cstddef>
#include <memory>
#include <queue>
#include <tuple>
#include <type_traits>
#include <utility>

class IPending {
public:
    virtual ~IPending() = default;
    virtual void run() = 0;
};

template<class Callable, class... Args>
class Pending : public IPending {
private:
    std::decay_t<Callable> callable;
    std::tuple<std::decay_t<Args>...> arguments;

public:
    Pending(Callable&& callable, Args&&... args)
        : callable(std::forward<Callable>(callable)),
          arguments(std::forward<Args>(args)...) {}

    void run() override {
        std::apply(callable, arguments);
    }
};

class PendingQueue {
private:
    std::queue<std::unique_ptr<IPending>> tasks;

public:
    template<class Callable, class... Args>
    void enqueue(Callable&& callable, Args&&... args) {
        using PendingType = Pending<Callable, Args...>;
        tasks.push(std::make_unique<PendingType>(
            std::forward<Callable>(callable),
            std::forward<Args>(args)...
        ));
    }

    void run() {
        if (tasks.empty()) {
            return;
        }

        tasks.front()->run();
        tasks.pop();
    }

    void runAll() {
        while (!tasks.empty()) {
            run();
        }
    }

    bool empty() const {
        return tasks.empty();
    }

    std::size_t size() const {
        return tasks.size();
    }
};

#endif
