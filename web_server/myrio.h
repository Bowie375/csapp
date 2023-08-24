#include<unistd.h>
#include<sys/errno.h>
#include<cstring>

#define RIO_BUFSIZE 8192

typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

ssize_t rio_readn(int fd, void* usrbuf, size_t num){
    size_t nleft = num;
    size_t nread = 0;
    char* buf=static_cast<char*>(usrbuf);
    
    while(nread == 0){
        size_t readed = read(fd,buf,nleft);
        if(readed < 0){
            if(errno == EINTR){
                readed = 0;
            }
            else{
                return -1;
            }
        }
        else if(readed == 0){
            break;
        }

        nleft-=readed;
        nread+=readed;
    }
    return nread;
}

ssize_t rio_writen(int fd, void* usrbuf, size_t num){
    size_t nwrite = 0;
    size_t nleft = num;
    char* buf = static_cast<char*>(usrbuf);

    while(nwrite < num){
        size_t writen = 0;
        if((writen=write(fd,buf,nleft))<0){
            if(errno == EINTR){
                writen = 0;
            }
            else{
                return -1;
            }
        }
        else if(writen==0){
            break;
        }

        nleft-=writen;
        nwrite+=writen;
    }

    return nwrite;
}

void rio_readintib(rio_t* rp, int fd){
    rp->rio_fd=fd;
    rp->rio_cnt=0;
    rp->rio_bufptr=rp->rio_buf;
}

ssize_t rio_read(rio_t* rp, void* usrbuf, size_t maxlen){
    if(rp->rio_cnt==0){
        ssize_t nread=rio_readn(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
        if(nread<0){
            return -1;
        }
        else if(nread==0){
            return 0;
        }
        else{
            rp->rio_cnt=nread;
            rp->rio_bufptr=rp->rio_buf;
        }
    }

    size_t nmove = (maxlen < rp->rio_cnt ? maxlen : rp->rio_cnt);
    memcpy(usrbuf,rp->rio_bufptr,nmove);
    rp->rio_cnt-=nmove;
    rp->rio_bufptr+=nmove;

    return nmove;
}

ssize_t rio_readnb(rio_t* rp,void* usrbuf, size_t num){
    size_t nleft = num;
    ssize_t nread;
    char *bufp = static_cast<char*>(usrbuf);

    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1;          /* errno set by read() */
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (num - nleft);         /* Return >= 0 */
}

ssize_t rio_readlineb(rio_t* rp, void* usrbuf, size_t maxlen){
    size_t nread = 0;
    size_t stat = 0;
    char* buf =static_cast<char*>(usrbuf);

    if(maxlen==0){
        return 0;
    }

    while((stat=rio_read(rp,buf,1))>0 && nread < (maxlen - 1)){
        if((*buf)=='\n'){
            nread++;
            buf++;
            break;
        }
        nread++;
        buf++;
    }

    if(stat < 0){
        return -1;
    }

    (*buf)='\0';
    return nread;
}