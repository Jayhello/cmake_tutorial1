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
#include <s2sClientIf.h>

/*
 * 场景: 建立多个会话, 注册多个服务；
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
		fd = ptr->initialize("s2sdemo","727201fcf2acbe10",TEXTPLAIN);
		
		// monitor fd, sdk有状态更新，订阅更新，通过它通知; 
		ev_io_init(&ioRead, &(ev_io_cb<MyApp, &MyApp::onReadData, ev_io>), fd, EV_READ);
		ioRead.data = this;
		ev_io_start(EV_A_ &ioRead);

		start();
		
		return 0;

	}

	void start()
	{
		std::string data = appName + ":service info";
		ptr->setMine(data); 	
	}

	void onReadData(int revents)
	{
		
		std::vector<S2sMeta> metas;
		S2sSessionStatus status = ptr->pollNotify(metas);

		// 如果关注sdk的状态，请先处理status; 
		// handleStatus();
		printf("meta status =%u\n", status);
	}

	struct	ev_loop *loop;
	std::string appName;
private:
	IMetaServer* ptr;
	int fd;
	struct ev_timer timer;
	struct ev_io ioRead;
};


using namespace std;
int main(int argc, char*argv[])
{
    signal(SIGHUP, SIG_IGN);

    signal(SIGPIPE, SIG_IGN);



	MyApp app1;
	MyApp app2;
	
  	struct	ev_loop *loop = ev_default_loop(EVBACKEND_EPOLL);
	app1.loop = loop;
	app2.loop = loop;

	app1.appName = "app1";
	app2.appName = "app2";
	
	app1.init();  // 将启动会话1
	app2.init();  // 将启动会话2

	ev_run(EV_A);
}



