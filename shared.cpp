#include "shared.h"

template<typename T>
shared<T>::shared(const shared& _ptr) {
    this->ptr = _ptr.ptr;
    this->refCount = _ptr.refCount;
    if (_ptr.ptr != nullptr) {
        (*this->refCount)++;
    }
}
template<typename T>
shared<T>& shared<T>::operator=(const shared& _ptr) {
    _clean();
    this->ptr = _ptr.ptr;
    this->refCount = _ptr.refCount;
    if (_ptr.ptr != nullptr) {
        (*this->refCount)++;
    }
}
template<typename T>
shared<T>::shared(shared&& _ptr) {
    this->ptr = _ptr.ptr;
    this->refCount = _ptr.refCount;
    _ptr.ptr = _ptr.refCount = nullptr;
}
template<typename T>
shared<T>& shared<T>::operator=(shared&& _ptr) {
    _clean();
    this->ptr = _ptr.ptr;
    this->refCount = _ptr.refCount;
    _ptr.ptr = _ptr.refCount = nullptr;
}
template<typename T>
size_t shared<T>::getCount() const {
    return *refCount;
}
template<typename T>
T* shared<T>::get() const {
    return this->ptr;
}
template<typename T>
T* shared<T>::operator->() const {
    return this->ptr;
}
template<typename T>
T& shared<T>::operator*() const {
    return *this->ptr;
}
template<typename T>
void shared<T>::_clean() {
    (*refCount)--;
    if (*refCount == 0) {
        if (ptr != nullptr) {
            delete ptr;
        }
        delete refCount;
    }
}
