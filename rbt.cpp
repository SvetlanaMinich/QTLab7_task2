#include "rbt.h"

template<class T, class T1>
shared<typename rbt<T, T1>::Node> rbt<T, T1>::grandparent(shared<Node> node) {
    shared<Node> p = node->parent;
    if (p && p->parent) {
        return p->parent;
    } else {
        return shared<Node>();
    }
}
template<class T, class T1>
shared<typename rbt<T, T1>::Node> rbt<T, T1>::uncle(shared<Node> node) {
    shared<Node> tmp = grandparent(node);
    if (tmp) {
        if (tmp->left.equals(node->parent)) {
            return tmp->right;
        } else {
            return tmp->left;
        }
    } else {
        return shared<Node>();
    }
}
template<class T, class T1>
shared<typename rbt<T, T1>::Node> rbt<T, T1>::sibling(shared<Node> node) {
    if (!node->parent) {
        return shared<Node>();
    }
    shared<Node> p = node->parent;
    if (node.equals(p->left)) {
        return p->right;
    } else {
        return p->left;
    }
}
template<class T, class T1> // Так же, как и в АВЛ-дереве
void rbt<T, T1>::RotateLeft(shared<Node> node) {
    shared<Node> right = node->right;
    right->parent = node->parent;
    if (node->parent) {
        shared<Node> p = node->parent;
        if (p->left.equals(node)) {
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
template<class T, class T1> // Так же, как и в АВЛ-дереве
void rbt<T, T1>::RotateRight(shared<Node> node) {
    shared<Node> left = node->left;
    left->parent = node->parent;
    if (node->parent) {
        shared<Node> p = node->parent;
        if (p->left.equals(node)) {
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
//вставляем красный узел, а после проверяем все возможные случаи
template<class T, class T1>
void rbt<T, T1>::add(T key, T1 value) {
    shared<Node> tmp = root, p = shared<Node>();
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
    tmp = shared<Node>(new Node(key, value));
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
template<class T, class T1> //если нода - корень, то перекрашиваем в черный
void rbt<T, T1>::InsCase1(shared<Node> node) {
    if (!node->parent) {
        node->color = 1;
    }
    else {
        InsCase2(node);
    }
}
template<class T, class T1>
void rbt<T, T1>::InsCase2(shared<Node> node) {
    if (node->parent.lock()->color) { //если цвет родителя черный, то все ок. иначе идем к 3ему случаю
        return;
    } else {
        InsCase3(node);
    }
}
template<class T, class T1>
void rbt<T, T1>::InsCase3(shared<Node> node) {
    shared<Node> u = uncle(node);
    if (u && !u->color) { //если существует красный дядя
        node->parent.lock()->color = 1;
        u->color = 1;
        shared<Node> g = grandparent(node);
        g->color = 0;
        InsCase1(g);
    } else {
        InsCase4(node);
    }
}
template<class T, class T1>
void rbt<T, T1>::InsCase4(shared<Node> node) {
    shared<Node> g = grandparent(node), tmp = node;
    shared<Node> p = node->parent;
    if (node.equals(p->right) && p.equals(g->left)) {
        RotateLeft(p);
        tmp = node->left;
    } else if (node.equals(p->left) && p.equals(g->right)) {
        RotateRight(p);
        tmp = node->right;
    }
    InsCase5(tmp);
}
template<class T, class T1>
void rbt<T, T1>::InsCase5(shared<Node> node) {
    shared<Node> g = grandparent(node);
    shared<Node> p = node->parent;
    p->color = 1;
    g->color = 0;
    if (node.equals(p->left)) {
        RotateRight(g);
    } else {
        RotateLeft(g);
    }
}
template<class T, class T1>
void rbt<T, T1>::remove(T key) {
    shared<Node> tmp = root ? find(root, key) : shared<Node>(); // Ищем узел с нужным ключем
    // Повезло повезло, ниче делать не надо если такого узла нет
    if (!tmp) {
        return;
    }
    // Случай, когда дерево состоит из одного узла
    if (!tmp->parent && !tmp->left && !tmp->right) {
        root = shared<Node>(); // Обнуляем корень
        return;
    }
    // Если у узла есть и левый и правый потомок
    if (tmp->left && tmp->right)  {
        // Ищем самый правый узел в левом поддереве
        // Этот узел будет ближайшим меньшим к удаляемому, по значнию ключа
        // Что дает нам возможность переместить его значение в удаляемый узел, и удалять уже новый, но у которого будет либо один, либо ноль потомков
        // При таком способе прийдется рассматривать намного меньшее количество вариантов
        shared<Node> removed = tmp->left;
        while (removed->right) {
            removed = removed->right;
        }
        tmp->data.second = removed->data.second;
        tmp->data.first = removed->data.first;
        tmp = removed;
    }
    shared<Node> child = tmp->left ? tmp->left : tmp->right;
    // Ищем потомка, если таково имеется
    // Случай, когда потомков нет
    if (!child) {
        if (tmp->color) {// Удаление красного узла не привело бы к разбалансировке дерева
            DelCase1(tmp); // А вот удаление черного узла нарушает свойства красно-черного дерева об одинаковой длинне черных путей
        }
        shared<Node> p = tmp->parent;

        if (p->left.equals(tmp)) {// Меняем связи, тем самым удаляя узел, после чего тот выйдет из области видимости
            p->left = child;
        } else {
            p->right = child;
        }
    }
    // Случай, когда потомок есть
    else {
        child->parent = tmp->parent; // Меняем связи
        shared<Node> p = tmp->parent;
        // Меняем связи
        if (tmp->parent) {
            if (tmp.equals(p->left)) {
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
template<class T, class T1>
void rbt<T, T1>::DelCase1(shared<Node> node) {
    if (node->parent) {
        DelCase2(node);
    }
}
template<class T, class T1>
void rbt<T, T1>::DelCase2(shared<Node> node) {
    shared<Node> s = sibling(node);
    shared<Node> p = node->parent;
    bool scol = s ? s->color : 1;
    if (!scol) {
        p->color = 0;
        s->color = 1;
        if (node.equals(p->left)) {
            RotateLeft(p);
        } else {
            RotateRight(p);
        }
    }
    DelCase3(node);
}
template<class T, class T1>
void rbt<T, T1>::DelCase3(shared<Node> node) {
    shared<Node> s = sibling(node);
    shared<Node> p = node->parent;
    bool scol = s ? s->color : 1;
    bool lcol = s && s->left ? s->left->color : 1;
    bool rcol = s && s->right ? s->right->color : 1;
    if (p->color && scol && lcol && rcol) {
        if (s) {
            s->color = 0;
        }
        DelCase1(p);
    } else {
        DelCase4(node);
    }
}
template<class T, class T1>
void rbt<T, T1>::DelCase4(shared<Node> node) {
    shared<Node> s = sibling(node);
    shared<Node> p = node->parent;
    bool scol = s ? s->color : 1;
    bool lcol = s && s->left ? s->left->color : 1;
    bool rcol = s && s->right ? s->right->color : 1;
    if (!p->color && scol && lcol && rcol) {
        if (s) {
            s->color = 0;
        }
        p->color = 1;
    } else {
        DelCase5(node);
    }
}
template<class T, class T1>
void rbt<T, T1>::DelCase5(shared<Node> node) {
    shared<Node> s = sibling(node);
    shared<Node> p = node->parent;
    bool lcol = s->left ? s->left->color : 1;
    bool rcol = s->right ? s->right->color : 1;
    if (s->color) {
        if (node.equals(p->left) && rcol && !lcol) {
            s->color = 0;
            s->left->color = 1;
            RotateRight(s);
        } else if (node.equals(p->right) && lcol && !rcol) {
            s->color = 0;
            s->right->color = 1;
            RotateLeft(s);
        }
    }
    DelCase6(node);
}
template<class T, class T1>
void rbt<T, T1>::DelCase6(shared<Node> node) {
    shared<Node>  s = sibling(node);
    shared<Node> p = node->parent;
    s->color = p->color;
    p->color = 1;
    if (node.equals(p->left)) {
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
