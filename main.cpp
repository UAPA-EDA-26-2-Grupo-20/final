#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

//===================== CLASE PACIENTE =====================
class Paciente {
public:
    string nombre;
    int edad;
    int urgencia;
    string fecha;

    Paciente() {}

    Paciente(string n, int e, int u, string f) {
        nombre = n;
        edad = e;
        urgencia = u;
        fecha = f;
    }
};

//===================== COLA DE PRIORIDAD =====================
struct Comparar {
    bool operator()(Paciente a, Paciente b) {
        return a.urgencia < b.urgencia;
    }
};

priority_queue<Paciente, vector<Paciente>, Comparar> colaAtencion;

//===================== LISTA ENLAZADA =====================
struct Nodo {
    Paciente dato;
    Nodo* siguiente;

    Nodo(Paciente p) {
        dato = p;
        siguiente = nullptr;
    }
};

Nodo* historial = nullptr;

// Vector para búsquedas y ordenamientos
vector<Paciente> atendidos;

// Pila de cancelados
stack<Paciente> cancelados;

//===================== FUNCIONES =====================

// Agregar al historial
void agregarHistorial(Paciente p) {
    Nodo* nuevo = new Nodo(p);

    if (historial == nullptr) {
        historial = nuevo;
    } else {
        Nodo* aux = historial;

        while (aux->siguiente != nullptr) {
            aux = aux->siguiente;
        }

        aux->siguiente = nuevo;
    }
}

// Registrar paciente
void registrarPaciente() {
    string nombre, fecha;
    int edad, urgencia;

    cin.ignore();

    cout << "\nNombre: ";
    getline(cin, nombre);

    cout << "Edad: ";
    cin >> edad;

    cout << "Nivel de urgencia\n";
    cout << "1. Baja\n";
    cout << "2. Media\n";
    cout << "3. Alta\n";
    cout << "4. Emergencia\n";
    cout << "Seleccione: ";
    cin >> urgencia;

    cin.ignore();

    cout << "Fecha (dd/mm/aaaa): ";
    getline(cin, fecha);

    Paciente p(nombre, edad, urgencia, fecha);

    colaAtencion.push(p);

    cout << "\nPaciente registrado correctamente.\n";
}

// Mostrar cola de atención
void mostrarCola() {
    if (colaAtencion.empty()) {
        cout << "\nNo hay pacientes en espera.\n";
        return;
    }

    priority_queue<Paciente,
                   vector<Paciente>,
                   Comparar> copia = colaAtencion;

    cout << "\n===== COLA DE ATENCION =====\n";

    while (!copia.empty()) {
        Paciente p = copia.top();

        cout << "Nombre: " << p.nombre
             << " | Edad: " << p.edad
             << " | Urgencia: " << p.urgencia
             << endl;

        copia.pop();
    }
}

// Atender paciente
void atenderPaciente() {
    if (colaAtencion.empty()) {
        cout << "\nNo hay pacientes para atender.\n";
        return;
    }

    Paciente p = colaAtencion.top();
    colaAtencion.pop();

    cout << "\nPaciente atendido:\n";
    cout << "Nombre: " << p.nombre << endl;
    cout << "Edad: " << p.edad << endl;
    cout << "Urgencia: " << p.urgencia << endl;

    agregarHistorial(p);
    atendidos.push_back(p);
}

// Cancelar cita
void cancelarCita() {
    if (colaAtencion.empty()) {
        cout << "\nNo hay pacientes en espera.\n";
        return;
    }

    string nombre;
    cin.ignore();

    cout << "\nNombre del paciente a cancelar: ";
    getline(cin, nombre);

    priority_queue<Paciente,
                   vector<Paciente>,
                   Comparar> nuevaCola;

    bool encontrado = false;

    while (!colaAtencion.empty()) {
        Paciente p = colaAtencion.top();
        colaAtencion.pop();

        if (p.nombre == nombre && !encontrado) {
            cancelados.push(p);
            encontrado = true;
        } else {
            nuevaCola.push(p);
        }
    }

    colaAtencion = nuevaCola;

    if (encontrado)
        cout << "Cita cancelada y guardada en la pila.\n";
    else
        cout << "Paciente no encontrado.\n";
}

// Reactivar cita
void reactivarCita() {
    if (cancelados.empty()) {
        cout << "\nNo hay citas canceladas.\n";
        return;
    }

    Paciente p = cancelados.top();
    cancelados.pop();

    colaAtencion.push(p);

    cout << "\nCita reactivada.\n";
    cout << "Paciente: " << p.nombre << endl;
}
// Mostrar citas canceladas
void mostrarCancelados() {
    if (cancelados.empty()) {
        cout << "\nNo hay citas canceladas.\n";
        return;
    }

    // Copia para no destruir la pila original
    stack<Paciente> copia = cancelados;

    cout << "\n===== CITAS CANCELADAS =====\n";
    cout << "(La de arriba es la proxima en reactivarse)\n\n";

    while (!copia.empty()) {
        Paciente p = copia.top();

        cout << "Nombre: " << p.nombre
             << " | Edad: " << p.edad
             << " | Urgencia: " << p.urgencia
             << " | Fecha: " << p.fecha
             << endl;

        copia.pop();
    }
}


// Buscar paciente
void buscarPaciente() {
    if (colaAtencion.empty()) {
        cout << "\nNo hay pacientes registrados.\n";
        return;
    }

    string nombre;
    cin.ignore();

    cout << "\nNombre a buscar: ";
    getline(cin, nombre);

    priority_queue<Paciente,
                   vector<Paciente>,
                   Comparar> copia = colaAtencion;

    bool encontrado = false;

    while (!copia.empty()) {
        Paciente p = copia.top();

        if (p.nombre == nombre) {
            encontrado = true;

            cout << "\nPaciente encontrado:\n";
            cout << "Nombre: " << p.nombre << endl;
            cout << "Edad: " << p.edad << endl;
            cout << "Urgencia: " << p.urgencia << endl;
            break;
        }

        copia.pop();
    }

    if (!encontrado)
        cout << "\nPaciente no encontrado.\n";
}

// Mostrar historial
void mostrarHistorial() {
    if (historial == nullptr) {
        cout << "\nNo hay pacientes atendidos.\n";
        return;
    }

    Nodo* aux = historial;

    cout << "\n===== HISTORIAL =====\n";

    while (aux != nullptr) {
        cout << "Nombre: " << aux->dato.nombre
             << " | Edad: " << aux->dato.edad
             << " | Fecha: " << aux->dato.fecha
             << endl;

        aux = aux->siguiente;
    }
}

// Ordenar por edad
void ordenarEdad() {
    if (atendidos.empty()) {
        cout << "\nNo hay historial.\n";
        return;
    }

    sort(atendidos.begin(),
         atendidos.end(),
         [](Paciente a, Paciente b) {
             return a.edad < b.edad;
         });

    cout << "\n===== HISTORIAL ORDENADO POR EDAD =====\n";

    for (Paciente p : atendidos) {
        cout << p.nombre
             << " - Edad: "
             << p.edad
             << endl;
    }
}

// Ordenar por fecha
void ordenarFecha() {
    if (atendidos.empty()) {
        cout << "\nNo hay historial.\n";
        return;
    }

    sort(atendidos.begin(),
         atendidos.end(),
         [](Paciente a, Paciente b) {
             return a.fecha < b.fecha;
         });

    cout << "\n===== HISTORIAL ORDENADO POR FECHA =====\n";

    for (Paciente p : atendidos) {
        cout << p.nombre
             << " - Fecha: "
             << p.fecha
             << endl;
    }
}

// Recursividad
int contarMayores60(vector<Paciente>& v, int i) {
    if (i == v.size())
        return 0;

    if (v[i].edad > 60)
        return 1 + contarMayores60(v, i + 1);

    return contarMayores60(v, i + 1);
}

//===================== MAIN =====================
int main() {
    int opcion;

    do {
        cout << "\n===================================";
        cout << "\n   CLINICA DE DON FABIO";
        cout << "\n===================================";
        cout << "\n1. Registrar paciente";
        cout << "\n2. Mostrar cola de atencion";
        cout << "\n3. Atender paciente";
        cout << "\n4. Cancelar cita";
        cout << "\n5. Reactivar cita";
        cout << "\n6. Buscar paciente";
        cout << "\n7. Mostrar historial";
        cout << "\n8. Ordenar historial por edad";
        cout << "\n9. Ordenar historial por fecha";
        cout << "\n10. Contar mayores de 60 anos";
        cout << "\n11. Mostrar citas canceladas";
        cout << "\n0. Salir";
        cout << "\nSeleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            registrarPaciente();
            break;

        case 2:
            mostrarCola();
            break;

        case 3:
            atenderPaciente();
            break;

        case 4:
            cancelarCita();
            break;

        case 5:
            reactivarCita();
            break;

        case 6:
            buscarPaciente();
            break;

        case 7:
            mostrarHistorial();
            break;

        case 8:
            ordenarEdad();
            break;

        case 9:
            ordenarFecha();
            break;

        case 10:
            cout << "\nPacientes mayores de 60 anos: "
                 << contarMayores60(atendidos, 0)
                 << endl;
            break;
            
         case 11:
            mostrarCancelados();
            break;
            
        case 0:
            cout << "\nGracias por utilizar el sistema de Don Fabio.\n";
            break;

        default:
            cout << "\nOpcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;
}
