#include <iostream>
#include <winsock2.h>
#include <ctime>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <fstream>

using namespace std;

class Server{
public:
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[1024];

    Server(){
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5000);

       //Declaramos la fecha/hora del dia.
        time_t now = time(0);
        tm* time = localtime(&now);
        int dia = time-> tm_mday;
        int mes = time-> tm_mon + 1;
        int anio = 1900 + time-> tm_year;
        int hora = time-> tm_hour;
        int minu = time-> tm_min;


        string nombreArchivo = "server.log";
        ofstream texto;

        texto.open(nombreArchivo.c_str(),fstream::out);//abrimos el txt

         texto << endl;
         texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ": ==================" << endl;
         texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ":" << ": =======Inicia Servidor======= " << endl;
         texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ": ==================" << endl;

         texto.close();//cerramos

         ConexionDeSocket();

    }

    void ConexionDeSocket(){
        bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(server, 0);

        time_t now = time(0);
        tm* time = localtime(&now);
        int dia = time-> tm_mday;
        int mes = time-> tm_mon + 1;
        int anio = 1900 + time-> tm_year;
        int hora = time-> tm_hour;
        int minu = time-> tm_min;


        string nombreArchivo = "server.log";
        ofstream texto;

        texto.open(nombreArchivo.c_str(),fstream::out);
        texto << endl;
         texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ": Socket creado. Puerto de escucha:5000. " << endl;
         cout << "Escuchando para conexiones entrantes." << endl;
        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET){
            texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ":Conexión Aceptada." << endl;
            cout << "Cliente conectado!" << endl;
        }
        texto.close();

    }




    void Recibir(){
      recv(client, buffer, sizeof(buffer), 0);
      cout << "El cliente dice: " << buffer << endl;
      //string buf=buffer;
      memset(buffer, 0, sizeof(buffer)); //revisar su no devuelve nulo o resetea y bugea todo
    }


    void Enviar(){
        int retorno=0;
        int aux=0;
        for(int i=0;i<1024;i++){
            if(buffer[i]=='1'){
                i=1024;
            if(aux==-4){ send(client,"algo", sizeof(buffer), 0); }
            strcpy(buffer,itoa(aux,buffer,10));
            send(client,buffer, sizeof(buffer), 0);
            }//1

            if(buffer[i]== '2'){
            i=1024;
            char auxChar[1024]=" ";
            FILE *puntero;
            puntero=fopen("server.log","r");
            while(!feof(puntero)){
                fgets(auxChar,1024,puntero);
                cout<< auxChar<<endl;
                send(client,auxChar,sizeof(auxChar),0);
                memset(auxChar, 0, sizeof(auxChar));

            }


                fclose(puntero);

            }//2

            if(buffer[i]=='4'){
                i=1024;
                CerrarSocketPorInactividad();
                cout << "El cliente se desconecto por inactividad"<< endl;
                 ConexionDeSocket();
            }//4

             if(buffer[i]=='0'){
                 i=1024;
                 CerrarSocket();
                 cout << "El cliente se desconecto"<< endl;
                 ConexionDeSocket();
             }//0
        }//for

        memset(buffer, 0, sizeof(buffer));

    }


    void CerrarSocket(){
        //Declaramos la fecha/hora del dia.
        time_t now = time(0);
        tm* time = localtime(&now);
        int dia = time-> tm_mday;
        int mes = time-> tm_mon + 1;
        int anio = 1900 + time-> tm_year;
        int hora = time-> tm_hour;
        int minu = time-> tm_min;


        string nombreArchivo = "server.log";
        ofstream texto;

        texto.open(nombreArchivo.c_str(),fstream::out);//abrimos el txt

        closesocket(client);
        cout << "Socket cerrado, cliente desconectado." << endl;
        texto << endl;
         texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ":Socket cerrado, cliente desconectado." << endl;

        texto.close();//cerramos
    }

    void CerrarSocketPorInactividad(){
     //Declaramos la fecha/hora del dia.
        time_t now = time(0);
        tm* time = localtime(&now);
        int dia = time-> tm_mday;
        int mes = time-> tm_mon + 1;
        int anio = 1900 + time-> tm_year;
        int hora = time-> tm_hour;
        int minu = time-> tm_min;


        string nombreArchivo = "server.log";
        ofstream texto;

        texto.open(nombreArchivo.c_str(),fstream::out);//abrimos el txt

        closesocket(client);
        cout << "Socket cerrado, cliente desconectado por inactividad." << endl;
        texto << endl;
         texto << anio <<"-"<< mes <<"-"<< dia <<"_"<< hora <<":"<< minu << ":Conexion Cerrada por Inactividad." << endl;

        texto.close();//cerramos

    }

    int calculo(){
        int a=0;
        int b=0;
        char auxPreDig[1024]= " ";
        char auxPosDig[1024]= " ";
        char letra[5]=" ";
        bool esLetra=false;
        char aux[10]=" ";
        bool caracterNoDigito= false;
        int caracter=0;
        int posicionCaracter=0;
        int resultado=0;
        enum opertacion{Suma,Resta,Multiplicacion,Division,Potenciacion,Factorial};

        if(strlen(buffer)>1 && strlen(buffer)<20){ //minimo un caracter maximo 20

        for(int i=0;i<strlen(buffer);i++){
            if(buffer[i]== '+'){
                caracter=Suma;
                posicionCaracter=i;
                if(isdigit(buffer[i+1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    } else if (isdigit(buffer[i-1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    }

            } else if(buffer[i]== '-'){
                caracter=Resta;
                posicionCaracter=i;
                if(isdigit(buffer[i+1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    } else if (isdigit(buffer[i-1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    }

                }else if(buffer[i]=='*'){
                caracter=Multiplicacion;
                posicionCaracter=i;
                if(isdigit(buffer[i+1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    } else if (isdigit(buffer[i-1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    }

                }else if(buffer[i]=='/'){
                caracter=Division;
                posicionCaracter=i;
                if(isdigit(buffer[i+1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    } else if (isdigit(buffer[i-1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    }

                } else if(buffer[i]=='^'){
                caracter=Potenciacion;
                posicionCaracter=i;
                if(isdigit(buffer[i+1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    } else if (isdigit(buffer[i-1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    }


                } else if (buffer[i]=='!'){
                 caracter=Factorial;
                posicionCaracter=i;
                if(isdigit(buffer[i+1])== false){
                    caracterNoDigito = true;
                    aux[0]=buffer[i];
                    aux[1]=buffer[i+1];
                    aux[2]=buffer[i+2];
                    }

                }     //el if de los caracteres

            if(isdigit(buffer[i])== false){
                esLetra=true;
                posicionCaracter=i;
                letra[0]=buffer[i];
            }
        }


        for(int j=0;j<posicionCaracter;j++){
            if(isdigit(buffer[j])==true){
                    auxPreDig[j]=buffer[j];
            }//if
        }//for
        if(esLetra == false && caracterNoDigito == false){
                    a=atoi(auxPreDig);
                }
        int r=0;
        for(r=posicionCaracter+1;r<strlen(buffer);r++){
           if(isdigit(buffer[r])==true){
                    auxPosDig[r]=buffer[r];
            }

        }
        if(esLetra == false && caracterNoDigito == false){
                    b=atoi(auxPosDig);
                }


                 switch(caracter){
                    case (Suma):
                        resultado=a+b;
                        break;
                    case(Resta):
                        resultado=a-b;
                        break;
                    case(Multiplicacion):
                        resultado=a*b;
                        break;
                    case(Division):
                        resultado=a/b;
                        break;
                    case(Potenciacion):
                        resultado=pow(a,b);
                        break;
                    case(Factorial):
                        resultado=1;
                        for(int i=1;i<=a;i++){resultado=resultado*i; }
                        break;

                    }

                    if (esLetra == true){
                        resultado = -1;
                        char auxChar[1024]="No se pudo realizar la operacion,se encontro un caracter no contemplado:";
                        strcpy(buffer,auxChar);
                        strcat(buffer,letra);
                        send(client,buffer, sizeof(buffer), 0);

                        cout << " Error: "<< buffer << endl;
                    }


                    if (caracterNoDigito == true){
                        resultado = -2;
                        char auxChar2[1024]="No se pudo realizar la operacion,la operacion esta mal formada: ";
                        strcpy(buffer,auxChar2);
                        strcat(buffer,aux);
                        send(client,buffer, sizeof(buffer), 0);

                        cout << " Error: "<< buffer << endl;
                    }



        }else{
            resultado= -3;
           char auxChar3[1024]="La operacion debe tener entre 1 y 20 caracteres.";
           strcpy(buffer,auxChar3);
             cout << " Error: "<< buffer << endl;
            send(client,buffer,sizeof(buffer),0);

        }

        if(strcmp(buffer,"volver") == 0){
            resultado=-4;
        }

        if(strcmp(buffer,"VOLVER") == 0){
            resultado=-4;
        }


        return resultado;
    }


};





int main()
{
  Server *Servidor = new Server();
    while(true){
         Servidor->Recibir();
         Servidor->Enviar();

    }
}
