#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <cctype>
#include <sstream>

using namespace std;

bool esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int precedencia(char operador) {
    if (operador == '^')
        return 3;
    if (operador == '*' || operador == '/')
        return 2;
    if (operador == '+' || operador == '-')
        return 1;
    return 0;
}

string infijoAPostfijo(string expresion) {
    stack<char> pila;
    string postfijo;
    bool ultimoFueNumero = false;

    for (int i = 0; i < expresion.length(); i++) {
        char c = expresion[i];

        if (isspace(c)) {
            continue;
        }

        if (isdigit(c) || c == '.') {
            if (ultimoFueNumero) {
                postfijo += c;
            } else {
                postfijo += ' ';
                postfijo += c;
                ultimoFueNumero = true;
            }
        }
        else if (c == '(') {
            pila.push(c);
            ultimoFueNumero = false;
        }
        else if (c == ')') {
            while (!pila.empty() && pila.top() != '(') {
                postfijo += ' ';
                postfijo += pila.top();
                pila.pop();
            }
            pila.pop();
            ultimoFueNumero = false;
        }
        else if (esOperador(c)) {
            postfijo += ' ';
            while (!pila.empty() && precedencia(pila.top()) >= precedencia(c)) {
                postfijo += pila.top();
                pila.pop();
            }
            pila.push(c);
            ultimoFueNumero = false;
        }
    }

    while (!pila.empty()) {
        postfijo += ' ';
        postfijo += pila.top();
        pila.pop();
    }
    return postfijo;
}

double evaluarPostfijo(string expresion) {
    stack<double> pila;
    stringstream ss(expresion);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1) || token[0] == '.') {
            pila.push(stod(token));
        } else {
            double val2 = pila.top(); pila.pop();
            double val1 = pila.top(); pila.pop();
            switch (token[0]) {
                case '+': pila.push(val1 + val2); break;
                case '-': pila.push(val1 - val2); break;
                case '*': pila.push(val1 * val2); break;
                case '/': pila.push(val1 / val2); break;
                case '^': pila.push(pow(val1, val2)); break;
            }
        }
    }
    return pila.top();
}

bool estanParentesisBalanceados(string expresion) {
    stack<char> pila;
    for (char c : expresion) {
        if (c == '(') {
            pila.push(c);
        } else if (c == ')') {
            if (pila.empty()) return false;
            pila.pop();
        }
    }
    return pila.empty();
}

bool operadoresValidos(string expresion) {
    for (int i = 0; i < expresion.length(); i++) {
        if (esOperador(expresion[i])) {
            if (i == 0 || i == expresion.length() - 1) return false;
            if (esOperador(expresion[i + 1])) return false;
        }
    }
    return true;
}

int main() {
    string expresion;

    cout << "Ingrese una operacion compuesta: ";
    getline(cin, expresion);

    if (!estanParentesisBalanceados(expresion)) {
        cout << "Error: Los paréntesis no están balanceados." << endl;
        return 1;
    }

    if (!operadoresValidos(expresion)) {
        cout << "Error: Operadores mal posicionados." << endl;
        return 1;
    }

    string postfijo = infijoAPostfijo(expresion);
    cout << "Expresion en notacion postfija: " << postfijo << endl;

    double resultado = evaluarPostfijo(postfijo);
    cout << "Resultado: " << resultado << endl;

    return 0;
}
