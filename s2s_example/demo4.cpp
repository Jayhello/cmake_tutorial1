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
 * 场景: 订阅成功后，再注册；
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
		isSet = false;
		ptr =  newMetaServer();
		fd = ptr->initialize("s2sdemo","16c76e1bdbc1014c1b49b39181daca23",TEXTPLAIN);
		
		// monitor fd, sdk有状态更新，订阅更新，通过它通知; 
		ev_io_init(&ioRead, &(ev_io_cb<MyApp, &MyApp::onReadData, ev_io>), fd, EV_READ);
		ioRead.data = this;
		ev_io_start(EV_A_ &ioRead);

		start();
		
		return 0;

	}

	void start()
	{
		SubFilter f;
		//f.interestedName = "uniap";
		f.interestedName = "s2sdemo2";
		f.interestedGroup = 0;
		f.s2sType = ANY_TYPE;
		
		std::vector <SubFilter> filtes;
		filtes.push_back(f);
		ptr->subscribe(filtes);	
	}

	void onReadData(int revents)
	{
		
		std::vector<S2sMeta> metas;
		S2sSessionStatus status = ptr->pollNotify(metas);

		// 如果关注sdk的状态，请先处理status; 
		// handleStatus();
		printf("meta status =%u\n", status);
		
		hanldeMetas(metas); // 处理订阅到的服务
	}

	struct	ev_loop *loop;
private:
	void hanldeMetas(std::vector<S2sMeta>& metas)
	{
		for(size_t i = 0; i<metas.size(); ++i)
		{
			S2sMeta& m = metas[i];
			printf("serverId = %lld, name = %s, groupId =%d,type =%d, timeStamp = %lld, status = %d\n", m.serverId, m.name.c_str(), m.groupId, m.type, m.timestamp,m.status);
			if(metas[i].status == S2SMETA_DIED_C)
			{
				// remove
			}
			else
			{
				// update
			}
		}

		// 已经获取到依赖服务的信息，本节点可以开始对外提供服务，注册服务信息；
		if (ptr->isPullAllSub())
		//if(!isSet)
		{
			printf("subscrible is ok, start and set Mine");
			isSet = true;
			std::string data = "service info";
			ptr->setMine(data);	
		}
	}
private:
	bool isSet;
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



