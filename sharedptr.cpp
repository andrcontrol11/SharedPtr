#include <algorithm>
#include <cstddef>

template<typename T>
class SharedPtr {
public:
    SharedPtr() noexcept: ptr_(nullptr), count_(nullptr) {}

    SharedPtr(T *ptr) : ptr_(ptr), count_(nullptr) {
        increase();
    }

    SharedPtr(const SharedPtr &other) : ptr_(other.ptr_), count_(other.count_) {
        increase();
    }

    SharedPtr(SharedPtr &&other) noexcept: ptr_(other.ptr_), count_(other.count_) {
        other.ptr_ = nullptr;
        other.count_ = nullptr;
    }

    SharedPtr &operator=(T *pointer) {
        if (pointer != ptr_) {
            decrease();
            ptr_ = pointer;
            count_ = nullptr;
            increase();
        }
        return *this;
    }

    SharedPtr &operator=(const SharedPtr &other) {
        if (other.ptr_ != ptr_) {
            decrease();
            ptr_ = other.ptr_;
            count_ = other.count_;
            increase();
        }
        return *this;
    }

    SharedPtr &operator=(SharedPtr &&other) noexcept {
        if (other.ptr_ != ptr_) {
            decrease();
            ptr_ = other.ptr_;
            count_ = other.count_;
            other.ptr_ = nullptr;
            other.count_ = nullptr;
        }
        return *this;
    }

    ~SharedPtr() {
        decrease();
    }

    T &operator*() {
        return *ptr_;
    }

    const T &operator*() const {
        return *ptr_;
    }

    T *operator->() noexcept {
        return ptr_;
    }

    const T *operator->() const noexcept {
        return ptr_;
    }

    void reset(T *ptr_) noexcept {
        *this = ptr_;
    }

    void swap(SharedPtr &other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(count_, other.count_);
    }

    T *get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return (count_ != nullptr);
    }

private:
    T *ptr_;
    size_t *count_;

    void increase() {
        if (ptr_ != nullptr) {
            if (count_ == nullptr) {
                count_ = new size_t(0);
            }
            ++(*count_);
        }
    }

    void decrease() noexcept {
        if (ptr_ == nullptr) {
            return;
        }
        if (*count_ == 1) {
            delete count_;
            delete ptr_;
            return;
        }
        --(*count_);
    }
};
