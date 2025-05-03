#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// Función para convertir una cadena a minúsculas
string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Función para cargar la base de conocimiento desde un archivo
map<string, string> cargarBaseDeConocimiento(const string& nombreArchivo) {
    map<string, string> base;
    ifstream archivo(nombreArchivo);
    string linea;

    while (getline(archivo, linea)) {
        size_t separador = linea.find("|");
        if (separador != string::npos) {
            string pregunta = linea.substr(0, separador);
            string respuesta = linea.substr(separador + 1);
            base[toLower(pregunta)] = respuesta;
        }
    }
    return base;
}

// Función para dividir una cadena en palabras
vector<string> dividirPalabras(const string& frase) {
    vector<string> palabras;
    stringstream ss(frase);
    string palabra;
    while (ss >> palabra) {
        palabras.push_back(palabra);
    }
    return palabras;
}

// Búsqueda por coincidencia exacta
bool busquedaExacta(const string& pregunta, const map<string, string>& base, string& respuesta) {
    auto it = base.find(toLower(pregunta));
    if (it != base.end()) {
        respuesta = it->second;
        return true;
    }
    return false;
}

// Búsqueda por palabras clave
bool busquedaPorPalabrasClave(const string& pregunta, const map<string, string>& base, string& respuesta) {
    vector<string> palabrasEntrada = dividirPalabras(toLower(pregunta));
    int maxCoincidencias = 0;

    for (const auto& par : base) {
        vector<string> palabrasBase = dividirPalabras(par.first);
        int coincidencias = 0;

        for (const auto& palabra : palabrasEntrada) {
            if (find(palabrasBase.begin(), palabrasBase.end(), palabra) != palabrasBase.end()) {
                coincidencias++;
            }
        }

        if (coincidencias > maxCoincidencias) {
            maxCoincidencias = coincidencias;
            respuesta = par.second;
        }
    }

    return maxCoincidencias > 0;
}

int main() {
    map<string, string> base = cargarBaseDeConocimiento("conocimiento.txt");
    string pregunta, respuesta;

    cout << "Chatbot: ¡Hola! Escribe 'salir' para terminar.\n";

    while (true) {
        cout << "\nTú: ";
        getline(cin, pregunta);

        if (toLower(pregunta) == "salir") break;

        if (busquedaExacta(pregunta, base, respuesta)) {
            cout << "Chatbot: " << respuesta << endl;
        }
        else if (busquedaPorPalabrasClave(pregunta, base, respuesta)) {
            cout << "Chatbot: " << respuesta << endl;
        }
        else {
            cout << "Chatbot: Lo siento, no tengo una respuesta para eso." << endl;
        }
    }

    cout << "Chatbot: ¡Hasta luego!" << endl;
    return 0;
}
