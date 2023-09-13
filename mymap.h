#ifndef MYMAP_H
#define MYMAP_H
#include "rbt.h"
#include <memory>
template <typename KeyType, typename ValueType>
class mymap : public rbt<KeyType,ValueType>
{
public:
    class Iterator
    {
    private:
        shared<Node<KeyType,ValueType>> node;
    public:
        Iterator() : node(nullptr){}
        Iterator(shared<Node<KeyType,ValueType>> node) : node(node){}
        std::pair< KeyType, ValueType>& operator*() const
        {
            return node->data;
        }
        Iterator& operator++()
        {
            if(node->right != nullptr)
            {
                node = node->right;
                //ищем самый маленький элемент большего поддерева
                while(node->left != nullptr)
                {
                    node = node->left;
                }
            }
            else
            {
                //В этом случае итератор перемещается вверх по дереву к родителю, а затем продолжает двигаться вверх до тех пор,
                //пока не будет найден родитель, у которого узел не является правым потомком.
                shared<Node<KeyType,ValueType>> parent = node->parent;
                while(parent != nullptr && node == parent->right)
                {
                    node = parent;
                    parent = parent->parent;
                }
                node = parent;
            }
            return *this;
        }
        bool operator==(const Iterator& other) const
        {
            return node == other.node;
        }
        bool operator!=(const Iterator& other) const
        {
            return node != other.node;
        }
    };
    Iterator begin()
    {
        shared<Node<KeyType,ValueType>> leftmost = this->root;
        while(leftmost != nullptr && leftmost->left != nullptr) //ищем самый маленький элемент
        {
            leftmost = leftmost->left;
        }
        return Iterator(leftmost);
    }
    Iterator end()
    {
        return Iterator(nullptr);
    }
    mymap();
    ~mymap();
};

#endif // MYMAP_H
