#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <regex>
#include <cctype>

using namespace std;

int read_file(ifstream &, map <string,int> &);
map<string,int> del_res_words(map <string,int> &);
int hits_count(map <string,int> & ,map <string,int> &);
void to_lower(map <string,int> &);

int main(int argc, char* argv[]){
   ifstream files[argc-1];
   map <string,int> listas_palabras[argc-1];
   map <string,int> list_pal_sim_pal[argc-1]; // la lista que tiene las palabras para comparar la similaridad de palabras

//Similaridad parcial de palabras
//Similaridad de líneas
//Similaridad en comentarios
//Similaridad de secuencia semántica
   // Verify existance of files
    for(int i = 0; i < argc -1 ; i++){
      string file(argv[i+1]);
      files[i].open(argv[i+1]);
      if(!files[i].good()){
        cout << "Files doesn't exist or doesn't have reading permissions: "<< argv[i+1] << endl;
        exit(-1);
      }
      cout << file << endl;
    }
    // Reading files
    for(int i = 0; i < argc -1 ; i++){
        read_file(files[i],listas_palabras[i]);
    }
//Similaridad de palabras
//   En todos los otros casos, las comparaciones deben hacerse en minúsculas
//   Establezca como índice la cantidad de palabras del primer archivo que se repiten en el segundo, más palabras repetidas indica que los archivos son más similares

    for(int i = 0; i < argc -1 ; i++){
        list_pal_sim_pal[i] = del_res_words(listas_palabras[i]); //eliminar palabras reservadas
        to_lower(list_pal_sim_pal[i]);
    }

    for(int i = 0; i < argc -1 ; i += 2){
        int hits_number = hits_count(list_pal_sim_pal[i],list_pal_sim_pal[i+1]);//compara el file y el siguiente y cuenta los hits
        cout << "Similaridad de palabras: " << hits_number << endl;
        cout << "Porcentaje de similaridad de palabras: " << (double(hits_number)/double(list_pal_sim_pal[i].size()))*100 << endl;
    }

    return 0;





}

int read_file(ifstream & file, map <string,int> &lista_palabra){
    string line;
    char * pch;
    //string w_symbol;
    std::regex simbolos("^\\W+");
    std::regex simbolos_end("\\W+$");

    basic_string<char> pch_bs;

    while(!file.eof()){
        std::getline(file,line); // Read line
        pch = strtok(strdup(line.c_str())," "); //divide the line in tokens
        while(pch != NULL){ //while there is any word
            string w_symbol(pch);
            pch_bs = regex_replace(w_symbol,simbolos,"").data();
            pch_bs = regex_replace(pch_bs,simbolos_end,"").data();

            lista_palabra[pch_bs] = 1;
            pch = strtok(NULL," "); //get next token
        }
    }
    return 1;
}

map<string,int> del_res_words(map <string,int> & lista_palabras){ // Delete reserved words
    map <string,int> list_pal_sim_pal = lista_palabras;
    string lista_res[] = {"include","int" , "short" , "char" , "unsigned" , "signed" , "volatile" , "const" , "enum" ,
    "static" , "typedef" , "sizeof" , "if" , "else" , "switch" , "case" , "default" , "break" , "for" , "while" ,
    "do" , "continue" , "goto" , "struct" , "return" , "union" , "register" , "extern" , "void" , "auto" ,
    "stream" , "cin" , "cout" , "endl" , "flush" , "string", "argc","argv"};
    for(unsigned i = 0; i < (sizeof(lista_res)/sizeof(*lista_res)) ; i++){
        std::map<string,int>::iterator it = list_pal_sim_pal.begin();
        while (it != list_pal_sim_pal.end()) {
            if (lista_res[i].compare(it->first) == 0) {
               it = list_pal_sim_pal.erase(it);
            } else {
               ++it;
            }
        }
//        for(map<string,int>::iterator it = list_pal_sim_pal.begin(); it != list_pal_sim_pal.end(); ++it) {
//            if(lista_res[i].compare(it->first) == 0){
//                it = list_pal_sim_pal.erase(it);
//                it = it;
//            }
//        }

    }
    return list_pal_sim_pal;
}

int hits_count(map <string,int> & lista_ref,map <string,int> & lista_tar){ //compara el file y el siguiente y cuenta los hits
    int hits_count = 0;
    for(map<string,int>::iterator it1 = lista_ref.begin(); it1 != lista_ref.end(); ++it1) {
        for(map<string,int>::iterator it2 = lista_tar.begin(); it2 != lista_tar.end(); ++it2) {
            if((it1->first).compare(it2->first) == 0){
                hits_count++;
            }
        }
    }
    return hits_count;
}

void to_lower(map <string,int> & lista_palabras){
    string uno;
//    for(map<string,int>::iterator it = lista_palabras.begin(); it != lista_palabras.end(); ++it) {
//        uno = it->first;
//        transform(uno.begin(),uno.end(),uno.begin(), ::tolower);
//        lista_palabras.erase(it);
//        lista_palabras[uno] = 1;
//    }
    std::map<string,int>::iterator it = lista_palabras.begin();
    while (it != lista_palabras.end()) {
        uno = it->first;
        transform(uno.begin(),uno.end(),uno.begin(), ::tolower);
        if(lista_palabras.find(uno) == lista_palabras.end() ){
            it = lista_palabras.erase(it);
            lista_palabras[uno] = 1;
        } else {
            ++it;
        }


    }
    cout << "a" << endl;
}
