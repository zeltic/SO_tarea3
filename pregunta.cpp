#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <limits>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

void quebrar() // Punto de quiebre, lo uso para debuggear
{}

struct Node
{
    bool carpeta;
    int id;
    int size;
    string nombre;
    string ruta;
    vector<Node *>child;
};

Node *newNode(bool carpeta, int id, int size, string nombre, string ruta)
{
    Node *temp = new Node;
    temp->carpeta = carpeta;
    temp->id = id;
    temp->size = size;
    temp->nombre = nombre;
    temp->ruta = ruta;
    return temp;
}

typedef std::map<std::string, Node*> MyMap;
typedef std::pair<std::string, Node*> MyPair;
MyMap Carpeta;
MyMap Archivo;
int id_c = 0;

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void Split_String(vector<string>& Addr, string s)
{
    string delimiter = "/";
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        Addr.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    Addr.push_back(s);
    return;
}

void ADDR(vector<string> CurrentDir, vector<string>& newAddr, string& str_Addr)
{
    while(str_Addr.front() == '/')
        str_Addr.erase(str_Addr.begin());
    while(str_Addr.back() == '/')
        str_Addr.pop_back();
    newAddr.clear();
    vector<string> Addr;
    Split_String(Addr, str_Addr);
    if(Addr[0] == "~")
    {
        for(auto & it : Addr)
            newAddr.push_back(it);
    }
    else if(Addr[0] == "..")
    {
        for(auto & it : Addr)
        {
            if(it == "..")
                CurrentDir.pop_back();
            else
                CurrentDir.push_back(it);
        }
        for(auto & it : CurrentDir)
        {
            newAddr.push_back(it);
            if(it == "~")
                str_Addr = it;
            else
                str_Addr = str_Addr + "/" + it;
        }
    }
    else
    {
        for(auto & it : CurrentDir)
            newAddr.push_back(it);
        for(auto & it : Addr)
            newAddr.push_back(it);
        for(auto & it : newAddr)
            if(it == "~")
                str_Addr = it;
            else
                str_Addr = str_Addr + "/" + it;
    }
    return;
}

bool CHECK(vector<string> CurrentDir, string str_Addr, bool carpeta)
{
    vector<string> Addr;
    ADDR(CurrentDir, Addr, str_Addr);

    if(Carpeta.count(str_Addr) > 0 && carpeta)
        return true;
    else if(Archivo.count(str_Addr) > 0 && (!carpeta))
        return true;
    return false;
}

void CD(vector<string>& CurrentDir, vector<string> ArgV, int ArgC)
{
    if(ArgC < 1)
    {
        cout << "Se necesitan más argumentos." << endl;
        return;
    }
    else if(ArgC > 1)
    {
        cout << "No se aceptan espacios en la ruta." << endl;
        return;
    }
    string str_Addr = ArgV.back();
    vector<string> Addr;
    ADDR(CurrentDir, Addr, str_Addr);
    if(str_Addr == "" || str_Addr.back() == '/')
    {
        cout << "Ruta invalida." << endl;
        return;
    }

    if(Carpeta.count(str_Addr) > 0) // La carpeta existe si no es 0
    {
        CurrentDir.clear();
        for(auto & it : Addr)
            CurrentDir.push_back(it);
    }
    else
        cout << "El directorio no existe." << endl;

    return;
}

void MKDIR(vector<string> CurrentDir, vector<string> ArgV, int ArgC)
{
    if(ArgC < 1)
    {
        cout << "Se necesitan más argumentos." << endl;
        return;
    }
    else if(ArgC > 1)
    {
        cout << "No se aceptan espacios en la ruta." << endl;
        return;
    }
    string str_Addr = ArgV.back();
    vector<string> Addr;
    ADDR(CurrentDir, Addr, str_Addr);
    if(str_Addr == "" || str_Addr.back() == '/')
    {
        cout << "Ruta invalida." << endl;
        return;
    }

    if(Carpeta.count(str_Addr) == 0 && Archivo.count(str_Addr) == 0) // La carpeta no existe si es 0
    {
        string Current = "";
        for(auto & it : Addr)
        {
            if(Carpeta.count(Current + "/" + it) == 0 && it != "~") // Si las carpetas entre medio no existen entonces las creo
            {
                Node *nuevo = newNode(true, id_c, 4096, it, Current + "/" + it);
                (Carpeta[Current]->child).push_back(nuevo);
                Carpeta.insert(MyPair(Current + "/" + it, nuevo));
                id_c++;
            }
            if(it == "~")
                Current = it;
            else
                Current = Current + "/" + it;
        }
    }
    else if(Carpeta.count(str_Addr) > 0 && Archivo.count(str_Addr) == 0)
        cout << "Un directorio con ese nombre ya existe." << endl;
    else if(Archivo.count(str_Addr) > 0 && Carpeta.count(str_Addr) == 0)
        cout << "Un archivo con ese nombre ya existe." << endl;

    return;
}

void TOUCH(vector<string> CurrentDir, vector<string> ArgV, int ArgC)
{
    if(ArgC < 1)
    {
        cout << "Se necesitan más argumentos." << endl;
        return;
    }
    else if(ArgC > 1)
    {
        cout << "No se aceptan espacios en la ruta." << endl;
        return;
    }
    string str_Addr = ArgV.back();
    vector<string> Addr;
    ADDR(CurrentDir, Addr, str_Addr);
    if(str_Addr == "" || str_Addr.back() == '/')
    {
        cout << "Ruta invalida." << endl;
        return;
    }

    if(Archivo.count(str_Addr) == 0 && Carpeta.count(str_Addr) == 0) // El archivo no existe si es 0
    {
        string Current = "";
        string str_nombre = Addr.back();
        Addr.pop_back();
        for(auto & it : Addr)
        {
            if(Carpeta.count(Current + "/" + it) == 0 && it != "~") // Reviso si las carpetas entre medio existen
            {
                cout << "La ruta " << Current + "/" + it << " no existe." << endl;
                return;
            }
            if(it == "~")
                Current = it;
            else
                Current = Current + "/" + it;
        }
        srand (time(NULL));
        Node *nuevo = newNode(false, id_c, (rand() % 2147483647), str_nombre, str_Addr);
        (Carpeta[Current]->child).push_back(nuevo);
        Archivo.insert(MyPair(str_Addr, nuevo));
        id_c++;
    }
    else if(Archivo.count(str_Addr) > 0 && Carpeta.count(str_Addr) == 0)
        cout << "Un archivo con ese nombre ya existe." << endl;
    else if(Carpeta.count(str_Addr) > 0 && Archivo.count(str_Addr) == 0)
        cout << "Un directorio con ese nombre ya existe." << endl;

    return;
}

void LS(vector<string> CurrentDir, vector<string> Addr, vector<string> ArgV, int ArgC, string str_Addr)
{
    string str_CurrentDir = "";
    for(auto & it : Addr)
        str_CurrentDir = str_CurrentDir + it + "/";
    str_CurrentDir.pop_back();
    int flag = 0;
    int flag2 = 0;
    int i = 0;
    int r = 0;
    int s = 0;

    for(auto & str : ArgV)
    {
        if(str == "-R")
            r = 1;
        else if(str == "-i")
            i = 1;
        else if(str == "-s")
            s = 1;
        else if(str.front() == '-')
        {
            cout << "\"" << str << "\" no se reconoce como una opcion." << endl;
            return;
        }
    }

    for(auto & it : Carpeta[str_CurrentDir]->child)
    {
        //quebrar();
        if(r != 0 && flag2 == 0)
        {
            if(CurrentDir.back() == Addr.back())
                cout << endl << "./:" << endl;
            else
                cout << endl << "." << str_Addr << ":" << endl;
            flag = 1;
            flag2 = 1;
        }
        if(i != 0)
        {
            if(it->carpeta)
                cout << "\033[1;34m" << it->id << "\033[0m" << "\t";
            else
                cout << it->id << "\t";
        }
        if(s != 0)
        {
            if(it->carpeta)
                cout << "\033[1;34m" << it->size << "kB" << "\033[0m" << "\t";
            else
                cout << it->size << "kB" << "\t";
        }
        if(it->carpeta)
            cout << "\033[1;34m" << it->nombre <<"\033[0m" << endl;
        else
            cout << it->nombre << endl;
    }
    if(flag!=0)
    {
        for(auto & it : Carpeta[str_CurrentDir]->child)
        {
            if(it->carpeta)
            {
                Addr.clear();
                Split_String(Addr, it->ruta);
                //quebrar();
                LS(CurrentDir, Addr, ArgV, ArgC, str_Addr + "/" + Addr.back());
            }
        }
    }
    return;
}

void copiar(string str_Addr, string str_newAddr)
{
    for(auto & it : Carpeta[str_newAddr]->child)
    {
        if(it->carpeta)
        {
            for(auto & jt : Carpeta[str_Addr]->child)
            {
                if(it->nombre == jt->nombre)
                {
                    Node *nuevo = newNode(true, 0, 0, "", "");
                    *nuevo = *jt;
                    it = nuevo;
                    Carpeta.insert(MyPair(str_newAddr+"/"+it->nombre, it));
                    Carpeta[str_newAddr+"/"+it->nombre]->id = id_c;
                    Carpeta[str_newAddr+"/"+it->nombre]->ruta = str_newAddr+"/"+it->nombre;
                    id_c++;
                    copiar(str_Addr+"/"+jt->nombre, str_newAddr+"/"+it->nombre);
                    //quebrar();
                }
            }
        }
        else
        {
            for(auto & jt : Carpeta[str_Addr]->child)
            {
                if(it->nombre == jt->nombre)
                {
                    Node *nuevo = newNode(false, 0, 0, "", "");
                    *nuevo = *jt;
                    it = nuevo;
                    Archivo.insert(MyPair(str_newAddr+"/"+it->nombre, it));
                    Archivo[str_newAddr+"/"+it->nombre]->id = id_c;
                    Archivo[str_newAddr+"/"+it->nombre]->ruta = str_newAddr+"/"+it->nombre;
                    id_c++;
                    //quebrar();
                }
            }
        }
    }
    return;
}

void CP(vector<string> CurrentDir, vector<string> ArgV, int ArgC)
{
    string newPadre = "";
    bool carpeta = false;
    if(ArgC < 2)
    {
        cout << "No hay suficientes argumentos." << endl;
        return;
    }
    /*else if(ArgC > 2)
    {
        cout << "Demasiados argumentos." << endl;
        return;
    }*/
    string str_Addr = (*(ArgV.end() - 2));
    string str_newAddr = ArgV.back();
    vector<string> Addr;
    vector<string> newAddr;

    ADDR(CurrentDir, Addr, str_Addr);
    if(str_Addr == "" || str_Addr.back() == '/')
    {
        cout << "Ruta 1 es invalida." << endl;
        return;
    }

    ADDR(CurrentDir, newAddr, str_newAddr);
    if(str_newAddr == "" || str_newAddr.back() == '/')
    {
        cout << "Ruta 2 es invalida." << endl;
        return;
    }

    string str_name = Addr.back();

    if(Archivo.count(str_Addr) > 0) // El archivo existe si es 0
        carpeta = false;
    else if(Carpeta.count(str_Addr) > 0) // El archivo existe si es 0
        carpeta = true;
    else
    {
        cout << "No existe el archivo o carpeta." << endl;
        return;
    }

    if(Archivo.count(str_newAddr+"/"+str_name) == 0 && Carpeta.count(str_newAddr+"/"+str_name) == 0)
    {
        //quebrar();
        if(Carpeta.count(str_newAddr) > 0)
        {
            if(carpeta)
            {
                Node *nuevo = newNode(true, 0, 0, "", "");
                *nuevo = *Carpeta[str_Addr];
                (Carpeta[str_newAddr]->child).push_back(nuevo);
                Carpeta.insert(MyPair(str_newAddr+"/"+str_name, nuevo));
                Carpeta[str_newAddr+"/"+str_name]->id = id_c;
                Carpeta[str_newAddr+"/"+str_name]->ruta = str_newAddr+"/"+str_name;
                id_c++;
                copiar(str_Addr, str_newAddr+"/"+str_name);
            }
            else
            {
                Node *nuevo = newNode(false, 0, 0, "", "");
                *nuevo = *Archivo[str_Addr];
                (Carpeta[str_newAddr]->child).push_back(nuevo);
                Archivo.insert(MyPair(str_newAddr+"/"+str_name, nuevo));
                Archivo[str_newAddr+"/"+str_name]->id = id_c;
                Archivo[str_newAddr+"/"+str_name]->ruta = str_newAddr+"/"+str_name;
                id_c++;
            }
            return;
        }
        else
        {
            cout << "La ruta de destino no existe." << endl;
            return;
        }
    }
    else
    {
        cout << "Ya existe un archivo o carpeta con el nombre en la ruta de destino." << endl;
        return;
    }
    return;
}

void remover(vector<string>& CurrentDir, string str_Addr)
{
    for(auto it = Carpeta[str_Addr]->child.begin(); it != Carpeta[str_Addr]->child.end();)
    {
        if((*it)->carpeta)
        {
            //quebrar();
            remover(CurrentDir, (*it)->ruta);
            Carpeta.erase((*it)->ruta);
            it = Carpeta[str_Addr]->child.erase(it);
        }
        else
        {
            //quebrar();
            Archivo.erase((*it)->ruta);
            it = Carpeta[str_Addr]->child.erase(it);
        }
    }
    string str_CurrentDir = "";
    for(auto & it : CurrentDir)
        if(it == "~")
            str_CurrentDir = it;
        else
            str_CurrentDir = str_CurrentDir + "/" + it;
    str_CurrentDir.pop_back();
    if(str_CurrentDir == str_Addr)
    {
        CurrentDir.clear();
        CurrentDir.push_back("~");
    }
    return;
}

void RM(vector<string>& CurrentDir, vector<string> ArgV, int ArgC)
{
    string Padre = "";
    if(ArgC < 1)
    {
        cout << "No hay suficientes argumentos." << endl;
        return;
    }
    /*else if(ArgC > 2)
    {
        cout << "Demasiados argumentos." << endl;
        return;
    }*/
    string str_Addr = ArgV.back();
    vector<string> Addr;

    ADDR(CurrentDir, Addr, str_Addr);
    if(str_Addr == "" || str_Addr.back() == '/')
    {
        cout << "La ruta es invalida." << endl;
        return;
    }

    if(Archivo.count(str_Addr) > 0) // El archivo existe si es 0
    {
        vector<string> temp;
        for(auto & it : Addr)
            temp.push_back(it);
        temp.pop_back();
        for(auto & it : temp)
            Padre = Padre + it + "/";
        Padre.pop_back();
        for(auto it = Carpeta[Padre]->child.begin(); it != Carpeta[Padre]->child.end();)
        {
            if(Archivo.count(str_Addr) > 0)
            {
                if((*it)->nombre == Archivo[str_Addr]->nombre)
                {
                    Archivo.erase(str_Addr);
                    it = Carpeta[Padre]->child.erase(it);
                }
                else
                    ++it;
            }
            else break;
        }
    }
    else if(Carpeta.count(str_Addr) > 0) // El archivo existe si es 0
    {
        vector<string> temp;
        for(auto & it : Addr)
            temp.push_back(it);
        temp.pop_back();
        for(auto & it : temp)
            Padre = Padre + it + "/";
        Padre.pop_back();
        for(auto it = Carpeta[Padre]->child.begin(); it != Carpeta[Padre]->child.end();)
        {
            //quebrar();
            //cout << Carpeta.count(str_Addr) << endl;
            if(Carpeta.count(str_Addr) > 0)
            {
                if((*it)->nombre == Carpeta[str_Addr]->nombre)
                {
                    remover(CurrentDir, str_Addr);
                    Carpeta.erase(str_Addr);
                    it = Carpeta[Padre]->child.erase(it);
                }
                else
                    ++it;
            }
            else break;
        }
    }
    else
    {
        cout << "No existe el archivo o carpeta." << endl;
        return;
    }
    //quebrar();
    string str_CurrentDir = "";
    for(auto & it : CurrentDir)
        if(it == "~")
            str_CurrentDir = it;
        else
            str_CurrentDir = str_CurrentDir + "/" + it;
    str_CurrentDir.pop_back();
    if(str_CurrentDir == str_Addr)
    {
        CurrentDir.clear();
        CurrentDir.push_back("~");
    }
    return;
}

void mv_copiar(string str_Addr, string str_newAddr)
{
    for(auto & it : Carpeta[str_newAddr]->child)
    {
        if(it->carpeta)
        {
            for(auto & jt : Carpeta[str_Addr]->child)
            {
                if(it->nombre == jt->nombre)
                {
                    Node *nuevo = newNode(true, 0, 0, "", "");
                    *nuevo = *jt;
                    it = nuevo;
                    Carpeta.insert(MyPair(str_newAddr+"/"+it->nombre, it));
                    Carpeta[str_newAddr+"/"+it->nombre]->ruta = str_newAddr+"/"+it->nombre;
                    copiar(str_Addr+"/"+jt->nombre, str_newAddr+"/"+it->nombre);
                    //quebrar();
                }
            }
        }
        else
        {
            for(auto & jt : Carpeta[str_Addr]->child)
            {
                if(it->nombre == jt->nombre)
                {
                    Node *nuevo = newNode(false, 0, 0, "", "");
                    *nuevo = *jt;
                    it = nuevo;
                    Archivo.insert(MyPair(str_newAddr+"/"+it->nombre, it));
                    Archivo[str_newAddr+"/"+it->nombre]->ruta = str_newAddr+"/"+it->nombre;
                    //quebrar();
                }
            }
        }
    }
    return;
}

void MV(vector<string>& CurrentDir, vector<string> ArgV, int ArgC)
{
    string Padre = "";
    string newPadre = "";
    bool carpeta = false;
    if(ArgC < 2)
    {
        cout << "No hay suficientes argumentos." << endl;
        return;
    }
    /*else if(ArgC > 2)
    {
        cout << "Demasiados argumentos." << endl;
        return;
    }*/
    string str_Addr = (*(ArgV.end() - 2));
    string str_newAddr = ArgV.back();
    vector<string> Addr;
    vector<string> newAddr;

    ADDR(CurrentDir, Addr, str_Addr);
    if(str_Addr == "" || str_Addr.back() == '/')
    {
        cout << "Ruta 1 es invalida." << endl;
        return;
    }

    ADDR(CurrentDir, newAddr, str_newAddr);
    if(str_newAddr == "" || str_newAddr.back() == '/')
    {
        cout << "Ruta 2 es invalida." << endl;
        return;
    }
    
    string str_newname = newAddr.back();

    if(Archivo.count(str_Addr) > 0) // El archivo existe si es 0
    {
        vector<string> temp;
        for(auto & it : Addr)
            temp.push_back(it);
        temp.pop_back();
        for(auto & it : temp)
            Padre = Padre + it + "/";
        Padre.pop_back();
        carpeta = false;
    }
    else if(Carpeta.count(str_Addr) > 0) // El archivo existe si es 0
    {
        vector<string> temp;
        for(auto & it : Addr)
            temp.push_back(it);
        temp.pop_back();
        for(auto & it : temp)
            Padre = Padre + it + "/";
        Padre.pop_back();
        carpeta = true;
    }
    else
    {
        cout << "No existe el archivo o carpeta." << endl;
        return;
    }

    if(Archivo.count(str_newAddr) == 0 && Carpeta.count(str_newAddr) == 0)
    {
        vector<string> temp;
        for(auto & it : newAddr)
            temp.push_back(it);
        temp.pop_back();
        for(auto & it : temp)
            newPadre = newPadre + it + "/";
        newPadre.pop_back();
        if(Carpeta.count(newPadre) > 0)
        {
            if(carpeta)
            {
                Node *nuevo = newNode(true, 0, 0, "", "");
                *nuevo = *Carpeta[str_Addr];
                (Carpeta[newPadre]->child).push_back(nuevo);
                Carpeta.insert(MyPair(newPadre+"/"+str_newname, nuevo));
                Carpeta[newPadre+"/"+str_newname]->nombre = str_newname;
                Carpeta[newPadre+"/"+str_newname]->ruta = newPadre+"/"+str_newname;
                mv_copiar(str_Addr, newPadre+"/"+str_newname);
                for(auto it = Carpeta[Padre]->child.begin(); it != Carpeta[Padre]->child.end();)
                {
                    if(Carpeta.count(str_Addr) > 0)
                    {
                        if((*it)->nombre == Carpeta[str_Addr]->nombre)
                        {
                            remover(CurrentDir, str_Addr);
                            Carpeta.erase(str_Addr);
                            it = Carpeta[Padre]->child.erase(it);
                        }
                        else
                            ++it;
                    }
                    else break;
                }
            }
            else
            {
                Node *nuevo = newNode(false, 0, 0, "", "");
                *nuevo = *Archivo[str_Addr];
                (Carpeta[newPadre]->child).push_back(nuevo);
                Archivo.insert(MyPair(newPadre+"/"+str_newname, nuevo));
                Archivo[newPadre+"/"+str_newname]->nombre = str_newname;
                Archivo[newPadre+"/"+str_newname]->ruta = newPadre+"/"+str_newname;
                for(auto it = Carpeta[Padre]->child.begin(); it != Carpeta[Padre]->child.end();)
                {
                    if(Archivo.count(str_Addr) > 0)
                    {
                        if((*it)->nombre == Archivo[str_Addr]->nombre)
                        {
                            Archivo.erase(str_Addr);
                            it = Carpeta[Padre]->child.erase(it);
                        }
                        else
                            ++it;
                    }
                }
            }
            return;
        }
        else
        {
            cout << "La ruta de destino no existe." << endl;
            return;
        }
    }
    else
    {
        cout << "Ya existe un archivo o carpeta con el nombre en la ruta de destino." << endl;
        return;
    }
    string str_CurrentDir = "";
    for(auto & it : CurrentDir)
        if(it == "~")
            str_CurrentDir = it;
        else
            str_CurrentDir = str_CurrentDir + "/" + it;
    str_CurrentDir.pop_back();
    if(str_CurrentDir == str_Addr)
    {
        CurrentDir.clear();
        CurrentDir.push_back("~");
    }
    return;
}

int main()
{
    int flag = 0;
    string strFilename = "~";
    Node *root = newNode(true, id_c, 4096, strFilename, strFilename);
    Carpeta.insert(MyPair(strFilename, root));
    id_c++;
    vector<string> CurrentDir;
    CurrentDir.push_back(strFilename);
    while(flag == 0)
    {
        string linea;
        string comando;
        vector<string> ArgV;
        int ArgC = 0;
        for(auto & it : CurrentDir)
        {
            if(it == "~")
                cout << it;
            else
                cout << "/" << it;
        }
        cout << "$ ";
        while(true)
        {
            cin >> linea;
            ArgC++;
            if(ArgC > 1)
                ArgV.push_back(linea);
            else
                comando = linea;
            if(cin.peek() == '\n')
                break;
        }
        ArgC--;

        switch(str2int(comando.c_str()))
        {
            case str2int("cd"):
                CD(CurrentDir, ArgV, ArgC);
                break;
            case str2int("mkdir"):
                MKDIR(CurrentDir, ArgV, ArgC);
                break;
            case str2int("touch"):
                TOUCH(CurrentDir, ArgV, ArgC);
                break;
            case str2int("ls"):
                LS(CurrentDir, CurrentDir, ArgV, ArgC, "");
                break;
            case str2int("mv"):
                MV(CurrentDir, ArgV, ArgC);
                break;
            case str2int("cp"):
                CP(CurrentDir, ArgV, ArgC);
                break;
            case str2int("rm"):
                RM(CurrentDir, ArgV, ArgC);
                break;
            case str2int("exit"):
                flag = 1;
                break;
            default:
                cout << "\""<< comando << "\" no se reconoce como un comando." << endl;
                break;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return 0;
}