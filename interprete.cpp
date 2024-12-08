/* Nota: el archivo text.txt puede ser modificado para que arroje varios resultados usando varios print().
* No colocar la palabra reservada "root()" sola en una de las líneas de text.txt. Esta se ejecuta solo para almacenarse en una variable de lo contrario saltará un error
* Las variables pueden redefinirse como se ve en el ejemplo de text.txt donde X adquiere nuevo valor al volverse a definir.
* Al estar basado en Validaciones.cpp (Proyecto #1) las identificadores son del tipo Identifier = Value como en C.
* El interprete está hecho para operaciones matemáticas como se pide en el proyecto #2 así que no se pueden declarar strings o Characters en las variables. 
* No dejar saltos de línea entre las operaciones.

OPERACIONES DEL PROYECTO: 
	x = 9 / 2
	y = 4
	z = 3.1 + (x + y) - 2 * x
	w = z ^ 2
	t = root( w + 1.4)
	print( t )
	print( 3 + 1 )
	print( w + z )
	print(y/2+4)
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>
#include <cctype>
#include <locale.h>

using namespace std;

// Estructuras de datos para almacenar variables y tokens.
map<string, double> variables;

// Función para dividir la cadena en tokens bien definidos
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string token;
    for (char c : input) {
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '^' || c == '(' || c == ')') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));  // Añadir operador como token ñ
        } else {
            token += c;  
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

double evaluarExpresion(const vector<string>& tokens, size_t& index);

// Función para evaluar números o variables basado en tokens
double valor(const string& token) {
    if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
        return stod(token);
    } else if (variables.find(token) != variables.end()) {
        return variables[token];
    } else {
        throw runtime_error("Variable no definida: " + token);
    }
}

double potencia(double base, double exp) {
    return pow(base, exp);
}

// Función para calcular la raíz cuadrada (root) y manejo de errores asociado a ella.
double root(double value) {
    if (value < 0) {
        throw runtime_error("Error: No se puede calcular la raíz cuadrada de un número negativo.");
    }
    return sqrt(value);
}

// Analizadores
double evaluarFactor(const vector<string>& tokens, size_t& index) {
    string token = tokens[index];
    index++;
    if (token == "(") {
        double result = evaluarExpresion(tokens, index);
        if (tokens[index] != ")") {
            throw runtime_error("Se esperaba ')'");
        }
        index++;  
        return result;
    } else if (token == "root") {
        if (tokens[index] != "(") {
            throw runtime_error("Se esperaba '(' después de 'root'");
        }
        index++;  
        double value = evaluarExpresion(tokens, index);
        if (tokens[index] != ")") {
            throw runtime_error("Se esperaba ')'");
        }
        index++; 
        return root(value);
    }
    return valor(token);
}

// Evaluar expresiones con operadores de multiplicación, división y potencia (agregado para el proyecto 2)
double evaluarTerm(const vector<string>& tokens, size_t& index) {
    double result = evaluarFactor(tokens, index);
    while (index < tokens.size() && (tokens[index] == "*" || tokens[index] == "/" || tokens[index] == "^")) {
        string op = tokens[index];
        index++;
        double opDer = evaluarFactor(tokens, index);
        if (op == "*") result *= opDer;
        else if (op == "/") result /= opDer;
        else if (op == "^") result = potencia(result, opDer);
    }
    return result;
}

// Evaluar expresiones con operadores de suma y resta de Validaciones.cpp
double evaluarExpresion(const vector<string>& tokens, size_t& index) {
    double result = evaluarTerm(tokens, index);
    while (index < tokens.size() && (tokens[index] == "+" || tokens[index] == "-")) {
        string op = tokens[index];
        index++;
        double opDer = evaluarTerm(tokens, index);
        if (op == "+") result += opDer;
        else if (op == "-") result -= opDer;
    }
    return result;
}

// Función para interpretar una asignación de Validaciones.cpp
void asignacion(const vector<string>& tokens) {
    if (tokens.size() < 3 || tokens[1] != "=") {
        throw runtime_error("Error en la asignación");
    }
    string var = tokens[0];
    size_t index = 2;
    double result = evaluarExpresion(tokens, index);
    variables[var] = result;
}

void interprete(const string& input) {
    vector<string> tokens = tokenize(input);
    
    if (tokens[0] == "print") {
        tokens.erase(tokens.begin());  // Elimina el "print" para que se vea solo el resultado.
        size_t index = 0;
        double result = evaluarExpresion(tokens, index);
        cout << result << endl;
    } else {
        asignacion(tokens);
    }
}

void procesarTxt(const string& archivo) {
    ifstream inputFile(archivo);
    
    if (!inputFile.is_open()) {
        cout << "Error: No se pudo abrir el archivo intentalo con ""text.txt"" " << archivo << endl;
        return;
    }
    
    string line;
    while (getline(inputFile, line)) {
        try {
            interprete(line);
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    inputFile.close();
}

int main() {
	setlocale(LC_ALL, "spanish");
	
    string archivo = "text.txt"; 
    procesarTxt(archivo);
    
    return 0;
}

