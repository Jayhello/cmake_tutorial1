#define _GLIBCXX_USE_CXX11_ABI 0
#include <ev++.h>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include "s2sClientIf.h"

using std::cout;
using std::endl;

/*
 * 场景: 只注册不订阅；
 */

template<class T, void (T::*method)(int), class W> void ev_io_cb(EV_P_ W *ev, int events)
{
	T *o = static_cast<T*>(ev->data);
	(o->*method)(events);
}


class MyApp
{
public:
	int init()
	{
		

		ptr =  newMetaServer();
		//ptr->setLostCheckType(NoLostCheck_C);
		fd = ptr->initialize("xy_test_s2s_usage","6427d483e41014088b3dc1866ea37034b8e642ffd5ffba87ad0d30620d03a0441df6d1db8403b074", S2SDECODER);
		
		// monitor fd, sdk有状态更新，订阅更新，通过它通知; 
		ev_io_init(&ioRead, &(ev_io_cb<MyApp, &MyApp::onReadData, ev_io>), fd, EV_READ);
		ioRead.data = this;
		ev_io_start(EV_A_ &ioRead);

		start();
		
		return 0;

	}

	void start()
	{		
//		std::string data = "service info";

		S2sDataEncoder encoder;

		encoder.writeUdpPort(10009);
		std::vector<int64_t> value;
		value.push_back(1113984275);
		encoder.insert("iplist", value);

		std::string data = encoder.endToString();
		cout<<"set mine data: "<<data<<endl;
		ptr->setMine(data);
	}

	void onReadData(int revents)
	{
		
		std::vector<S2sMeta> metas;
		S2sSessionStatus status = ptr->pollNotify(metas);

		// 如果关注sdk的状态，请先处理status; 
		// handleStatus();
		printf("meta status =%u\n", status);
		
		//hanldeMetas(metas); // 处理订阅到的服务
	}

	struct	ev_loop *loop;

private:
	IMetaServer* ptr;
	int fd;
	struct ev_timer timer;
	struct ev_io ioRead;
};

class TetsExit{
public:
    TetsExit(){
        cout<<"new item"<<endl;
    }

    ~TetsExit(){
        cout<<"~~~~~~~new item"<<endl;
    }
};


using namespace std;
int main(int argc, char*argv[])
{
    signal(SIGHUP, SIG_IGN);

    signal(SIGPIPE, SIG_IGN);



	MyApp app;

    TetsExit te;
	
  	struct	ev_loop *loop = ev_default_loop(EVBACKEND_EPOLL);
	app.loop = loop;

	app.init();

	ev_run(EV_A);
}



