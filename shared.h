#ifndef SHARED_H
#define SHARED_H

template<class T>
class shared
{
private:
    T* ptr;
    size_t* refCount;
    void _clean();
public:
    shared() :ptr(nullptr), refCount(new size_t(0)) {};
    shared(T* _ptr) :ptr(_ptr), refCount(new size_t(1)) {};
    shared(const shared& _ptr);
    shared& operator=(const shared& _ptr);
    shared(shared&& _ptr);
    shared& operator=(shared&& _ptr);
    size_t getCount() const;
    T* get() const;
    T* operator->() const;
    T& operator*() const;
    ~shared() { _clean(); };
};

#endif // SHARED_H
