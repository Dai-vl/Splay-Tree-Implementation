// Daniela Vidal, 2ºA Ingeniería Informática
// MAR - Práctica sobre Splay Trees

#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_set>
using namespace std;

#define ARCHIVOS 9
#define PRUEBAS 3
// Generador de casos de prueba para el código de Splay Tree

double P = 0.7;            // probabilidad de que se busque o borre un elemento que está en el árbol
char letters[] = "iiibbs"; // mas i's que b's y s's para que haya mas insertados y se puedan probar el resto de operaciones

int operaciones[ARCHIVOS] = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000};

// selecciona un elemento aleatorio del conjunto de elementos del arbol
int seleccionar(const unordered_set<int> &insertados)
{
    int n = rand() % insertados.size();
    auto it = insertados.begin();
    for (int i = 0; i < n; i++)
        it++;
    return *it;
}

// Funciones para generar archivos individuales que tengan n operaciones de inserción, borrado o búsqueda aleatorias
// (los elementos pueden o pueden no estar en el árbol, dependera del valor de Repetidos)------------------------------------------------------------------------
void generarInserciones(unordered_set<int> &insertados, int operaciones, bool repetidos)
{
    char x = 'i';

    cout << operaciones << endl;
    for (int i = 0; i < operaciones; i++)
    {
        int n = rand() % (operaciones * 2) + 1;
        if (!repetidos)
            while (insertados.count(n)) // si el elemento ya esta en el arbol, se genera otro
                n = rand() % (operaciones * 2) + 1;
        insertados.insert(n);
        cout << x << " " << n << endl;
    }
}

void generarBorrados(unordered_set<int> &insertados, int operaciones, bool aux)
{
    // aux es un parametro que no se usa, pero que se necesita para que la funcion tenga el mismo numero de parametros que las otras
    char x = 'b';
    cout << operaciones << endl;

    for (int i = 0; i < operaciones; i++)
    {
        int n = rand() % (operaciones * 2) + 1;
        // numero entre 1 y operaciones*2 para que (+/-) salga la mitad de las veces un numero que esta y
        // la mitad un numero que no esta
        insertados.erase(n);
        cout << x << " " << n << endl;
    }
}

void generarBusquedas(unordered_set<int> &insertados, int operaciones, bool aux)
{
    // aux es un parametro que no se usa, pero que se necesita para que la funcion tenga el mismo numero de parametros que las otras
    char x = 's';
    cout << operaciones << endl;

    for (int i = 0; i < operaciones; i++)
    {
        int n = rand() % (operaciones * 2) + 1;
        cout << x << " " << n << endl;
    }
}

void archivo(string nombre, int number, int a, bool repetidos, unordered_set<int> &insertados, int nOp, void (*funcion)(unordered_set<int> &insertados, int, bool))
{
    // Funcion que abre el archivo correspondiente y escribe en el
    streambuf *coutbuf;
    ofstream out(nombre + to_string(number) + to_string(a) + ".txt");
    coutbuf = cout.rdbuf(out.rdbuf());
    funcion(insertados, nOp, repetidos);
    cout.rdbuf(coutbuf);
    out.close();
}

void archivosPorOperacion(bool repetidos)
{
    string path = "Datos/";
    // Creamos 10 archivos insertar, buscar y borrar con distintos valores de operaciones
    // y ademas para cada valor de operaciones creamos 3 archivos con distintos valores a insertar/ borrar/ buscar
    int number = 0;

    streambuf *coutbuf;

    for (; number < ARCHIVOS; number++)
    {
        // Creamos los archivo insertar y escribimos ahi
        for (int a = 0; a < PRUEBAS; ++a) // 3 archivos por cada valor de operaciones
        {
            unordered_set<int> insertados;
            // Creamos los archivo insertar y escribimos ahi
            string rep = repetidos ? "r" : "nr";
            archivo(path + "i" + rep, number, a, repetidos, insertados, operaciones[number], generarInserciones);
            // Creamos los archivo buscar y escribimos ahi
            archivo(path + "s" + rep, number, a, repetidos, insertados, operaciones[number], generarBusquedas);
            // Creamos los archivo borrar y escribimos ahi
            archivo(path + "b" + rep, number, a, repetidos, insertados, operaciones[number], generarBorrados);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Funciones que genera un archivo con n operaciones aleatorias de insercion, borrado y busqueda mezcladas------------------------------------------------------------------------
void generadorTodasOperaciones(unordered_set<int> &insertados, int operaciones)
{
    cout << operaciones << endl;

    for (int i = 0; i < operaciones; i++)
    {
        char x = letters[rand() % 6];
        int n = rand() % (operaciones * 2) + 1; // Cogera numeros entre 1 y operaciones*2 para que salgan numeros que estan y que no estan un 50% de las veces
        cout << x << " " << n << endl;
    }
}

void archivosTodasOperaciones()
{
    int number = 0;
    string path = "Datos/";
    string name = "todasOperaciones";

    streambuf *coutbuf;

    for (; number < ARCHIVOS; number++)
    {
        for (int i = 0; i < PRUEBAS; ++i)
        {
            // Creamos el archivo insertar y escribimos ahi
            ofstream out(path + name + to_string(number) + to_string(i) + ".txt");
            coutbuf = cout.rdbuf(out.rdbuf());
            unordered_set<int> insertados;
            generadorTodasOperaciones(insertados, operaciones[number]);
            cout.rdbuf(coutbuf);
            out.close();
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    // archivosPorOperacion(true); // archivos con numeros repetidos (se utilizaran para los costes de hacer n operaciones)
    // archivosPorOperacion(false); // archivos sin numeros repetidos (se utilizaran para los costes de cada operacion de insertar, borrar y buscar)
    // archivosTodasOperaciones();

    return 0;
}