//#ifndef MYMAP_H
//#define MYMAP_H
//#include "rbt.h"
//template <class KeyType, class ValueType>
//class Iterator
//{
//private:
//    std::shared_ptr<Node<KeyType,ValueType>> node;
//public:
//    Iterator() : node(nullptr){}
//    Iterator(std::shared_ptr<Node<KeyType,ValueType>> node) : node(node){}
//    std::pair<KeyType, ValueType>& operator*() const
//    {
//        return node->data;
//    }
//    Iterator& operator++()
//    {
//        if(node->right != nullptr)
//        {
//            node = node->right;
//            //ищем самый маленький элемент большего поддерева
//            while(node->left != nullptr)
//            {
//                node = node->left;
//            }
//        }
//        else
//        {
//            //В этом случае итератор перемещается вверх по дереву к родителю, а затем продолжает двигаться вверх до тех пор,
//            //пока не будет найден родитель, у которого узел не является правым потомком.
//            std::shared_ptr<Node<KeyType,ValueType>> parent = node->parent.lock();
//            while(parent != nullptr && node == parent->right)
//            {
//                node = parent;
//                parent = parent->parent.lock();
//            }
//            node = parent;
//        }
//        return *this;
//    }
//    bool operator==(const Iterator& other) const
//    {
//        return node == other.node;
//    }
//    bool operator!=(const Iterator& other) const
//    {
//        return node != other.node;
//    }
//};
//template <class KeyType, class ValueType>
//class mymap : public rbt<KeyType,ValueType>
//{
//public:
////    mymap(){}
////    ~mymap(){}
//    Iterator<KeyType,ValueType> begin()
//    {
//        std::shared_ptr<Node<KeyType,ValueType>> leftmost = this->root;
//        while(leftmost != nullptr && leftmost->left != nullptr) //ищем самый маленький элемент
//        {
//            leftmost = leftmost->left;
//        }
//        return Iterator<KeyType,ValueType>(leftmost);
//    }
//    Iterator<KeyType,ValueType> end()
//    {
//        return Iterator<KeyType,ValueType>(nullptr);
//    }
//};

//#endif // MYMAP_H
