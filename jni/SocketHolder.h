#ifndef _Included_SocketHolder
#define _Included_SocketHolder

#include <string.h>

using namespace std;

class SocketHolder{



int mSocket;


public:
	~SocketHolder();
	SocketHolder();
	int lt_connect(const char* host, int port);
	int lt_init();
	int lt_send(const char* content, int length);

	int lt_readSync(char*buffer, int buf_length);

	int lt_setKeepalive(int tcp_keepalive_probes, int tcp_keepalive_time, int tcp_keepalive_intvl);

};



#endif


