#ifndef RBT_H
#define RBT_H
#include <utility> // Необходимо включить заголовочный файл для std::pair
#include <memory>

template<class T, class T1>
struct Node {
    Node(T nKey, T1 nValue) : data(nKey,nValue),
        left(nullptr),
        right(nullptr),
        next(nullptr),
        prev(nullptr),
        color(false){}
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::weak_ptr<Node> parent; // weak_ptr во избежания циклических ссылок
    bool color; // red - false, black - true
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;
    std::pair<T,T1> data;
};
template <class T, class T1>
class Iterator
{
private:
    std::shared_ptr<Node<T,T1>> node;
public:
    Iterator() : node(nullptr){}
    Iterator(std::shared_ptr<Node<T,T1>> node) : node(node){}
    std::pair<T, T1>& operator*() const
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
            std::shared_ptr<Node<T,T1>> parent = node->parent.lock();
            while(parent != nullptr && node == parent->right)
            {
                node = parent;
                parent = parent->parent.lock();
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
template<class T, class T1>
class rbt
{
protected:
    std::shared_ptr<Node<T,T1>> root;
    std::shared_ptr<Node<T,T1>> grandparent(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        if (p && p->parent.expired()) {
            return p->parent.lock();
        } else {
            return std::shared_ptr<Node<T,T1>>();
        }
    }
    std::shared_ptr<Node<T,T1>> uncle(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> tmp = grandparent(node);
        if (tmp) {
            if (tmp->left==node->parent.lock()) {
                return tmp->right;
            } else {
                return tmp->left;
            }
        } else {
            return std::shared_ptr<Node<T,T1>>();
        }
    }
    std::shared_ptr<Node<T,T1>> sibling(std::shared_ptr<Node<T,T1>> node)
    {
        if (!(node->parent.expired())) {
            return std::shared_ptr<Node<T,T1>>();
        }
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        if (node==p->left) {
            return p->right;
        } else {
            return p->left;
        }
    }
    void RotateLeft(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> right = node->right;
        right->parent = node->parent;
        if (node->parent.expired()) {
            std::shared_ptr<Node<T,T1>> p = node->parent.lock();
            if (p->left==node) {
                p->left = right;
            } else {
                p->right = right;
            }
        } else {
            root = right;
        }
        node->right = right->left;
        if (right->left) {
            right->left->parent = node;
        }
        node->parent = right;
        right->left = node;
    }
    void RotateRight(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> left = node->left;
        left->parent = node->parent;
        if (node->parent.expired()) {
            std::shared_ptr<Node<T,T1>> p = node->parent.lock();
            if (p->left==node) {
                p->left = left;
            } else {
                p->right = left;
            }
        } else {
            root = left;
        }
        node->left = left->right;
        if (left->right) {
            left->right->parent = node;
        }
        node->parent = left;
        left->right = node;
    }
    void InsCase1(std::shared_ptr<Node<T,T1>> node)
    {
        if (!node->parent.expired()) {
            node->color = true;
        }
        else {
            InsCase2(node);
        }
    }
    void InsCase2(std::shared_ptr<Node<T,T1>> node)
    {
        //если цвет родителя черный, то все ок. иначе идем к 3ему случаю
        if (node && !node->parent.expired()){
            std::shared_ptr<Node<T, T1>> parent = node->parent.lock();
            if (parent->color) {
                return;
            } else {
                InsCase3(node);
            }
        }
    }
    void InsCase3(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> u = uncle(node);
        if (u && !(u->color)) { //если существует красный дядя
            node->parent.lock()->color = 1;
            u->color = 1;
            std::shared_ptr<Node<T,T1>> g = grandparent(node);
            g->color = 0;
            InsCase1(g);
        } else {
            InsCase4(node);
        }
    }
    void InsCase4(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> g = grandparent(node);
        std::shared_ptr<Node<T,T1>> tmp = node;
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        if(p && g)
        {
            if (node==p->right && p==g->left) {
                RotateLeft(p);
                tmp = node->left;
            } else if (node==p->left && p==g->right) {
                RotateRight(p);
                tmp = node->right;
            }
            InsCase5(tmp);
        }
    }
    void InsCase5(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> g = grandparent(node);
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        p->color = 1;
        g->color = 0;
        if (g && p && node==p->left) {
            RotateRight(g);
        } else {
            RotateLeft(g);
        }
    }
    void DelCase1(std::shared_ptr<Node<T,T1>> node)
    {
        if (node->parent.expired()) {
            DelCase2(node);
        }
    }
    void DelCase2(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> s = sibling(node);
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        bool scol = s ? s->color : 1;
        if (!scol && p) {
            p->color = 0;
            s->color = 1;
            if (node==p->left) {
                RotateLeft(p);
            } else {
                RotateRight(p);
            }
        }
        DelCase3(node);
    }
    void DelCase3(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> s = sibling(node);
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        bool scol = s ? s->color : 1;
        bool lcol = s && s->left ? s->left->color : 1;
        bool rcol = s && s->right ? s->right->color : 1;
        if (p && p->color && scol && lcol && rcol) {
            if (s) {
                s->color = 0;
            }
            DelCase1(p);
        } else {
            DelCase4(node);
        }
    }
    void DelCase4(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> s = sibling(node);
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        bool scol = s ? s->color : 1;
        bool lcol = s && s->left ? s->left->color : 1;
        bool rcol = s && s->right ? s->right->color : 1;
        if (p && !p->color && scol && lcol && rcol) {
            if (s) {
                s->color = 0;
            }
            p->color = 1;
        } else {
            DelCase5(node);
        }
    }
    void DelCase5(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>> s = sibling(node);
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        bool lcol = s->left ? s->left->color : 1;
        bool rcol = s->right ? s->right->color : 1;
        if (s->color) {
            if (p && node==p->left && rcol && !lcol) {
                s->color = 0;
                s->left->color = 1;
                RotateRight(s);
            } else if (p && node==p->right && lcol && !rcol) {
                s->color = 0;
                s->right->color = 1;
                RotateLeft(s);
            }
        }
        DelCase6(node);
    }
    void DelCase6(std::shared_ptr<Node<T,T1>> node)
    {
        std::shared_ptr<Node<T,T1>>  s = sibling(node);
        std::shared_ptr<Node<T,T1>> p = node->parent.lock();
        s->color = p->color;
        p->color = 1;
        if (p && node==p->left) {
            if (s->right) {
                s->right->color = 1;
            }
            RotateLeft(p);
        } else {
            if (s->left) {
                s->left->color = 1;
            }
            RotateRight(p);
        }
    }
    std::shared_ptr<Node<T,T1>> find(std::shared_ptr<Node<T,T1>> node, T key)
    {
        std::shared_ptr<Node<T,T1>> current = root;
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
public:
    rbt()
    {
        root = nullptr;
    }
    void add(T key, T1 value)
    {
        std::shared_ptr<Node<T,T1>> tmp = root;
        std::shared_ptr<Node<T,T1>> p = std::shared_ptr<Node<T,T1>>();
        // Спускаемся по дереву, ища, куда вставить новый узел
        while (tmp) {
            p = tmp;

            if (tmp->data.first > key) {
                tmp = tmp->left;
            } else if (tmp->data.first < key) {
                tmp = tmp->right;
            } else {
                return;// Если такой узел уже есть, то прото выходим
            }
        }
        tmp = std::make_shared<Node<T,T1>>(key, value);
        tmp->parent = p;
        if (p) {
            if (tmp->data.first > p->data.first) {
                p->right = tmp;
            } else {
                p->left = tmp;
            }
        } else {
            root = tmp;
        }
        InsCase1(tmp);
        return;
    }
    void remove(T key)
    {
        std::shared_ptr<Node<T,T1>> tmp = root ? find(root, key) : std::shared_ptr<Node<T,T1>>(); // Ищем узел с нужным ключем
        // Повезло повезло, ниче делать не надо если такого узла нет
        if (!tmp) {
            return;
        }
        // Случай, когда дерево состоит из одного узла
        if (tmp->parent.expired() && !tmp->left && !tmp->right) {
            root = std::shared_ptr<Node<T,T1>>(); // Обнуляем корень
            return;
        }
        // Если у узла есть и левый и правый потомок
        if (tmp->left && tmp->right)  {
            // Ищем самый правый узел в левом поддереве
            // Этот узел будет ближайшим меньшим к удаляемому, по значнию ключа
            // Что дает нам возможность переместить его значение в удаляемый узел, и удалять уже новый, но у которого будет либо один, либо ноль потомков
            // При таком способе прийдется рассматривать намного меньшее количество вариантов
            std::shared_ptr<Node<T,T1>> removed = tmp->left;
            while (removed->right) {
                removed = removed->right;
            }
            tmp->data.second = removed->data.second;
            tmp->data.first = removed->data.first;
            tmp = removed;
        }
        std::shared_ptr<Node<T,T1>> child = tmp->left ? tmp->left : tmp->right;
        // Ищем потомка, если таково имеется
        // Случай, когда потомков нет
        if (!child) {
            if (tmp->color) {// Удаление красного узла не привело бы к разбалансировке дерева
                DelCase1(tmp); // А вот удаление черного узла нарушает свойства красно-черного дерева об одинаковой длинне черных путей
            }
            std::shared_ptr<Node<T,T1>> p = tmp->parent.lock();

            if (p && p->left==tmp) {// Меняем связи, тем самым удаляя узел, после чего тот выйдет из области видимости
                p->left = child;
            } else {
                p->right = child;
            }
        }
        // Случай, когда потомок есть
        else {
            child->parent = tmp->parent; // Меняем связи
            std::shared_ptr<Node<T,T1>> p = tmp->parent.lock();
            // Меняем связи
            if (!tmp->parent.expired()) {
                if (p && tmp==p->left) {
                    p->left = child;
                } else {
                    p->right = child;
                }
            } else {
                root = child; // Если надо, меняем корень дерева
                // Если удаляемы узел был красным, то никакие свойства не нарушаются
                if (tmp->color) {
                    if (!child->color) {// Если удаляемый узел был черным, а потомок красный, то можем сохранить свойстав дерева, перекрасив потомка в черныйё
                        child->color = 1;
                    } else {
                        DelCase1(child);
                    }
                }
            }
        }
    }
    void deleteSubtree(std::shared_ptr<Node<T,T1>> node)
    {
        if(node != nullptr)
        {
            deleteSubtree(node->left);
            deleteSubtree(node->right);
        }
    }
    void clear()
    {
        deleteSubtree(root);
        root = nullptr;
    }
    bool empty() const
    {
        return !root;
    }
    Iterator<T,T1> begin()
    {
        std::shared_ptr<Node<T,T1>> leftmost = this->root;
        while(leftmost != nullptr && leftmost->left != nullptr) //ищем самый маленький элемент
        {
            leftmost = leftmost->left;
        }
        return Iterator<T,T1>(leftmost);
    }
    Iterator<T,T1> end()
    {
        return Iterator<T,T1>(nullptr);
    }
    ~rbt()
    {
        clear();
    }
};


#endif // RBT_H
