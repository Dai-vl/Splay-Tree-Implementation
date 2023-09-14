// Daniela Vidal, 3ºA Ingeniería Informática
// MAR - Práctica sobre Splay Trees

// Implementar en Java o en C++ un arbol de busqueda autoajustable(splay tree) con las operaciones de buscar, insertar y borrar una clave.
// implementación de la estructura
// Se ha hecho toda la implementación en un solo header para que sea más facil ver qué hace cada operación y cómo están definidas.

#ifndef SplayTree_h
#define SplayTree_h

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

template <class T>
class SplayTree
{
private:
    struct TreeNode
    {
        T elem;
        TreeNode *left;
        TreeNode *right;
        TreeNode *parent;
        TreeNode(const T &e, TreeNode *l, TreeNode *r, TreeNode *p) : elem(e), left(l), right(r), parent(p) {}
    };

    TreeNode *raiz; // puntero a la raiz del arbol

    void rotacionDerecha(TreeNode *p)
    {
        TreeNode *q = p->left;
        p->left = q->right;
        q->right = p;
        q->parent = p->parent;
        if (q->parent != nullptr && q->parent->elem > q->elem)
            q->parent->left = q;
        else if (q->parent != nullptr)
            q->parent->right = q;
        p->parent = q;
        if (p->left != nullptr)
            p->left->parent = p;
    }

    void rotacionIzquierda(TreeNode *p)
    {
        TreeNode *q = p->right;
        p->right = q->left;
        q->left = p;
        q->parent = p->parent;
        if (q->parent != nullptr && q->parent->elem > q->elem)
            q->parent->left = q;
        else if (q->parent != nullptr)
            q->parent->right = q;
        p->parent = q;
        if (p->right != nullptr)
            p->right->parent = p;
    }

    void flotacion(TreeNode *q)
    {
        TreeNode *p = q->parent; // puntero al padre de q

        // 1. si q es la raiz, no se hace nada
        if (p == nullptr)
            return;

        TreeNode *g = p->parent; // puntero al abuelo de q

        while (p != nullptr) // mientras q tenga padre (no sea la raiz) se hace la rotacion correspondiente
        {
            // 2. si q es el hijo directo de la raiz, se hace una rotacion simple (no tiene abuelo)
            if (g == nullptr)
            {
                if (q == p->left)
                    rotacionDerecha(p);
                else
                    rotacionIzquierda(p);
            }
            else // 3. q tiene padre y abuelo
            {
                if (q == p->left)
                {
                    if (p == g->left) // q es el hijo izquierdo de su padre y este es el hijo izquierdo de su abuelo
                    {
                        rotacionDerecha(g);
                        rotacionDerecha(p);
                    }
                    else // q es el hijo izquierdo de su padre y este es el hijo derecho de su abuelo
                    {
                        rotacionDerecha(p);
                        rotacionIzquierda(g);
                    }
                }
                else
                {
                    if (p == g->left) // q es el hijo derecho de su padre y este es el hijo izquierdo de su abuelo
                    {
                        rotacionIzquierda(p);
                        rotacionDerecha(g);
                    }
                    else // q es el hijo derecho de su padre y este es el hijo derecho de su abuelo
                    {
                        rotacionIzquierda(g);
                        rotacionIzquierda(p);
                    }
                }
            }
            p = q->parent; // se actualizan los punteros
            if (p != nullptr)
                g = p->parent;
        }
        raiz = q; // la raiz es q (el nodo que se ha movido a la raiz)
    }

    void insertar(TreeNode *&t, const T &e)
    {
        if (t == nullptr) // si el nodo actual es nulo
        {
            t = new TreeNode(e, nullptr, nullptr, nullptr); // se crea un nuevo nodo con el elemento a insertar
            if (raiz == nullptr)                            // si la raiz es nula, se hace que el nuevo nodo sea la raiz
                raiz = t;
            return;
        }
        if (e < t->elem) // si el elemento a insertar es menor que el elemento del nodo actual
        {
            if (t->left == nullptr) // si el hijo izquierdo es nulo
            {
                t->left = new TreeNode(e, nullptr, nullptr, t);
                flotacion(t->left);
                return;
            }
            else
                insertar(t->left, e); // si el hijo izquierdo no es nulo, se llama a insertar recursivamente
        }
        else if (e > t->elem) // si el elemento a insertar es mayor que el elemento del nodo actual
        {
            if (t->right == nullptr) // si el hijo derecho es nulo
            {
                t->right = new TreeNode(e, nullptr, nullptr, t);
                flotacion(t->right);
                return;
            }
            else
                insertar(t->right, e); // si el hijo derecho no es nulo, se llama a insertar recursivamente
        }
        else // si el nodo con ese valor ya existia se hace una flotacion de ese nodo
            flotacion(t);
    }

    void borrar(TreeNode *&t, const T &e)
    {
        // el origen de flotacion es el padre del nodo borrado, o el último nodo visitado, si el nodo a borrar no se encuentra en el árbol
        int a = 0;

        if (t == nullptr)
            return;

        if (e < t->elem)            // si el elemento a borrar es menor que el elemento del nodo actual
            if (t->left == nullptr) // si el hijo izquierdo es nulo, se hace una flotacion del nodo actual
                flotacion(t);
            else
                borrar(t->left, e); // si no, se sigue buscando en el subárbol izquierdo

        else if (e > t->elem)
            if (t->right == nullptr) // si el hijo derecho es nulo, se hace una flotacion del nodo actual
                flotacion(t);
            else
                borrar(t->right, e); // si no, se sigue buscando en el subárbol derecho

        else if (e == t->elem)
        {
            if (t->right == nullptr)
            {
                if (t->parent == nullptr && t->left == nullptr)
                    raiz = t->parent;
                else if (t->parent != nullptr)
                {
                    TreeNode *x = t->parent;
                    if (t->parent->elem > t->elem)
                    {
                        t->parent->left = t->left;
                    }
                    else
                    {
                        t->parent->right = t->left;
                    }
                    if (x->left != nullptr)
                        x->left->parent = x;
                    flotacion(x);
                }
                else
                {
                    if (t->left != nullptr)
                        t->left->parent = t->parent;
                    raiz = t->left;
                }
            }
            else
            {
                pair<TreeNode *, TreeNode *> min = borraMin(t->right);
                TreeNode *x = t;
                min.first->left = t->left;
                min.first->right = min.second;
                t = min.first;
                t->parent = x->parent;

                if (t->left != nullptr)
                    t->left->parent = t;
                if (t->right != nullptr)
                    t->right->parent = t;
                if (t->parent == nullptr)
                    raiz = t;
                else
                    flotacion(t->parent);
            }
        }
    }

    // t' t.q. elems(t') = elems(t) - {e}, e = min(t)
    pair<TreeNode *, TreeNode *> borraMin(TreeNode *&t)
    { // Función auxiliar para borrar, basado en los apuntes de la asignatura
        if (t->left == nullptr)
        {
            TreeNode *x = t;
            TreeNode *derecha = t;
            // borrar de t el nodo x
            if (t->right != nullptr)
            {
                t->right->parent = t->parent;
                derecha = t->right;
            }
            if (t->parent->elem > t->elem)
                t->parent->left = t->right;
            else
                t->parent->right = t->right;
            // el minimo es x
            return {x, t}; // se devuelve x que sera el minimo y el arbol sin el minimo
        }
        else
        {
            pair<TreeNode *, TreeNode *> min = borraMin(t->left);
            TreeNode *x = t;
            if (min.second != nullptr)
                min.second->parent = x;
            x->left = min.second;
            return {min.first, x};
        }
    }

    TreeNode *buscar(const T &e, TreeNode *t)
    {
        if (t == nullptr)
            return nullptr;
        if (e < t->elem)
        {
            if (t->left == nullptr) // si el nodo no se encuentra en el árbol se hace una flotacion del último nodo visitado
                flotacion(t);
            else
                return buscar(e, t->left); // si el valor es menor que el actual se busca en el subárbol izquierdo
        }
        else if (e > t->elem)
        {
            if (t->right == nullptr) // si el nodo no se encuentra en el árbol se hace una flotacion del último nodo visitado
                flotacion(t);
            else
                return buscar(e, t->right); // si el valor es mayor que el actual se busca en el subárbol derecho
        }
        else
            flotacion(t); // si el nodo con ese valor se encuentra se hace una flotacion de ese nodo
        return t;
    }

    void printTree(const string &prefix, const TreeNode *t, bool izquierdo)
    {
        // Fuente : TianTian 2 et al. (2019) How to print binary tree diagram in Java?,
        // Stack Overflow. Disponible en: https://stackoverflow.com/questions/4965335/how-to-print-binary-tree-diagram (Visitado: diciembre 7, 2022).
        // (modificado)
        // Imprime el árbol en consola de forma recursiva con un formato de árbol
        if (t != nullptr)
        {
            cout << prefix;
            if (t->parent == nullptr)
                cout << "┌─> R ";
            else if (izquierdo)
                cout << "├─> I ";
            else
                cout << "└─> D ";
            cout << t->elem << endl;
            printTree(prefix + (izquierdo ? "│   " : "    "), t->left, true);
            printTree(prefix + (izquierdo ? "│   " : "    "), t->right, false);
        }
    }

    void printTreeInfo(const TreeNode *t, int nivel)
    {
        // Imprime el árbol en consola de forma recursiva con un formato de lista
        if (t != nullptr)
        {
            cout << "Nodo: " << t->elem;
            cout << " Padre: " << (t->parent == nullptr ? "[vacio]" : to_string(t->parent->elem));
            cout << " Hijo izquierdo: " << (t->left == nullptr ? "[vacio]" : to_string(t->left->elem));
            cout << " Hijo derecho: " << (t->right == nullptr ? "[vacio]" : to_string(t->right->elem));
            cout << endl;
            printTreeInfo(t->left, nivel + 1);
            printTreeInfo(t->right, nivel + 1);
        }
    }

    vector<T> inorder(TreeNode *t)
    { // Función auxiliar para inorder que devuelve un vector con los elementos del árbol
        vector<T> v;
        if (t != nullptr)
        {
            vector<T> v1 = inorder(t->left);
            v.insert(v.end(), v1.begin(), v1.end());
            v.push_back(t->elem);
            vector<T> v2 = inorder(t->right);
            v.insert(v.end(), v2.begin(), v2.end());
        }
        return v;
    }

public:
    SplayTree() : raiz(nullptr) {}                                              // constructora vacía
    SplayTree(const T &e) : raiz(new TreeNode(e, nullptr, nullptr, nullptr)) {} // constructora con un elemento
    void insertar(const T &e) { insertar(raiz, e); }
    void borrar(const T &e) { borrar(raiz, e); }
    void printTree()
    {
        printTree("", raiz, false);
        cout << endl;
    }
    void printTreeInfo()
    {
        printTreeInfo(raiz, 0);
        cout << endl;
    }
    bool buscar(const T &e)
    {
        TreeNode *n = buscar(e, raiz);
        if (n == nullptr || n->elem != e)
            return false;
        return true;
    }
    vector<T> inorder() { return inorder(raiz); }
};

#endif