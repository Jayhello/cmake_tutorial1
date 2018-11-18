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
 * ����: ע��ɹ����������޸�ע����Ϣ��
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
		
		// monitor fd, sdk��״̬���£����ĸ��£�ͨ����֪ͨ; 
		ev_io_init(&ioRead, &(ev_io_cb<MyApp, &MyApp::onReadData, ev_io>), fd, EV_READ);
		ioRead.data = this;
		ev_io_start(EV_A_ &ioRead);

		start();
		
		return 0;

	}

	void start()
	{
		std::string data = "service info";
		ptr->setMine(data); 

		// 20 ���޸�ע����Ϣ,ģ���޸�ע����Ϣ�ĳ�����
		ev_timer_init(&timer, &(ev_io_cb<MyApp, &MyApp::onTimeout, ev_timer>), 20 ,0);
		timer.data = this;
		ev_timer_start(EV_A_ &timer);  		
	}

	void onReadData(int revents)
	{
		
		std::vector<S2sMeta> metas;
		S2sSessionStatus status = ptr->pollNotify(metas);

		// �����עsdk��״̬�����ȴ���status; 
		// handleStatus();
		printf("meta status =%u\n", status);
	}

	void onTimeout(int revents)
	{
		printf("update service info\n");
		std::string data = "service info, update update";
		ptr->setMine(data);		
	}

	struct	ev_loop *loop;

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



	MyApp app;
	
	
  	struct	ev_loop *loop = ev_default_loop(EVBACKEND_EPOLL);
	app.loop = loop;

	app.init();

	ev_run(EV_A);
}



