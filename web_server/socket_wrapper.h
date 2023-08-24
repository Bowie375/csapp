#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netdb.h>
#include<cstring>
#include<stdio.h>
#include<unistd.h>

#define LISTENQ 1024
typedef struct sockaddr SA;

int open_listenfd(char* port){
    int listenfd = 0,optval=1;
    addrinfo hints,*listp,*p;

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags= AI_ADDRCONFIG| AI_PASSIVE;
    hints.ai_flags=AI_NUMERICSERV;
    int rt=getaddrinfo(NULL,port,&hints,&listp);

    if(rt!=0){
        fprintf(stderr,"getaddrinfo : %s\n",gai_strerror(rt));
        exit(1);
    }

    for(p=listp;p;p=p->ai_next){
        listenfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(listenfd<0){
            continue;
        }
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));
        if(bind(listenfd,p->ai_addr,p->ai_addrlen)==0){
            break;
        }
        close(listenfd);
    }

    freeaddrinfo(listp);
    if(!p){
        fprintf(stdout,"errno : %d\n",errno);
        return -1;
    }
    if(listen(listenfd,LISTENQ)<0){
        close(listenfd);
        return -1;
    }
    //fprintf(stdout,"I'm here with listenfd %d\n",listenfd);
    return listenfd;
}

int open_clientfd(char* hostname,char* port){
    int clientfd = 0;
    struct addrinfo hints, *listp, *p;

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    int rt=getaddrinfo(hostname,port,&hints,&listp);

    if(rt!=0){
        fprintf(stderr,"getaddrinfo : %s\n",gai_strerror(rt));
        exit(1);
    }

    for(p=listp;p;p=p->ai_next){
        clientfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(clientfd>=0){
            if(connect(clientfd,(p->ai_addr),p->ai_addrlen)!=0){
                continue;
            }
            else{
                break;
            }
            close(clientfd);
        }
    }

    freeaddrinfo(listp);
    if(!p){
        return -1;
    }
    return clientfd;
}