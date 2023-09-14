// Daniela Vidal, 3ºA Ingeniería Informática
// MAR - Práctica sobre Splay Trees

// Código principal para probar el funcionamiento del SplayTree (tanto tiempos como por consola)

#include "SplayTree.h"
#include <time.h>

using namespace std;

#define ARCHIVOS 9
#define PRUEBAS 3
#define TODAS "todasOperaciones"

// Funciones para calcular el coste de las operaciones
double bucleOperaciones(SplayTree<int> &t);
double tiempoNOperaciones(SplayTree<int> &t, int aux, char f);
double tiempoFunciones(SplayTree<int> &t, int aux, char f);
double costePrueba(char f, bool arbolVacio, string arbolOriginal, string name, int media, int &nodos, double (*funcionTiempo)(SplayTree<int> &t, int, char));
void calculoCoste(double (*funcionTiempo)(SplayTree<int> &t, int aux, char f), string repetidos, bool arbolVacio, string tipo);
// Funcion para probar el funcionamiento del SplayTree
void probarPorConsola();
void imprimirCoste(double (*funcionTiempo)(SplayTree<int> &t, int aux, char f), string repetidos, bool arbolVacio, string tipo);

int main()
{
    int opcion;
    do
    {
        cout << "\nElija una opción" << endl;
        cout << "1: Probar la implementación a mano" << endl;
        cout << "2: Tiempo de n operaciones de inserción, n de borrado y n de búsqueda" << endl;
        cout << "3: Tiempo de una inserción, un borrado y una búsqueda en árboles de tamaño n" << endl;
        cout << "4: Tiempo de n operaciones aleatorias" << endl;
        cout << "\n";
        cout << "Introduce operacion (0 para salir): ";
        cin >> opcion;
        switch (opcion)
        {
        case 0:
            return 0;
        case 1:
            probarPorConsola();
            break;
        case 2:
            cout << "\nTiempo de n operaciones de inserción, n de borrado y n de búsqueda" << endl;
            imprimirCoste(tiempoNOperaciones, "r", true, ""); // calcula el Tiempo de n operaciones de insercion, n de borrado y n de busqueda
            break;
        case 3:
            cout << "\nTiempo de inserción, borrado y búsqueda en árboles de tamaño n" << endl;
            imprimirCoste(tiempoFunciones, "nr", false, ""); // calcula el Tiempo de insercion, borrado y búsqueda de un arbol con n nodos (nr = no repetidos para mas informacion referirse al generador)
            break;
        case 4:
            cout << "\nTiempo de n operaciones aleatorias" << endl;
            imprimirCoste(tiempoNOperaciones, "nr", true, TODAS); // calcula el Tiempo de insercion, borrado y búsqueda de un arbol con n nodos (nr = no repetidos para mas informacion referirse al generador)
            break;
        default:
            cout << "\nOpción no válida" << endl;
            break;
        }
    } while (opcion != 0);

    return 0;
}

double bucleOperaciones(SplayTree<int> &t)
{
    double tiempo = 0;
    clock_t start, end;
    char op;
    cin >> op;
    int n = 0;
    while (cin)
    {
        int e;
        cin >> e;
        switch (op)
        {
        case 'i':
            start = clock();
            t.insertar(e);
            end = clock();
            tiempo += ((double)(end - start)) / CLOCKS_PER_SEC;
            break;
        case 'b':
            start = clock();
            t.borrar(e);
            end = clock();
            tiempo += ((double)(end - start)) / CLOCKS_PER_SEC;
            break;
        case 's':
            start = clock();
            t.buscar(e);
            end = clock();
            tiempo += ((double)(end - start)) / CLOCKS_PER_SEC;
            break;
        }
        cin >> op;
    }
    return tiempo;
}

double tiempoNOperaciones(SplayTree<int> &t, int aux, char f)
{
    // Funcion que mide el tiempo de n operaciones
    // aux y f no se usan pero son necesarios para que la funcion tenga el mismo tipo que la funcion que mide el tiempo de cada operacion
    return bucleOperaciones(t);
}

double tiempoFunciones(SplayTree<int> &t, int n, char f)
{
    // Funcion que mide el tiempo de insercion/borrado/busqueda (f) de un arbol con n nodos
    // metemos elementos tanto que esten como que no esten y vemos el coste de cada operacion
    // el arbol tendra elementos cuyos valores estan entre 0 y n, por lo que los valores que metemos estan dentro y fuera de ese rango
    double tiempo = 0;
    int rango = 10;

    for (int i = n - rango; i < n + rango; ++i)
    {
        clock_t start, end;
        if (f == 'i')
        {
            start = clock();
            t.insertar(i);
            end = clock();
            t.borrar(i); // borramos el elemento que hemos insertado para que el arbol vuelva a tener n nodos
        }
        else if (f == 'b')
        {
            start = clock();
            t.borrar(i);
            end = clock();
            t.insertar(i); // insertamos el elemento que hemos borrado para que el arbol vuelva a tener n nodos
        }
        else if (f == 's')
        {
            start = clock();
            t.buscar(i);
            end = clock();
        }
        tiempo = ((double)(end - start)) / CLOCKS_PER_SEC + tiempo;
    }

    return tiempo;
}

double costePrueba(char f, bool arbolVacio, string arbolOriginal, string name, int media, int &nodos, double (*funcionTiempo)(SplayTree<int> &t, int, char))
{
    double tiempo = 0;
    SplayTree<int> t;
    ifstream in;
    streambuf *cinbuf;
    if (!arbolVacio)
    { // depende del valor arbol vacio o no vacio, leemos el arbol original o no y lo preparamos para las pruebas
        in.open(arbolOriginal);
        cinbuf = cin.rdbuf(in.rdbuf());
        cin >> nodos; // numero de nodos
        bucleOperaciones(t);
        cin.rdbuf(cinbuf);
        in.close();
    }

    in.open(name);
    cinbuf = cin.rdbuf(in.rdbuf());
    cin >> nodos; // lee los nodos que van a ser los mismos que los del arbol original

    for (int j = 0; j < media; ++j)
    { // Hago la media de 100 pruebas para que el tiempo sea mas preciso
        SplayTree<int> tAux = t;
        tiempo = funcionTiempo(tAux, nodos, f) + tiempo;
    }
    tiempo = tiempo / media;
    in.close();
    cin.rdbuf(cinbuf);
    return tiempo;
}

void calculoCoste(double (*funcionTiempo)(SplayTree<int> &t, int aux, char f), string repetidos, bool arbolVacio, string tipo)
{
    // Funcion que lee de un archivo de datos y mide los tiempos de dependiendo de el tipo de medición que se quiera hacer y la funcion que se le pase
    int media = 100;
    int n;

    string path = "Datos/";
    if (tipo != TODAS)
    {
        for (int i = 0; i < ARCHIVOS; i++)
        {
            double timeInsertar = 0, timeBorrar = 0, timeBuscar = 0;

            for (int a = 0; a < PRUEBAS; ++a) // medidas para cada punto de la grafica
            {
                // Insertar
                timeInsertar += costePrueba('i', arbolVacio, path + "i" + repetidos + to_string(i) + to_string(a) + ".txt", path + "i" + repetidos + to_string(i) + to_string(a) + ".txt", media, n, funcionTiempo);

                // Buscar
                timeBuscar += costePrueba('s', false, path + "i" + repetidos + to_string(i) + to_string(a) + ".txt", path + "s" + repetidos + to_string(i) + to_string(a) + ".txt", media, n, funcionTiempo);

                // Borrar
                timeBorrar += costePrueba('b', false, path + "i" + repetidos + to_string(i) + to_string(a) + ".txt", path + "b" + repetidos + to_string(i) + to_string(a) + ".txt", media, n, funcionTiempo);
            }

            // Calculo de la media de las pruebas
            double tIns, tBor, tBus;
            tIns = timeInsertar / PRUEBAS;
            tBor = timeBorrar / PRUEBAS;
            tBus = timeBuscar / PRUEBAS;

            // Datos que se van a usar para la grafica
            cout << n << "   " << tIns;
            cout << "   " << tBor;
            cout << "   " << tBus << endl;
        }
    }
    else
    {
        for (int i = 0; i < ARCHIVOS; i++)
        {
            double time = 0;
            for (int a = 0; a < PRUEBAS; ++a) // medidas para cada punto de la grafica
            {
                time += costePrueba('.', arbolVacio, path + TODAS + to_string(i) + to_string(a) + ".txt", path + TODAS + to_string(i) + to_string(a) + ".txt", media, n, funcionTiempo);
            }
            double t = time / PRUEBAS;
            cout << n << "   " << t << endl;
        }
    }
}

void imprimirCoste(double (*funcionTiempo)(SplayTree<int> &t, int aux, char f), string repetidos, bool arbolVacio, string tipo)
{
    // Funcion que imprime los datos para la grafica
    if (tipo == TODAS)
        cout << "N  |  Tiempo" << endl;
    else
        cout << "N  |  Insertar  |  Borrar  |  Buscar" << endl;
    cout << "___________________________________________________" << endl;
    calculoCoste(funcionTiempo, repetidos, arbolVacio, tipo);
}

void probarPorConsola()
{
    cout << "--------------------------------" << endl;
    cout << "PRUEBA A MANO (para enteros)" << endl;
    cout << "\ni: insertar\nb: borrar\ns: buscar" << endl;
    cout << "Funciones adicionales:\n";
    cout << "a: imprimir en forma de arbol,";
    cout << " f: imprimir información de los nodos,";
    cout << " l: imprimir en lista en inorder,";
    cout << " r: empezar un nuevo arbol" << endl;
    cout << endl;
    cout << "Introduce operacion (0 para salir): ";
    SplayTree<int> t;

    char op = ' ';
    cin >> op;
    while (op != '0')
    {
        int e;
        switch (op)
        {
        case 'i':
            cout << "Introduce un número: ";
            cin >> e;
            t.insertar(e);
            cout << "\nInsertar " << e << endl;
            break;
        case 'b':
            cout << "Introduce un número: ";
            cin >> e;
            t.borrar(e);
            cout << "\nBorrar " << e << endl;
            break;
        case 's':
            cout << "Introduce un número: ";
            cin >> e;
            cout << "\nBuscar " << e << endl;
            if (t.buscar(e))
                cout << "Encontrado" << endl;
            else
                cout << "No encontrado" << endl;
            break;
        case 'a':
            t.printTree();
            break;
        case 'f':
            t.printTreeInfo();
            break;
        case 'l':
            for (int e : t.inorder())
                cout << e << " ";
            cout << endl;
            break;
        case 'r':
            t = SplayTree<int>();
            cout << "Arbol reiniciado" << endl;
            break;
        default:
            cout << "Operacion no valida" << endl;
            break;
        }
        cout << "\nIntroduce operacion (0 para salir): ";
        cin >> op;
    }

    cout << "Terminado correctamente" << endl;
}
