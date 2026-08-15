
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>                                                                                           
#include <map>
#include <set>                             
#include <queue> 
                                   
using namespace std;

// Estructura para la fecha de envio
struct Fecha {
    int dia;
    int mes;
    int anio;

    // Funcion auxiliar para imprimir la fecha
    string toString() const {
        stringstream ss;
        ss << setw(2) << setfill('0') << dia << "/"
           << setw(2) << setfill('0') << mes << "/"
           << anio;
        return ss.str();
    }
};

// Estructura para el producto
struct Producto {
    string nombre;
    double precio; 
};

// Estructura principal del registro de envio
struct Envio {
    int id;
    string cliente_nombre;
    Producto producto;
    Fecha fecha_envio;

    // Operador de comparacion para min_element/max_element nasado en precio
    bool operator<(const Envio& otro) const {
        return producto.precio < otro.producto.precio;
    }
    // Operador de comparacion para ordenar o usar en contenedores
    bool operator>(const Envio& otro) const {
        return producto.precio > otro.producto.precio;
    }
};

// Comparador para la priority queue por precio mas alto
struct ComparadorEnvio {
    bool operator()(const Envio& a, const Envio& b) const {
        return a.producto.precio < b.producto.precio;
    }
};

// Vector (almacenamiento principal) de envios
vector<Envio> registros_envios;

// Array simple para generar IDs 
int ID_ARRAY[100] = {0};
int next_id_index = 0;

// Queue para envios pendientes de verificacion
queue<Envio> envios_en_espera;

// Priority queue para envios urgentes (por precio mas alto)
priority_queue<Envio, vector<Envio>, ComparadorEnvio> envios_urgentes;

// Funcion para registrar un nuevo envio
void registrarEnvio() {
    if (next_id_index >= 100) {
        cout << "Error: Limite de registros alcanzado." << endl;
        return;
    }

    int n;
    cout << "\nIngrese el numero de envios que desea registrar: ";
    while(!(cin >> n) || n <= 0){
        cout << "Entrada no valida. Ingrese un numero entero positivo: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Limpiar buffer

    for(int i = 0; i < n; i++){
        if(next_id_index >=100){
            cout << "\nSe alcanzo el mimite maximo de resgistro (100 maximo)." << endl;
            break;
        }

        Envio nuevo_envio;
        ID_ARRAY[next_id_index] = next_id_index + 1;
        nuevo_envio.id = ID_ARRAY[next_id_index];
        next_id_index++;

        cout << "\n--- REGISTRO DE ENVIO " << (i + 1) << " DE " << n << " ---" << endl;
        cout << "ID de envio: " << nuevo_envio.id << endl;

        cout << "Nombre del cliente: ";
        getline(cin, nuevo_envio.cliente_nombre);

        cout << "Nombre del producto: ";
        getline(cin, nuevo_envio.producto.nombre);

        cout << "Precio del producto en soles (use . para decimales): ";
        while (!(cin >> nuevo_envio.producto.precio) || nuevo_envio.producto.precio <= 0) {
            if(cin.fail()){
            cout << "Entrada invalida. Ingrese un numero para el precio: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }else{
                cout << "Precio no valido. Ingrese de nuevo: ";
            } 
        }
    
        cout << "Fecha de entrega (DD/MM/AAAA): ";
        string fecha_str;
        char slash;
    
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, fecha_str);
        stringstream ss(fecha_str);

        if (!(ss >> nuevo_envio.fecha_envio.dia >> slash && slash == '/' 
          && ss >> nuevo_envio.fecha_envio.mes >> slash && slash == '/' 
          && ss >> nuevo_envio.fecha_envio.anio)) 
        {
        cout << "Formato de fecha incorrecto. Use DD/MM/AAAA por defecto." << endl;
        nuevo_envio.fecha_envio.dia = 1;
        nuevo_envio.fecha_envio.mes = 1;
        nuevo_envio.fecha_envio.anio = 2000;
        }
    

        registros_envios.push_back(nuevo_envio);
        envios_en_espera.push(nuevo_envio); 
        envios_urgentes.push(nuevo_envio);  
    
        cout << "\nEnvio " << nuevo_envio.id << " registrado exitosamente." << endl;
    }
    cout << "\n--- TODOS LOS ENVIOS HAN SIDO REGISTRADOS EXITOSAMENTE ---\n";
}

// Funcion para mostrar todos los envios
void mostrarEnvios() {
    if (registros_envios.empty()) {
        cout << "\nNo hay envios registrados." << endl;
        return;
    }

    cout << "\n============================================================================================================" << endl;
    cout << "                               LISTA DE ENVIOS REGISTRADOS" << endl;
    cout << "============================================================================================================" << endl;
    
    cout << setw(5) << left << "ID"
              << setw(25) << "CLIENTE"
              << setw(40) << "PRODUCTO"
              << setw(25) << "PRECIO (SOLES)" 
              << setw(25) << "FECHA" << endl;
    cout << "------------------------------------------------------------------------------------------------------------" << endl;

    // Uso de for_each para iterar y mostrar
    for_each(registros_envios.begin(), registros_envios.end(), [](const Envio& e) {
        cout << setw(5) << left << e.id
                  << setw(25) << e.cliente_nombre
                  << setw(40) << e.producto.nombre
                  << setw(25) << fixed << setprecision(2) << e.producto.precio
                  << setw(25) << e.fecha_envio.toString() << endl;
    });

    cout << "============================================================================================================" << endl;
}

//Funcion para buscar registros mediante el nombre del cliente
void buscarCliente(){
    if(registros_envios.empty()){
        cout << "\nNo hay envios registrados para buscar." << endl;
        return;
    }

    string nombre_buscar;
    cout << "\nIngrese el nombre del cliente: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nombre_buscar);

    bool encontrado = false;

    cout << "\n=================================================================================================================" << endl;
    cout << "                      RESULTADOS DE BUSQUEDA PARA: " << nombre_buscar << endl;
    cout << "=================================================================================================================" << endl;
    cout << setw(5) << left << "ID"
        << setw(25) << "CLIENTE"
        << setw(40) << "PRODUCTO"
        << setw(25) << "PRECIO"
        << setw(25) << "FECHA DE ENTREGA" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;

    for(const auto& envio : registros_envios){
        if(envio.cliente_nombre == nombre_buscar){
            cout << setw(5) << left << envio.id
                << setw(25) << envio.cliente_nombre
                << setw(40) << envio.producto.nombre
                << setw(25) << fixed << setprecision(2) << envio.producto.precio
                << setw(25) << envio.fecha_envio.toString() << endl;
                encontrado = true;
        }
    }

    if(!encontrado){
        cout << "No se encontraron envios registrados para el cliente " << nombre_buscar << "." << endl;
    }else{
        cout << "=================================================================================================================" << endl;
        cout << " ------ SE HAN MOSTRADO TODOS LOS RESULTADOS ------" << endl;
    }
}

// Funcion para mostrar analisis de envios
void analisisEnvios() {
    if (registros_envios.empty()) {
        cout << "\nNo hay datos para el analisis de envios." << endl;
        return;
    }

    cout << "\n--- ANALISIS DE ENVIOS ---" << endl;
    
    // Uso de map para contar envios por cliente
    map<string, int> conteo_clientes;
    for (const auto& envio : registros_envios) {
        conteo_clientes[envio.cliente_nombre]++;
    }
    cout << "\n1. CONTEO DE ENVIOS POR CLIENTE: " << endl;
    for (const auto& par : conteo_clientes) {
        cout << " - " << par.first << ": " << par.second << " envios" << endl;
    }

    // Uso de multimap para organizar envios por cliente
    multimap<string, Envio> envios_por_cliente;
    for (const auto& envio : registros_envios) {
        envios_por_cliente.insert({envio.cliente_nombre, envio});
    }
    cout << "\n2. AGRUPACION POR CLIENTE: " << endl;
    for (const auto& par : envios_por_cliente) {
        cout << " - " << par.first << " (ID: " << par.second.id << ", Prod: " << par.second.producto.nombre << ")" << endl;
    }

    // Uso de set para obtener productos unicos
    set<string> productos_unicos;
    for (const auto& envio : registros_envios) {
        productos_unicos.insert(envio.producto.nombre);
    }
    cout << "\n3. PRODUCTOS UNICOS: " << endl;
    for (const auto& producto : productos_unicos) {
        cout << " - " << producto << endl;
    }

    // Encontrar el envio con el precio mas bajo y mas alto
    auto envio_mas_barato = min_element(registros_envios.begin(), registros_envios.end());
    auto envio_mas_caro = max_element(registros_envios.begin(), registros_envios.end());

    cout << "\n4. ENVIOS MINIMO Y MAXIMO POR PRECIO: " << endl;
    cout << "Envio mas barato: " << envio_mas_barato->producto.nombre
              << " ($" << fixed << setprecision(2) << envio_mas_barato->producto.precio << ")" << endl;

    cout << "Envio mas caro: " << envio_mas_caro->producto.nombre
              << " ($" << fixed << setprecision(2) << envio_mas_caro->producto.precio << ")" << endl;

    //El primero que entro es el primero en ser verificado
    cout << "\n5. PROCESAMIENTO DE COLAS: " << endl;
    
    if (!envios_en_espera.empty()) {
        cout << "   - Siguiente en verificar: ID " << envios_en_espera.front().id 
             << ", Cliente: " << envios_en_espera.front().cliente_nombre << endl;
    
    } else {
        cout << "   - Cola de espera vacía." << endl;
    }
    
    //El envio mas caro es el de maxima prioridad
    if (!envios_urgentes.empty()) {
        cout << "   - Maxima Prioridad (debido a su precio): ID " << envios_urgentes.top().id
             << ", Precio: $" << fixed << setprecision(2) << envios_urgentes.top().producto.precio << endl;
     
    } else {
        cout << "   - Cola de prioridad vacia." << endl;
    }

    // Simulamos un 'patrón' de precios esperados para los primeros x productos 
    cout << "\n6. COMPARACION DE RANGOS: " << endl;
    
    vector<double> precios_esperados = {100.00, 50.00, 250.00};
    vector<double> precios_reales;

    int count = 0;
    for (const auto& envio : registros_envios) {
        if (count < precios_esperados.size()) {
            precios_reales.push_back(envio.producto.precio);
            count++;
        }
    }

    // Uso de equal para saber si los precios esperados son iguales a los reales
    bool iguales = equal(precios_esperados.begin(), precios_esperados.end(), precios_reales.begin());
    cout << "Los primeros " << precios_esperados.size() << " precios coinciden con los precios esperados?: " 
         << (iguales ? "Si" : "No") << endl;
}

int main() {
    cout << left;

    int opcion;
    do {
        cout << "\n===========================================================" << endl;
        cout << "         Sistema de envios de LogiTrans S.A.   " << endl;
        cout << "===========================================================" << endl;
        cout << "1. Registrar nuevo envio" << endl;
        cout << "2. Mostrar todos los envios" << endl;
        cout << "3. Buscar envios registrados segun nombre del cliente" << endl;
        cout << "4. Analisis de envios" << endl;
        cout << "0. Salir" << endl;
        cout << "\nSeleccione una opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada no valida." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
            case 1:
                registrarEnvio();
                break;
            case 2:
                mostrarEnvios();
                break;
            case 3:
                buscarCliente();
                break;
            case 4:
                analisisEnvios();
                break;
            case 0:
                cout << "\nHasta luego! Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 0);

    return 0;
}