#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <tuple>
using namespace std;

class Fecha {
public:
    int dia, mes, año;
    Fecha(int d, int m, int a) : dia(d), mes(m), año(a) {}

    void mostrar() const {
        cout << dia << "/" << mes << "/" << año;
    }

    // Función para calcular los días restantes hasta esta fecha desde una fecha actual dada
    int diasHasta(const Fecha& actual) const {
        tm tCumple = {};
        tCumple.tm_year = actual.año - 1900;
        tCumple.tm_mon = mes - 1;
        tCumple.tm_mday = dia;

        time_t tiempoCumple = mktime(&tCumple);

        tm tActual = {};
        tActual.tm_year = actual.año - 1900;
        tActual.tm_mon = actual.mes - 1;
        tActual.tm_mday = actual.dia;

        time_t tiempoActual = mktime(&tActual);

        double diferenciaSegundos = difftime(tiempoCumple, tiempoActual);
        int diasRestantes = diferenciaSegundos / (60 * 60 * 24);

        if (diasRestantes < 0) {
            tCumple.tm_year++;
            tiempoCumple = mktime(&tCumple);
            diferenciaSegundos = difftime(tiempoCumple, tiempoActual);
            diasRestantes = diferenciaSegundos / (60 * 60 * 24);
        }

        return diasRestantes;
    }
};

class Persona {
public:
    string nombre;
    Fecha fechaCumpleaños;
    string etiqueta;

    Persona(string n, Fecha f, string e) : nombre(n), fechaCumpleaños(f), etiqueta(e) {}

    void mostrar() const {
        cout << "Nombre: " << this->nombre << ", Cumpleaños: ";
        this->fechaCumpleaños.mostrar();
        cout << ", Etiqueta: " << this->etiqueta << endl;
    }

    bool operator<(const Persona& otra) const {
        return tie(fechaCumpleaños.año, fechaCumpleaños.mes, fechaCumpleaños.dia) <
            tie(otra.fechaCumpleaños.año, otra.fechaCumpleaños.mes, otra.fechaCumpleaños.dia);
    }
};

class SistemaCumpleaños {
private:
    vector<Persona> personas;

public:
    void agregarCumpleaños(const string& nombre, int dia, int mes, int año, const string& etiqueta) {
        personas.emplace_back(nombre, Fecha(dia, mes, año), etiqueta);
    }

    void mostrarCumpleaños() const {
        for (const auto& persona : personas) {
            persona.mostrar();
        }
    }

    void buscarCumpleaños(const string& nombre) const {
        auto it = find_if(personas.begin(), personas.end(),
            [&nombre](const Persona& p) { return p.nombre == nombre; });

        if (it != personas.end()) {
            it->mostrar();
        }
        else {
            cout << "No se encontró a la persona con nombre: " << nombre << endl;
        }
    }

    void eliminarCumpleaños(const string& nombre) {
        auto it = remove_if(personas.begin(), personas.end(),
            [&nombre](const Persona& p) { return p.nombre == nombre; });

        if (it != personas.end()) {
            personas.erase(it, personas.end());
            cout << "Cumpleaños eliminado." << endl;
        }
        else {
            cout << "No se encontró a la persona con nombre: " << nombre << endl;
        }
    }

    void mostrarCumpleañosCercanos(const Fecha& fechaActual) const {
        vector<pair<int, Persona>> proximosCumpleaños;

        for (const auto& persona : personas) {
            int dias = persona.fechaCumpleaños.diasHasta(fechaActual);
            proximosCumpleaños.emplace_back(dias, persona);
        }

        sort(proximosCumpleaños.begin(), proximosCumpleaños.end(),
            [](const pair<int, Persona>& a, const pair<int, Persona>& b) {
                return a.first < b.first;
            });

        cout << "Los 3 cumpleaños más cercanos son:" << endl;
        for (int i = 0; i < 3 && i < proximosCumpleaños.size(); ++i) {
            proximosCumpleaños[i].second.mostrar();
        }
    }

    void mostrarCumpleañosPorEtiqueta(const string& etiqueta) const {
        cout << "Cumpleaños con etiqueta " << etiqueta << ":" << endl;
        for (const auto& persona : personas) {
            if (persona.etiqueta == etiqueta) {
                persona.mostrar();
            }
        }
    }
};

int main() {
    SistemaCumpleaños sistema;

    // Datos hipotéticos precargados
    sistema.agregarCumpleaños("Alice", 5, 6, 1995, "familia");
    sistema.agregarCumpleaños("Bob", 20, 7, 1990, "amigos");
    sistema.agregarCumpleaños("Charlie", 12, 6, 1985, "conocidos");
    sistema.agregarCumpleaños("David", 25, 12, 1992, "familia");
    sistema.agregarCumpleaños("Eve", 30, 11, 1988, "amigos");
    sistema.agregarCumpleaños("Frank", 1, 1, 1991, "conocidos");

    int opcion;
    Fecha fechaActual(1, 6, 2024);  // Fecha actual fija para pruebas

    do {
        cout << "1. Mostrar todos los cumpleaños\n";
        cout << "2. Buscar cumpleaños\n";
        cout << "3. Eliminar cumpleaños\n";
        cout << "4. Mostrar los 3 cumpleaños más cercanos\n";
        cout << "5. Filtrar cumpleaños por etiqueta\n";
        cout << "6. Salir\n";
        cout << "Elige una opción: ";
        cin >> opcion;

        string nombre, etiqueta;

        switch (opcion) {
        case 1:
            sistema.mostrarCumpleaños();
            break;
        case 2:
            cout << "Nombre: ";
            cin.ignore();
            getline(cin, nombre);
            sistema.buscarCumpleaños(nombre);
            break;
        case 3:
            cout << "Nombre: ";
            cin.ignore();
            getline(cin, nombre);
            sistema.eliminarCumpleaños(nombre);
            break;
        case 4:
            sistema.mostrarCumpleañosCercanos(fechaActual);
            break;
        case 5:
            cout << "Etiqueta (familia, amigos, conocidos): ";
            cin.ignore();
            getline(cin, etiqueta);
            sistema.mostrarCumpleañosPorEtiqueta(etiqueta);
            break;
        case 6:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opción no válida." << endl;
        }
    } while (opcion != 6);

    return 0;
}
