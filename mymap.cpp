#include "mymap.h"
template <typename KeyType, typename ValueType>
shared<Node<KeyType,ValueType>> mymap<KeyType,ValueType>::findNode(const KeyType& key)
{
    shared<Node<KeyType,ValueType>> current = this->root;
    while(current != nullptr)
    {
        if(key < current->data.first)
        {
            current = current->left;
        }
        else if (key > current->data.first)
        {
            current = current->right;
        }
        else
        {
            return current;
        }
    }
    return nullptr;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::balanceAfterInsert(shared<Node<KeyType,ValueType>> node)
{
    while(node != this->root && node->parent->color == 0)
    {
        if(node->parent == node->parent->parent->left)
        {
            shared<Node<KeyType,ValueType>> uncle = node->parent->parent->right;
            if(uncle != nullptr && uncle->color == 0)
            {
                node->parent->color = 1;
                uncle->color = 1;
                node->parent->parent->color = 0;
                node = node->parent->parent;
            }
            else
            {
                if(node == node->parent->right)
                {
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = 1;
                node->parent->parent->color = 0;
                rotateRight(node->parent->parent);
            }
        }
        else
        {
            shared<Node<KeyType,ValueType>> uncle = node->parent->parent->left;
            if(uncle != nullptr && uncle->color == 0)
            {
                node->parent->color = 1;
                uncle->color = 1;
                node->parent->parent->color = 0;
                node = node->parent->parent;
            }
            else
            {
                if(node == node->parent->left)
                {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = 1;
                node->parent->parent->color = 1;
                rotateLeft(node->parent->parent);
            }
        }
    }
    this->root->color = 1;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::balanceAfterErase(shared<Node<KeyType,ValueType>> node)
{
    while (node != this->root && node->color == 1) {
        if (node == node->parent->left) {
            shared<Node<KeyType,ValueType>> sibling = node->parent->right;
            if (sibling->color == 0) {
                sibling->color = 1;
                node->parent->color = 0;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == 1 && sibling->right->color == 1) {
                sibling->color = 0;
                node = node->parent;
            } else {
                if (sibling->right->color == 1) {
                    sibling->left->color = 1;
                    sibling->color = 0;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = 1;
                sibling->right->color = 1;
                rotateLeft(node->parent);
                node = this->root;
            }
        } else {
            shared<Node<KeyType,ValueType>> sibling = node->parent->left;
            if (sibling->color == 0) {
                sibling->color = 1;
                node->parent->color = 0;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            if (sibling->left->color == 1 && sibling->right->color == 1) {
                sibling->color = 0;
                node = node->parent;
            } else {
                if (sibling->left->color == 1) {
                    sibling->right->color = 1;
                    sibling->color = 0;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = 1;
                sibling->left->color = 1;
                rotateRight(node->parent);
                node = this->root;
            }
        }
    }

    node->color = 1;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::rotateLeft(shared<Node<KeyType,ValueType>> node)
{
    shared<Node<KeyType,ValueType>> right_child = node->right;
    node->right = right_child->left;
    if(right_child->left != nullptr)
    {
        right_child->left->parent = node;
    }
    right_child->parent = node->parent;
    if(node->parent == nullptr)
    {
        this->root = right_child;
    }
    else if(node == node->parent->left)
    {
        node->parent->left = right_child;
    }
    else
    {
        node->parent->right = right_child;
    }
    right_child->left = node;
    node->parent = right_child;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::rotateRight(shared<Node<KeyType,ValueType>> node) //правый поворот
{
    shared<Node<KeyType,ValueType>> left_child = node->left;
    node->left = left_child->right;
    if (left_child->right != nullptr) {
        left_child->right->parent = node;
    }
    left_child->parent = node->parent;
    if (node->parent == nullptr) {
        this->root = left_child;
    } else if (node == node->parent->left) {
        node->parent->left = left_child;
    } else {
        node->parent->right = left_child;
    }
    left_child->right = node;
    node->parent = left_child;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::deleteSubtree(shared<Node<KeyType,ValueType>> node) //удаление поддерева с корнем node
{
    if(node != nullptr)
    {
        deleteSubtree(node->left);
        deleteSubtree(node->right);
        delete node;
    }
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::insert(const std::pair<const KeyType, ValueType>& data) //вставка элемента
{
    shared<Node<KeyType,ValueType>> current = this->root;
    shared<Node<KeyType,ValueType>> prev = nullptr;
    while (current != nullptr) {
        prev = current;
        if (data.first < current->data.first) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    shared<Node<KeyType,ValueType>> newNode = createNode(data);
    newNode->parent = prev;
    if (prev == nullptr) { //один узел в дереве
        this->root = newNode;
    } else if (data.first < prev->data.first) {
        prev->left = newNode;
    } else {
        prev->right = newNode;
    }
    newNode->prev = prev;
    newNode->next = nullptr;
    if (prev != nullptr) {
        if (newNode->data.first < prev->data.first) {
            while (prev->prev != nullptr && newNode->data.first < prev->prev->data.first) {
                prev = prev->prev;
            }
        } else {
            while (prev->next != nullptr && newNode->data.first > prev->next->data.first) {
                prev = prev->next;
            }
        }
        if (newNode->data.first < prev->data.first) {
            newNode->next = prev;
            newNode->prev = prev->prev;
            if (prev->prev != nullptr) {
                prev->prev->next = newNode;
            }
            prev->prev = newNode;
        } else {
            newNode->prev = prev;
            newNode->next = prev->next;
            if (prev->next != nullptr) {
                prev->next->prev = newNode;
            }
            prev->next = newNode;
        }
    }
    size++;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::erase(const KeyType& key) //удаление элемента
{
    shared<Node<KeyType,ValueType>> node = findNode(key);
    if(node == nullptr)
    {
        return;
    }
    shared<Node<KeyType,ValueType>> replacement = nullptr;
    if(node->left != nullptr && node->right!= nullptr)
    {
        shared<Node<KeyType,ValueType>> successor = node->right; //successor - потомок
        while(successor->left != nullptr)
        {
            successor = successor->left;
        }
        replacement = successor;
        node->data = std::move(successor->data);
        node = successor;
    }
    shared<Node<KeyType,ValueType>> child = (node->left != nullptr) ? node->left : node->right;
    bool original_color = node->color;
    if(child != nullptr)
    {
        child->parent = node->parent;
        if(node->parent == nullptr)
        {
            this->root = child;
        }
        else if(node == node->parent->left)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }
        if(original_color == 1)
        {
            balanceAfterErase(child);
        }
    }
    else if(node->parent == nullptr)
    {
        this->root = nullptr;
    }
    else
    {
        if(original_color == 1)
        {
            balanceAfterErase(node);
        }
        if(node->parent != nullptr)
        {
            if(node == node->parent->left)
            {
                node->parent->left = nullptr;
            }
            else if(node == node->parent->right)
            {
                node->parent->right = nullptr;
            }
        }
    }
    delete node;
    --size;
}
template <typename KeyType, typename ValueType>
ValueType& mymap<KeyType,ValueType>::operator[](const KeyType& key)
{
    shared<Node<KeyType,ValueType>> node = findNode(key);
    if(node == nullptr) //инициализируем этот ключ при помощи конструктора по умолчанию нашего ValueType
    {
        std::pair<const KeyType, ValueType> data(key, ValueType());
        insert(data);
        node = findNode(key);
    }
    return node->data.second;
}
template <typename KeyType, typename ValueType>
void mymap<KeyType,ValueType>::clear()
{
    deleteSubtree(this->root);
    this->root = nullptr;
    size = 0;
}
template <typename KeyType, typename ValueType>
bool mymap<KeyType,ValueType>::empty() const
{
    return size == 0;
}
template <typename KeyType, typename ValueType>
std::size_t mymap<KeyType,ValueType>::getSize() const
{
    return size;
}
