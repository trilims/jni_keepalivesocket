#include "common_net_keepalive_LTSocketImpl.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/in.h>

#include <stdio.h>
#include <signal.h>


#include "SocketHolder.h"

static SocketHolder *mpSocketHolder = NULL;

#ifdef __cplusplus
extern "C"
{
#endif

  static JNIEnv * mJNIEnv = NULL;
  static jobject* mJobject = NULL;

  void sigroutine (int dunno)
  {				/* 信号处理例程，其中dunno将会得到信号的值 */
    switch (dunno)
      {
      case 1:
	printf ("Get a signal -- SIGHUP ");
	break;
	case 2:printf ("Get a signal -- SIGINT ");
	break;
	case SIGPIPE:{
		printf ("Get a signal -- SIGPIPE ");
		 if(mJNIEnv != NULL){ jclass cls = mJNIEnv->GetObjectClass (*mJobject);
  jmethodID callback = mJNIEnv->GetMethodID (cls, "callback", "(II)V");
  mJNIEnv->CallVoidMethod (*mJobject, callback, -1, SIGPIPE);
		 	}
		}
	
	break;
      }
    return;
  }




#ifdef __cplusplus
}
#endif


JNIEXPORT jint JNICALL Java_common_net_keepalive_LTSocketImpl_init
  (JNIEnv * env, jobject obj)
{
	mJNIEnv= env;
	mJobject = &obj;

  jclass cls = env->GetObjectClass (obj);
  jmethodID callback = env->GetMethodID (cls, "callback", "(II)V");
  env->CallVoidMethod (obj, callback, 5, 10);

  if (mpSocketHolder != NULL)
    {
      delete mpSocketHolder;
	  mpSocketHolder = NULL;
    }

  mpSocketHolder = new SocketHolder ();

  int result = mpSocketHolder->lt_init();

return result;

}


/*
 * Class:     common_net_keepalive_LTSocketImpl
 * Method:    connect
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_common_net_keepalive_LTSocketImpl_connect
  (JNIEnv * env, jobject obj, jstring host, jint port)
{

  const char *cHost = env->GetStringUTFChars (host, 0);

  int result = mpSocketHolder->lt_connect (cHost, port);

  return result;

}

/*
 * Class:     common_net_keepalive_LTSocketImpl
 * Method:    send
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_common_net_keepalive_LTSocketImpl_send
  (JNIEnv * env, jobject obj, jbyteArray content, jint length)
{
  jbyte *olddata = (jbyte *) env->GetByteArrayElements (content, 0);
  jsize oldsize = env->GetArrayLength (content);
  char *bytearr = (char *) olddata;
  int result = mpSocketHolder->lt_send (bytearr, length);
  return result;

}


	/*
	 * Class:	  common_net_keepalive_LTSocketImpl
	 * Method:	  readBlock
	 * Signature: ([BI)I
	 */
	JNIEXPORT jint JNICALL Java_common_net_keepalive_LTSocketImpl_readBlock
	  (JNIEnv * env, jobject obj, jbyteArray buf, jint buf_length)

{
	char* buffer =(char*) env->GetByteArrayElements(buf,0);
  int result = mpSocketHolder->lt_readSync (buffer, buf_length);

  return result;

}


/*
 * Class:     common_net_keepalive_LTSocketImpl
 * Method:    delete
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_common_net_keepalive_LTSocketImpl_delete
  (JNIEnv * env, jobject obj){
    if (mpSocketHolder != NULL)
    {
      delete mpSocketHolder;
	  mpSocketHolder = NULL;
    }
	return 0;
  
}


/*
 * Class:     common_net_keepalive_LTSocketImpl
 * Method:    setKeepalive
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_common_net_keepalive_LTSocketImpl_setKeepalive
  (JNIEnv *env , jobject obj , jint tcp_keepalive_probes, 
  jint tcp_keepalive_time, jint tcp_keepalive_intvl){
  if (mpSocketHolder != NULL)
    {
    return mpSocketHolder->lt_setKeepalive(tcp_keepalive_probes,tcp_keepalive_time,tcp_keepalive_intvl);
  	}
}



