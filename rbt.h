#ifndef RBT_H
#define RBT_H
#include "shared.h"
#include <memory>
#include <utility> // Необходимо включить заголовочный файл для std::pair

template<class T, class T1>
struct Node {
    Node(T nKey, T1 nValue) : value(nKey,nValue),
        left(nullptr),
        right(nullptr),
        next(nullptr),
        prev(nullptr),
        parent(nullptr),
        color(false){}
    shared<Node> left, right;
    std::weak_ptr<Node> parent; // weak_ptr во избежания циклических ссылок
    bool color; // red - false, black - true
    Node* next, prev;
    std::pair<T,T1> value;
};

template<class T, class T1>
class rbt
{
protected:
    struct Node;
    shared<Node> root;
    shared<Node> grandparent(shared<Node> node);
    shared<Node> uncle(shared<Node> node);
    shared<Node> sibling(shared<Node> node);
    void RotateLeft(shared<Node> node);
    void RotateRight(shared<Node> node);
    void InsCase1(shared<Node> node);
    void InsCase2(shared<Node> node);
    void InsCase3(shared<Node> node);
    void InsCase4(shared<Node> node);
    void InsCase5(shared<Node> node);
    void DelCase1(shared<Node> node);
    void DelCase2(shared<Node> node);
    void DelCase3(shared<Node> node);
    void DelCase4(shared<Node> node);
    void DelCase5(shared<Node> node);
    void DelCase6(shared<Node> node);
    shared<Node> find(shared<Node> node, T key);
    shared<Node> min(shared<Node> node);
    shared<Node> max(shared<Node> node);
public:
    rbt():root(nullptr){}
    ~rbt();
    void add(T key, T1 value);
    void remove(T key);
    T1& operator[](T key);
    void clear();
};

#endif // RBT_H
