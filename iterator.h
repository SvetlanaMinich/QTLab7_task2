#ifndef ITERATOR_H
#define ITERATOR_H

template<class T, class T1>
class Iterator
{
private:
    shared<Node<T,char>> node;
public:
    Iterator() : node(nullptr){}
    explicit Iterator(shared<Node<KeyType,char>> node) : node(node){} //explicit для явного использования конструктора в дальнейшем
    Iterator& operator++()
    {
        if(node->right)
        {
            node = node->right;
            while(node->left)
            {
                node = node->left;
            }
        }
        else
        {
            shared<Node<KeyType,char>> parent = node->parent;
            while(parent && node == parent->right)
            {
                node = parent;
                parent = parent->parent;
            }
            node = parent;
        }
        return *this;
    }
    const std::pair< KeyType, char>& operator*() const
    {
        return node->data;
    }
    bool operator==(const Iterator& other) const
    {
        return node == other.node;
    }
    bool operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }
};
Iterator begin() const
{
    shared<Node<KeyType,char>> current = this->root;
    while(current && current->left)
    {
        current = current->left;
    }
    return Iterator(current);
}
Iterator end() const
{
    return Iterator();
}

#endif // ITERATOR_H
