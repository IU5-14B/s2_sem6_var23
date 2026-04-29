#ifndef MY_CONTAINER_H
#define MY_CONTAINER_H

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class MyContainer {
private:
    using Allocator = std::allocator<T>;
    using AllocTraits = std::allocator_traits<Allocator>;

    Allocator allocator;
    T* data;
    std::size_t currentSize;
    std::size_t currentCapacity;

    void destroyElements() noexcept {
        for (std::size_t i = 0; i < currentSize; ++i) {
            AllocTraits::destroy(allocator, data + i);
        }
        currentSize = 0;
    }

    void freeStorage() noexcept {
        if (data != nullptr) {
            AllocTraits::deallocate(allocator, data, currentCapacity);
            data = nullptr;
            currentCapacity = 0;
        }
    }

    void reserveInternal(std::size_t newCapacity) {
        if (newCapacity <= currentCapacity) {
            return;
        }

        T* newData = AllocTraits::allocate(allocator, newCapacity);
        std::size_t constructedCount = 0;

        try {
            for (; constructedCount < currentSize; ++constructedCount) {
                AllocTraits::construct(
                    allocator,
                    newData + constructedCount,
                    std::move_if_noexcept(data[constructedCount])
                );
            }
        } catch (...) {
            for (std::size_t i = 0; i < constructedCount; ++i) {
                AllocTraits::destroy(allocator, newData + i);
            }
            AllocTraits::deallocate(allocator, newData, newCapacity);
            throw;
        }

        destroyElements();
        freeStorage();

        data = newData;
        currentSize = constructedCount;
        currentCapacity = newCapacity;
    }

public:
    template <bool IsConst>
    class BasicIterator {
    private:
        using PointerType = std::conditional_t<IsConst, const T*, T*>;
        using ReferenceType = std::conditional_t<IsConst, const T&, T&>;

        PointerType ptr;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = PointerType;
        using reference = ReferenceType;

        BasicIterator(PointerType ptr = nullptr) : ptr(ptr) {}

        template <bool OtherIsConst, typename = std::enable_if_t<IsConst && !OtherIsConst>>
        BasicIterator(const BasicIterator<OtherIsConst>& other) : ptr(other.base()) {}

        reference operator*() const {
            return *ptr;
        }

        pointer operator->() const {
            return ptr;
        }

        BasicIterator& operator++() {
            ++ptr;
            return *this;
        }

        BasicIterator operator++(int) {
            BasicIterator old(*this);
            ++(*this);
            return old;
        }

        BasicIterator& operator--() {
            --ptr;
            return *this;
        }

        BasicIterator operator--(int) {
            BasicIterator old(*this);
            --(*this);
            return old;
        }

        BasicIterator& operator+=(difference_type offset) {
            ptr += offset;
            return *this;
        }

        BasicIterator& operator-=(difference_type offset) {
            ptr -= offset;
            return *this;
        }

        BasicIterator operator+(difference_type offset) const {
            BasicIterator result(*this);
            result += offset;
            return result;
        }

        BasicIterator operator-(difference_type offset) const {
            BasicIterator result(*this);
            result -= offset;
            return result;
        }

        difference_type operator-(const BasicIterator& other) const {
            return ptr - other.ptr;
        }

        reference operator[](difference_type index) const {
            return ptr[index];
        }

        bool operator==(const BasicIterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const BasicIterator& other) const {
            return !(*this == other);
        }

        bool operator<(const BasicIterator& other) const {
            return ptr < other.ptr;
        }

        bool operator>(const BasicIterator& other) const {
            return ptr > other.ptr;
        }

        bool operator<=(const BasicIterator& other) const {
            return ptr <= other.ptr;
        }

        bool operator>=(const BasicIterator& other) const {
            return ptr >= other.ptr;
        }

        PointerType base() const {
            return ptr;
        }
    };

    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;

    MyContainer() : data(nullptr), currentSize(0), currentCapacity(0) {}

    MyContainer(const MyContainer& other) : data(nullptr), currentSize(0), currentCapacity(0) {
        reserveInternal(other.currentSize);
        for (const T& value : other) {
            push_back(value);
        }
    }

    MyContainer(MyContainer&& other) noexcept
        : data(other.data),
          currentSize(other.currentSize),
          currentCapacity(other.currentCapacity) {
        other.data = nullptr;
        other.currentSize = 0;
        other.currentCapacity = 0;
    }

    MyContainer& operator=(const MyContainer& other) {
        if (this == &other) {
            return *this;
        }

        MyContainer copy(other);
        swap(copy);
        return *this;
    }

    MyContainer& operator=(MyContainer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroyElements();
        freeStorage();

        data = other.data;
        currentSize = other.currentSize;
        currentCapacity = other.currentCapacity;

        other.data = nullptr;
        other.currentSize = 0;
        other.currentCapacity = 0;
        return *this;
    }

    ~MyContainer() {
        destroyElements();
        freeStorage();
    }

    void push_back(const T& value) {
        if (currentSize == currentCapacity) {
            reserveInternal(currentCapacity == 0 ? 1 : currentCapacity * 2);
        }

        AllocTraits::construct(allocator, data + currentSize, value);
        ++currentSize;
    }

    void push_back(T&& value) {
        if (currentSize == currentCapacity) {
            reserveInternal(currentCapacity == 0 ? 1 : currentCapacity * 2);
        }

        AllocTraits::construct(allocator, data + currentSize, std::move(value));
        ++currentSize;
    }

    void pop_back() {
        if (currentSize == 0) {
            return;
        }

        --currentSize;
        AllocTraits::destroy(allocator, data + currentSize);
    }

    T& operator[](std::size_t index) {
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        return data[index];
    }

    std::size_t size() const {
        return currentSize;
    }

    bool empty() const {
        return currentSize == 0;
    }

    Iterator begin() {
        return Iterator(data);
    }

    Iterator end() {
        return Iterator(data + currentSize);
    }

    ConstIterator begin() const {
        return ConstIterator(data);
    }

    ConstIterator end() const {
        return ConstIterator(data + currentSize);
    }

    ConstIterator cbegin() const {
        return ConstIterator(data);
    }

    ConstIterator cend() const {
        return ConstIterator(data + currentSize);
    }

    void swap(MyContainer& other) noexcept {
        std::swap(data, other.data);
        std::swap(currentSize, other.currentSize);
        std::swap(currentCapacity, other.currentCapacity);
    }
};

template <typename T>
typename MyContainer<T>::template BasicIterator<false> operator+(
    typename MyContainer<T>::Iterator::difference_type offset,
    typename MyContainer<T>::template BasicIterator<false> iterator
) {
    return iterator + offset;
}

template <typename T>
typename MyContainer<T>::template BasicIterator<true> operator+(
    typename MyContainer<T>::ConstIterator::difference_type offset,
    typename MyContainer<T>::template BasicIterator<true> iterator
) {
    return iterator + offset;
}

#endif
