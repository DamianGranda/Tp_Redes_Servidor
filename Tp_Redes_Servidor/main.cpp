#include <iostream>
#include <winsock2.h>
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
    char buffer[1024]=" ";
    char registro[1024]=" ";




    Server(){
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5000);

        time_t tiempoahora;
        time(&tiempoahora);
        struct tm *mitiempo=localtime(&tiempoahora);

        FILE * puntero;
        puntero=fopen("Server.log","w");
        fprintf(puntero,"%d-%d-%d_%d:%d ==========================\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
        fprintf(puntero,"%d-%d-%d_%d:%d ========INICIO SERVIDOR========\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
        fprintf(puntero,"%d-%d-%d_%d:%d ==========================\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);

        fclose(puntero);

        Conexion();
    }
    void Conexion(){


        bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
        listen(server, 0);

         time_t tiempoahora;
        time(&tiempoahora);
        struct tm *mitiempo=localtime(&tiempoahora);


        FILE *puntero;
        puntero=fopen("server.log","a");
        fprintf(puntero," %d-%d-%d_%d:%d --Socket Creado.Escuchando para conexiones entrantes.Puerto de escucha:5000\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
        system("cls");
        cout << "Escuchando para conexiones entrantes." << endl;
        int clientAddrSize = sizeof(clientAddr);
        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            fprintf(puntero," %d-%d-%d_%d:%d --Conexion aceptada.\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
            fprintf(puntero,"%d-%d-%d_%d:%d --Cliente conectado!\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
            cout << "Cliente conectado!" << endl;

        }
        fclose(puntero);
    }
    void Recibir(){
      recv(client, buffer, sizeof(buffer), 0);
      cout << "El cliente dice: " << buffer << endl;

    }
    void Enviar(){
        int i=0;
        float aux=0;
        int opcion=atoi(buffer);
        switch(opcion){
    case 1:{
        recv(client, buffer, sizeof(buffer), 0);
                char volver[7]="volver";
                if(strcmp(buffer,volver)!=0){
                    aux=calcular();
                }else{
                    aux=-4;
                }

                if(aux==-1){
                   aux=0;
                }else if(aux==-2){
                    aux=0;
                }else if(aux==-3){
                    aux=0;
                 }else if(aux==-4){
                    send(client,"menu", sizeof(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));
                    aux=0;
                 }else{

                    strcpy(buffer,itoa(aux,buffer,10));
                    send(client,buffer, sizeof(buffer), 0);
                    aux=0;

                 }
                    break;
    }
    case 2:{
        FILE *puntero;
                puntero=fopen("server.log","r");
                while(!feof(puntero)){
                    fgets(registro,1024,puntero);
                    cout<< registro<<endl;
                    send(client,registro,sizeof(registro),0);
                    memset(registro, 0, sizeof(registro));

                }


                fclose(puntero);
                break;}
    case 0:{
        CerrarSocket();
                cout << "El cliente se desconecto"<< endl;
                Conexion();
                break;}

    case 4:{
        CerrarSocketPorInactividad();
                cout << "El cliente se desconecto"<< endl;
                Conexion();
            break;}

        }

        memset(buffer, 0, sizeof(buffer));
    }

    void CerrarSocket(){
        time_t tiempoahora;
        time(&tiempoahora);
        struct tm *mitiempo=localtime(&tiempoahora);

        FILE *puntero;
        puntero=fopen("Server.log","a");
        closesocket(client);
        cout << "Socket cerrado, cliente desconectado." << endl;
        fprintf(puntero,"%d-%d-%d_%d:%d --Socket cerrado, cliente desconectado.\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
        fclose(puntero);
    }

    void CerrarSocketPorInactividad(){
          time_t tiempoahora;
        time(&tiempoahora);
        struct tm *mitiempo=localtime(&tiempoahora);
        FILE *puntero;
        puntero=fopen("Server.log","a");
        closesocket(client);
        cout << "Socket cerrado, cliente desconectado por inactividad." << endl;
        fprintf(puntero,"%d-%d-%d_%d:%d --Socket cerrado, cliente desconectado por inactividad.\n",mitiempo->tm_mday,mitiempo->tm_mon+1,mitiempo->tm_year+1900,mitiempo->tm_hour,mitiempo->tm_min);
        fclose(puntero);
    }

    float calcular(){
        enum caracter{Suma=1,Resta,Multiplicacion,Division,Potenciacion,Factorial,Numero};
        int a;
        int b;
        float retorno=0;
        bool unidad=false;
        int tipoCaracter;
        int j=0;
        int posCaracter;
        bool caracterInes=false;
        char caracterInesperado[1024]=" ";
        bool malFormado=false;
        char OperacionMalFormada[1024]=" ";

            if(strlen(buffer)>=1 && strlen(buffer)<20){

                for(j=0;j<strlen(buffer);j++){
                        if(buffer[j+1]==' '){
                            unidad=true;
                        }

                    if(buffer[j]=='+'){
                        tipoCaracter=Suma;
                        posCaracter=j;

                        if(!isdigit(buffer[j+1])){
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j];
                            OperacionMalFormada[1]=buffer[j+1];
                            OperacionMalFormada[2]=buffer[j+2];
                        }else if(!isdigit(buffer[j-1])){
                            malFormado=true;

                            OperacionMalFormada[1]=buffer[j];
                            OperacionMalFormada[2]=buffer[j+1];
                        }

                    }else if
                    (buffer[j]=='-'){
                        tipoCaracter=Resta;
                        posCaracter=j;
                        if(!isdigit(buffer[j+1])){
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j];
                            OperacionMalFormada[1]=buffer[j+1];
                            OperacionMalFormada[2]=buffer[j+2];

                        }else if(!isdigit(buffer[j-1])){
                            malFormado=true;

                            OperacionMalFormada[1]=buffer[j];
                            OperacionMalFormada[2]=buffer[j+1];
                        }

                    }else if
                    (buffer[j]=='*'){
                        tipoCaracter=Multiplicacion;
                        posCaracter=j;

                        if(!isdigit(buffer[j+1])){
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j];
                            OperacionMalFormada[1]=buffer[j+1];
                            OperacionMalFormada[2]=buffer[j+2];
                        }else if(!isdigit(buffer[j-1])){
                            malFormado=true;

                            OperacionMalFormada[1]=buffer[j];
                            OperacionMalFormada[2]=buffer[j+1];
                        }

                    }else if
                    (buffer[j]=='/'){
                    tipoCaracter=Division;
                        posCaracter=j;

                        if(!isdigit(buffer[j+1])){
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j];
                            OperacionMalFormada[1]=buffer[j+1];
                            OperacionMalFormada[2]=buffer[j+2];
                        }else if(!isdigit(buffer[j-1])){
                            malFormado=true;
                            OperacionMalFormada[1]=buffer[j];
                            OperacionMalFormada[2]=buffer[j+1];
                        }

                    }else if
                    (buffer[j]=='^'){
                        tipoCaracter=Potenciacion;
                        posCaracter=j;

                        if(!isdigit(buffer[j+1])){
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j];
                            OperacionMalFormada[1]=buffer[j+1];
                            OperacionMalFormada[2]=buffer[j+2];
                        }else if(!isdigit(buffer[j-1])){
                            malFormado=true;
                            OperacionMalFormada[1]=buffer[j];
                            OperacionMalFormada[2]=buffer[j+1];
                        }

                    }else if
                    (buffer[j]=='!'){
                        if(isdigit(buffer[j-1]) && !isdigit(buffer[j+1])){
                        tipoCaracter=Factorial;
                        posCaracter=j;
                        }else if(!isdigit(buffer[j-1])){
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j];
                            OperacionMalFormada[1]=buffer[j+1];
                        }else {
                            malFormado=true;
                            OperacionMalFormada[0]=buffer[j-1];
                            OperacionMalFormada[1]=buffer[j];
                            OperacionMalFormada[2]=buffer[j+1];

                        }


                    }else if
                     (!isdigit(buffer[j])){
                        posCaracter=j;
                        caracterInes=true;
                        caracterInesperado[0]=buffer[j];

                     }

                }

                if(unidad==true){
                    tipoCaracter=Numero;
                }else{


                char preOp[1024]=" ";
                int i=0;
                for(i=0;i<posCaracter;i++){
                    if(isdigit(buffer[i])){
                        preOp[i]=buffer[i];
                    }
                }
                if(caracterInes==false && malFormado==false){
                    a=atoi(preOp);
                }


                int k=0;
                char aux[1024]=" ";
                for(k=posCaracter+1;k<strlen(buffer);k++){
                        if(isdigit(buffer[k])){
                            aux[k-posCaracter-1]=buffer[k];
                        }
                }
                if(caracterInes==false && malFormado==false){
                    b=atoi(aux);
                }
                }

                if(caracterInes==false && malFormado==false){

                switch(tipoCaracter){
                    case (Suma):
                        retorno=a+b;break;
                    case(Resta):
                        retorno=a-b;break;
                    case(Multiplicacion):
                        retorno=a*b;break;
                    case(Division):
                        retorno=a/b;break;
                    case(Potenciacion):
                        retorno=pow(a,b);break;
                    case(Factorial):
                        retorno=1;
                        for(int i=1;i<=a;i++){
                            retorno=retorno*i;
                        }break;
                    case(Numero):
                        retorno=atoi(buffer);break;

                    }
                }else if(malFormado==true ){
                    char auxChar2[1024]="No se pudo realizar la operacion,la operacion esta mal formada: ";
                    strcpy(buffer,auxChar2);
                    strcat(buffer,OperacionMalFormada);
                    cout << buffer<< endl;
                    retorno=-2;

                    send(client,buffer, sizeof(buffer), 0);


                }else{

                    char auxChar[1024]="No se pudo realizar la operacion,se encontro un caracter no contemplado:";
                    strcpy(buffer,auxChar);
                    strcat(buffer,caracterInesperado);
                    cout << buffer <<endl;
                    retorno=-1;
                    send(client,buffer, sizeof(buffer), 0);

                }
            }else{
                char auxChar3[1024]="La operacion debe tener entre 1 y 20 caracteres.";
                strcpy(buffer,auxChar3);
                retorno=-3;
                send(client,buffer,sizeof(buffer),0);
            }

            memset(buffer,0,sizeof(buffer));

                return retorno;

    }


};


int main(){

         Server *Servidor = new Server();
    while(true){
         Servidor->Recibir();
         Servidor->Enviar();

    }
}
