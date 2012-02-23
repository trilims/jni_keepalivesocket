
#include "SocketHolder.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <signal.h>

extern "C"
{
  void sigroutine (int dunno);
}

SocketHolder::SocketHolder ()
{
  signal (SIGPIPE, sigroutine);

};




SocketHolder::~SocketHolder ()
{
  if (mSocket > 0)
    {
      close (mSocket);
	  mSocket = 0;
    }
};


int
SocketHolder::lt_connect (const char *sHost, int nPort)
{

  if (mSocket < 0)
    {
      return -1;
    }

  struct hostent *host;
  struct sockaddr_in serv_addr;

  if ((host = gethostbyname (sHost)) == NULL)
    {
      herror ("gethostbyname出错！");
      return -2;
    }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (nPort);
  serv_addr.sin_addr = *((struct in_addr *) host->h_addr);
  bzero (&(serv_addr.sin_zero), 8);
  if (connect
      (mSocket, (struct sockaddr *) &serv_addr,
       sizeof (struct sockaddr)) == -1)
    {
      perror ("connect出错！");
      return -3;
    }


  return 0;
}


int
SocketHolder::lt_init ()
{
  if ((mSocket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("socket创建出错！");
      mSocket = 0;
      return -1;
    }




  return 0;
}

int
SocketHolder::lt_send (const char *content, int length)
{

  int sendbytes = -1;
  if(mSocket >0){
  if ((sendbytes = send (mSocket, content, length, 0)) == -1)
    {
      perror ("send出错！");
      return -1;
    }
  	}


  return sendbytes;
}

int
SocketHolder::lt_readSync (char *buffer, int buf_length)
{
  int readbyte = -1;

  if(mSocket >0){

  readbyte = recv (mSocket, buffer, buf_length, 0);

  if (readbyte == -1)
    {
      perror ("recv出错！");
      return -1;
    }

  	}
  return readbyte;
}

int
SocketHolder::lt_setKeepalive(int tcp_keepalive_probes, int tcp_keepalive_time, int tcp_keepalive_intvl){
	   /* TCP_KEEPCNT: overrides tcp_keepalive_probes */

if(tcp_keepalive_probes > 0){

	   int optval;
	   socklen_t optlen = sizeof (optval);
	   
	   /* Check the status for the keepalive option */
	   if (getsockopt (mSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0)
		 {
		   perror ("getsockopt()");
		   close (mSocket);
		   mSocket = 0;
		   return -2;
		 }
	   printf ("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));
	   
	   /* Set the option active */
	   optval = 1;
	   optlen = sizeof (optval);
	   if (setsockopt (mSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0)
		 {
		   perror ("setsockopt()");
		   close (mSocket);
		   mSocket = 0;
		   return -3;
		 }
	   printf ("SO_KEEPALIVE set on socket\n");
	   
	   
	   
	   
	   
	   /* Check the status again */
	   if (getsockopt (mSocket, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0)
		 {
		   perror ("getsockopt()");
		   close (mSocket);
		   mSocket = 0;
		   return -4;
		 }


	

	 if (setsockopt (mSocket, SOL_TCP , TCP_KEEPCNT, &optval, optlen) < 0)
	   {
		 perror ("setsockopt()");
		 close (mSocket);
		 mSocket = 0;
		 return -5;
	   }
	 printf ("TCP_KEEPCNT set on socket\n");
	
		 /* TCP_KEEPIDLE: overrides tcp_keepalive_time */
	 optval = tcp_keepalive_time;
	 optlen = sizeof (optval);
	 if (setsockopt (mSocket, SOL_TCP , TCP_KEEPIDLE, &optval, optlen) < 0)
	   {
		 perror ("setsockopt()");
		 close (mSocket);
		 mSocket = 0;
		 return -6;
	   }
	 printf ("TCP_KEEPIDLE set on socket\n");
	
		 /* TCP_KEEPINTVL: overrides tcp_keepalive_intvl */
	 optval = tcp_keepalive_intvl;
	 optlen = sizeof (optval);
	 if (setsockopt (mSocket, SOL_TCP , TCP_KEEPINTVL, &optval, optlen) < 0)
	   {
		 perror ("setsockopt()");
		 close (mSocket);
		 mSocket = 0;
		 return -7;
	   }
	 printf ("TCP_KEEPINTVL set on socket\n");

	 return 0;
}else{

}

}

