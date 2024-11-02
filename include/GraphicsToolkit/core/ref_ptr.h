#ifndef REF_PTR_H
#define REF_PTR_H

#include <atomic>

class RefCtrl {
public:
  size_t RefCount() const { return count_; }

  size_t IncRefCount() { return count_.fetch_add(1); }

  size_t DecRefCount() { return count_.fetch_sub(1); }

private:
  std::atomic<size_t> count_ = 1;
};

template<typename T>
class RefPtr {
public:
  using ElementType = T;

  RefPtr() = default;

  template<typename U>
  RefPtr(U* ptr) : ptr_{ptr},
                   ctrl_{new RefCtrl} {}

  // template<typename U, typename Deleter>
  // RefPtr(U* ptr, Deleter d);

  RefPtr(std::nullptr_t) {}

  // template<typename Deleter>
  // RefPtr(std::nullptr_t, Deleter d);

  RefPtr(const RefPtr& other) : ptr_{other.ptr_}, ctrl_{other.ctrl_} { ctrl_->IncRefCount(); }

  RefPtr(RefPtr&& other) noexcept : ptr_{other.ptr_}, ctrl_{other.ctrl_} {
    other.ptr_ = nullptr;
    other.ctrl_ = nullptr;
  }

  ~RefPtr() {
    if (ctrl_ && ctrl_->DecRefCount() == 1)
      Destory();
  }

  size_t RefCount() const { return ctrl_->RefCount(); }

  template<typename U>
  void Reset(U* ptr) {
    if (ctrl_->DecRefCount() == 1) {
      Destory();
    }
    ptr_ = ptr;
    ctrl_ = new RefCtrl;
  }

  // template<typename U, typename Deleter>
  // void Reset(U* ptr, Deleter d);

  T& operator*() { return *ptr_; }

  const T& operator*() const { return *ptr_; }

  T* operator->() { return ptr_; }

  const T* operator->() const { return ptr_; }

private:
  T* ptr_ = nullptr;
  RefCtrl* ctrl_ = nullptr;

  void Destory() {
    delete ptr_;
    delete ctrl_;
  }
};


#endif  // REF_PTR_H