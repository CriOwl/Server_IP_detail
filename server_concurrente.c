#include<stdio.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
/*
Criowl
Creacion de un servidor concurrente por medio de sockets 
que reciba una peticcion y devuelva su direccion IP y puerto
Servidor TCP
*/
char *ip_interface="" ; // 0.0.0.0 asocia a todas las interfaces
char *service_or_port="";
int QL_CONECT=10;
int create_bind_socket(char *ip, char *port){
    //resolucion de la ip
    struct addrinfo *res, *p, hints;
    memset(&hints,0,sizeof(hints));
    int fd_local=-1;
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    int status_info=getaddrinfo(ip,port,&hints,&res);
    if (status_info !=0){
        printf("Error en el name service");
        return fd_local;
    }
    for (p=res;p!=NULL;p=p->ai_next){
        fd_local=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(fd_local<0){
            printf("El socket no se pudo crear");
            return;
        }
        int status_err=bind(fd_local,p->ai_addr,p->ai_addrlen);
        if(status_err==0){
            listen(fd_local,QL_CONECT);
            break;
        }
        close(fd_local);
        printf("Error en la asociacion del socket y la dirreccion IP");
    }
    freeaddrinfo(res);
    return fd_local;
}

char *get_addr_clt(int fd_local){
    if(fd_local<0){
        printf("El fd enviado no es el adecuado");
        return;
    }
    struct sockaddr_in addr={0};
    int status_err=getpeername(fd_local,(struct sockaddr*)&addr,sizeof(addr));
    char *ip = malloc(INET_ADDRSTRLEN);
    if(status_err!=0){
        printf("Falla en la obtencion de la direccion IP cliente");
        return;
    }
    inet_ntop(AF_INET,&addr.sin_addr,ip,sizeof(ip));
    return ip;
}
void aplication(){
    int fd_socket=create_bind_socket(ip_interface,service_or_port);
    char *ip_local;
    while(1){
        int new_socket=accept(fd_socket,NULL,NULL);
        ip_local=get_addr_clt(new_socket);
        printf("%s",ip_local);
        free(ip_local);
    }
}

int main(){

    return 0;
}