#include "myserver.h"

static size_t MAXLINE = 1024;

void answer(int connfd);
void restore_kidpid();
void clienterror(int connfd, char* name, 
            const char* errcode,const char* overall_msg,const char* detail_msg);
void serve_static(int connfd, char* filename, size_t filesize);
void serve_dynamic(int connfd, char* filename, char* cgiargs);
void read_requesthdrs(rio_t *rp);
void get_filetype(char* filename, char* filetype);
int parse_uri(char* uri,char* filename,char* cgiargs);

int main(int argc, char* argv[]){
    int listenfd = 0;
    int connfd = 0;
    char hostname[MAXLINE],port[MAXLINE];
    sockaddr_storage clientaddr;
    socklen_t clientlen = sizeof(struct sockaddr_storage);
    
    if(argc != 2){
        fprintf(stdout,"usage : %s < port >\n",argv[0]);
        exit(1);
    }

    listenfd = open_listenfd(argv[1]);

    if(listenfd<0){
        fprintf(stdout,"error : open_listenfd doesn't work.\n");
        exit(1);
    }

    while(1){
        fprintf(stdout,"waiting for a request...\n");
        connfd = accept(listenfd,(SA*)&clientaddr,&clientlen);

        if(connfd < 0){
            fprintf(stdout,"accept : connection collapsed !\n");
            exit(1);
        }

        if(fork()==0){
            getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
            fprintf(stdout,"accepted a request form (%s : %s), answering now...\n",hostname,port);
            answer(connfd);
            close(connfd);
            exit(0);
        }
        /*
        getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        fprintf(stdout,"accepted a request form (%s : %s), answering now...\n",hostname,port);
        answer(connfd);
        close(connfd);*/
        
        close(connfd);
        restore_kidpid();
    }

    return 0;
}

void restore_kidpid(){
    while(waitpid(-1,NULL,WNOHANG)){;}
}

void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    
    rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n")) {
        rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}

void answer(int connfd){
    int is_static = 0;
    struct stat sbuf;
    char usrbuf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    char filename[MAXLINE],cgiargs[MAXLINE];
    rio_t buf;

    rio_readintib(&buf,connfd);

    int state=rio_readlineb(&buf,usrbuf,MAXLINE);
    if(state<0){
        fprintf(stdout,"error : answer -> rio_readlineb doesn't work.");
        return;
    }
    
    read_requesthdrs(&buf);

    printf("Request header: %s\n",usrbuf);
    sscanf(usrbuf,"%s %s %s",method,uri,version);

    if(strcasecmp(method,"GET")){
        clienterror(connfd,method,"501","Not implemented","we don't serve this.");
        return;
    }

    is_static = parse_uri(uri,filename,cgiargs);

    if(stat(filename,&sbuf)<0){
        clienterror(connfd,filename,"404","Not found","the file doesn't exist.");
        return;
    }

    if(is_static){
        if(!(S_ISREG(sbuf.st_mode))||!(sbuf.st_mode & S_IRUSR)){
            clienterror(connfd,filename,"403","Forbidden","you have no access to this file.");
            return;
        }
        serve_static(connfd,filename,sbuf.st_size);
    }
    else{
        if(!(S_ISREG(sbuf.st_mode))||!(S_IXUSR & sbuf.st_mode)){
            clienterror(connfd,filename,"403","Forbidden","you can't run this program.");
            return;
        }
        serve_dynamic(connfd,filename,cgiargs);
    }
}

int parse_uri(char* uri,char* filename,char* cgiargs){
    if(!strstr(uri,"cgi-bin")){
        strcpy(cgiargs,"");
        strcpy(filename,".");
        strcat(filename,uri);
        if(uri[strlen(uri)-1]=='/'){
            strcat(filename,"home.html");
        }
        return 1;
    }
    else{
        char *p=index(uri,'?');
        if(!p){
            strcpy(cgiargs,"");
        }
        else{
            p++;
            strcpy(cgiargs,p);
            p--;
            (*p)='\0';
        }
        strcpy(filename,".");
        strcat(filename,uri);
        return 0;
    }
}

void clienterror(int connfd, char* name, 
        const char* errcode, const char* overall_msg, const char* detail_msg){
    char buf[MAXLINE],body[MAXLINE];

    sprintf(body,"<html><title>Error</title>");
    sprintf(body,"%s<body bgcolor=""ffffff>""\r\n",body);
    sprintf(body,"%s%s : %s\r\n",body,errcode,overall_msg);
    sprintf(body,"%s<p>%s : %s\r\n",body,detail_msg,name);
    sprintf(body,"%s<hr><em>Tiny web_server from Bowie</em>\r\n",body);

    sprintf(buf,"HTTP/1.0 %s %s\r\n",errcode,overall_msg);
    rio_writen(connfd,buf,strlen(buf));
    sprintf(buf,"Content-type: text/html\r\n");
    rio_writen(connfd,buf,strlen(buf));
    sprintf(buf,"Content-length: %d\r\n\r\n",(int)strlen(body));
    rio_writen(connfd,buf,strlen(buf));
    rio_writen(connfd,body,strlen(body));
}

void get_filetype(char* filename, char* filetype){
    if(strstr(filename,".html")){
        strcpy(filetype,"text/html");
    }
    else if(strstr(filename,".png")){
        strcpy(filetype,"image/png");
        return;
    }
    else if(strstr(filename,".jpg")){
        strcpy(filetype,"image/jpg");
        return;
    }
    else{
        strcpy(filetype,"text/plain");
        return;
    }
}

void serve_static(int connfd, char* filename, size_t filesize){
    int srcfd;
    char buf[MAXLINE];
    char filetype[MAXLINE];
    char* srcp;

    get_filetype(filename,filetype);

    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    sprintf(buf,"%sServer Tiny web_server from Bowie\r\n",buf);
    sprintf(buf,"%sConnection: close\r\n",buf);
    sprintf(buf,"%sContent-type: %s\r\n",buf,filetype);
    sprintf(buf,"%sContent-length: %d\r\n\r\n",buf,(int)filesize);
    rio_writen(connfd,buf,strlen(buf));

    srcfd = open(filename,O_RDONLY,0);
    srcp=(char*)mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
    close(srcfd);
    rio_writen(connfd,srcp,filesize);
    munmap(srcp,filesize);
}

void serve_dynamic(int connfd, char* filename, char* cgiargs){
    char buf[MAXLINE],*emptylist[]={NULL};

    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    sprintf(buf,"%sServer Tiny web_server from Bowie\r\n",buf);
    sprintf(buf,"%sConnection: close\r\n",buf);
    rio_writen(connfd,buf,strlen(buf));

    if(fork()==0){
        setenv("QUERY_STRING",cgiargs,1);
        dup2(connfd,STDOUT_FILENO);
        execve(filename,emptylist,environ);
    }
    wait(NULL);
}