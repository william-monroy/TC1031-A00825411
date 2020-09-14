//
//  main.cpp
//  Act1-3
//
//  Created by Maria Teresa Angulo Tello on 10/09/20.
//  Copyright © 2020 Maria Teresa Angulo Tello. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Datos {
    string mes;
    string dia;
    string hrs;
    string min;
    string seg;
    string ip;
    string error;
};

int buscarMenor(vector<string> datos, string clave){
    if (clave < datos[0])
        return 0;
    if (clave > datos[datos.size() - 1])
        return -2;
    int low, mid, sup;
    low = 0;
    sup = datos.size();
    while (low <= sup){
        mid = (low + sup) / 2;
        if (clave == datos[mid] || (clave < datos[mid] && clave > datos[mid-1]))
            return mid;
        else if(clave > datos[mid] && clave < datos[mid + 1])
            return mid + 1;
        else if (clave < datos[mid])
            sup = mid - 1;
        else
            low = mid + 1;
    }
    return -1;
}

int buscarMayor(vector<string> datos, string clave){
    if (clave < datos[0])
        return -2;
    if (clave > datos[datos.size() - 1])
        return datos.size()-1;
    int low, mid, sup;
    low = 0;
    sup = datos.size();
    while (low <= sup){
        mid = (low + sup) / 2;
        if (clave == datos[mid] || (clave > datos[mid] && clave < datos[mid + 1]))
            return mid;
        else if (clave < datos[mid] && clave > datos[mid - 1])
            return mid - 1;
        else if (clave < datos[mid])
            sup = mid - 1;
        else
            low = mid + 1;
    }
    return -1;
}

string createString(Datos data){
    if (data.dia.length() == 1)
        data.dia = "0" + data.dia;
    if (data.hrs.length() == 1)
        data.hrs = "0" + data.hrs;
    string sData = data.mes + data.dia + data.hrs + data.min + data.seg;
    return sData;
}

void printBitacora(vector<Datos> info, int i){
    cout << info[i].mes << " ";
    cout << info[i].dia << " ";
    cout << info[i].hrs << ":";
    cout << info[i].min << ":";
    cout << info[i].seg << " ";
    cout << info[i].ip << " ";
    cout << info[i].error << endl;
}

void monthToNum(string &mes){
    if (mes == "Jan")
        mes = "01";
    else if (mes == "Feb")
        mes = "02";
    else if (mes == "Mar")
        mes = "03";
    else if (mes == "Apr")
        mes = "04";
    else if (mes == "May")
        mes = "05";
    else if (mes == "Jun")
        mes = "06";
    else if (mes == "Jul")
        mes = "07";
    else if (mes == "Aug")
        mes = "08";
    else if (mes == "Sep")
        mes = "09";
    else if (mes == "Oct")
        mes = "10";
    else if (mes == "Nov")
        mes = "11";
    else if (mes == "Dic")
        mes = "12";
}

void readInput(Datos &data, string fecha){
    stringstream s(fecha);
    getline(s, data.mes, ' ');
    getline(s, data.dia, ' ');
    getline(s, data.hrs, ':' );
    getline(s, data.min, ':' );
    getline(s, data.seg, ' ' );
    monthToNum(data.mes);
}

void leerArchivo(vector<Datos>& bitacora, vector<string>& datos){
    Datos info;
    string data;
    ifstream archivo("bitacora.txt");
    if (archivo.fail()){
        cout << "El archivo no se ha leido" << endl;
        exit(1);
    }
    while (!archivo.eof()){
        getline(archivo, info.mes, ' ');
        getline(archivo, info.dia, ' ');
        getline(archivo, info.hrs, ':' );
        getline(archivo, info.min, ':' );
        getline(archivo, info.seg, ' ' );
        getline(archivo, info.ip, ' ' );
        getline(archivo, info.error);
        // Guardar los structs de los datos en un vector
        bitacora.push_back(info);
        // Crear claves para facilitar el ordenamiento de los datos
        monthToNum(info.mes);
        data = createString(info);
        datos.push_back(data);
    }
    archivo.close();
}

void outputArchivo(vector<Datos> info){
    ofstream salida("bitacoraSalida.txt");
    if (salida.fail()){
        cout << "El archivo no se ha creado correctamente" << endl;
        exit(1);
    }
    for (int i = 0; i < info.size(); i++){
        salida << info[i].mes << " ";
        salida << info[i].dia << " ";
        salida << info[i].hrs << ":";
        salida << info[i].min << ":";
        salida << info[i].seg << " ";
        salida << info[i].ip << " ";
        salida << info[i].error << endl;
    }
    salida.close();
}

void merge(vector<Datos> &bitacora, vector<string> &clave, int inicio, int fin){
    int centro = (inicio + fin) / 2;
    int j = inicio;
    int k = centro + 1;
    int tam = fin - inicio + 1;
    string claveTemp[tam];
    Datos bitacoraTemp[tam];
    for (int i = 0; i < tam; i++){
        if (j <= centro && k <= fin){
            if (clave[j] < clave[k]){
                bitacoraTemp[i] = bitacora[j];
                claveTemp[i] = clave[j++];
            }
            else {
                bitacoraTemp[i] = bitacora[k];
                claveTemp[i] = clave[k++];
            }
        }
        else if (j <= centro){
            bitacoraTemp[i] = bitacora[j];
            claveTemp[i] = clave[j++];
        }
        else {
            bitacoraTemp[i] = bitacora[k];
            claveTemp[i] = clave[k++];
        }
    }
    for (int i = 0; i < tam; i++){
        bitacora[inicio + i] = bitacoraTemp[i];
        clave[inicio + i] = claveTemp[i];
    }
}

void mergeSort(vector<Datos> &bitacora, vector<string> &clave, int inicio, int fin){
    if (inicio < fin){
        int mid = (inicio + fin) / 2;
        mergeSort(bitacora, clave, inicio, mid);
        mergeSort(bitacora, clave, mid + 1, fin);
        merge(bitacora, clave, inicio, fin);
    }
}

int main(){
    Datos bajo, alto;
    vector<Datos> bitacora;
    vector<string> clave;
    string rangoBajo, rangoAlto;
    int indiceBajo, indiceAlto, tam;
    char op = 'C';
    
    // Importar los datos del archivo, ordenarlos y exportarlos
    leerArchivo(bitacora, clave);
    tam = bitacora.size() - 1;
    mergeSort(bitacora, clave, 0, tam);
    outputArchivo(bitacora);
    
    // Pedirle al usuario un rango de busqueda
    cout << "Introduzca el rango para su busqueda" << endl;
    do {
        cout << "Ejemplo de formato: " << endl;
        cout << "Aug 6 23:17:56" << endl;
        cout << "Fecha de inicio: " << endl;
        getline(cin, rangoBajo);
        readInput(bajo, rangoBajo);
        rangoBajo = createString(bajo);
        cout << "Fecha de fin: " << endl;
        getline(cin, rangoAlto);
        readInput(alto, rangoAlto);
        rangoAlto = createString(alto);
        if (rangoBajo > rangoAlto){
            cout << "\nEl rango es invalido. Introduzca la fecha mas antigua primero" << endl;
        }
    } while (rangoBajo > rangoAlto);
    
    // Buscar los índices de las claves que delimitan el rango
    indiceBajo = buscarMenor(clave, rangoBajo);
    indiceAlto = buscarMayor(clave, rangoAlto);
    
    // Desplegar los resultados de la busqueda
    if (indiceBajo == -2 || indiceAlto == -2) {
        cout << "\nNo se han encontrado datos" << endl;
    }
    else {
        cout << "\nSe han encontrado " << (indiceAlto - indiceBajo + 1) << " datos\n" << endl;
        while ((op == 'C' || op == 'c') && indiceBajo <= indiceAlto){
            for (int i = 0; i < 20 && indiceBajo <= indiceAlto; i++){
                printBitacora(bitacora, indiceBajo++);
            }
            if (indiceBajo <= indiceAlto){
                cout << "\nPresione C para continuar visualizando los datos" << endl;
                cout << "Presione X para salir" << endl;
                cin >> op;
            }
        }
    }
    return 0;
}
