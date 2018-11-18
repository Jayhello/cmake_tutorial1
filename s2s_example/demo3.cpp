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
 * 场景: 初始化后同时订阅和注册；
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
		// 初始化与监听fd
		configRecoverTime(300);
		ptr =  newMetaServer();
		fd = ptr->initialize("s2sdemo","727201fcf2acbe10",(MetaType)128);
		
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
		f.interestedName = "svc_dispatcher";
		f.interestedGroup = 0;
		f.s2sType = ANY_TYPE;
		
		std::vector <SubFilter> filtes;
		filtes.push_back(f);
		// 订阅
		ptr->subscribe(filtes);	

		// 注册
		std::string data = "service info";
		ptr->setMine(data);

	}

	void onReadData(int revents)
	{
		
		std::vector<S2sMeta> metas;
		S2sSessionStatus status = ptr->pollNotify(metas);

		// 如果关注sdk的状态，请先处理status; 
		// handleStatus();
		printf("meta status =%u\n", status);
		
		hanldeMetas(metas); // 处理订阅到的服务
    //            ptr->delMine();
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
				S2sDataDecoder decoder(m.data);
				std::map<S2S::ISPType, uint32_t> ips;
				if(decoder.readIpList(ips) ==0)
				{
					printf("ips size = %u", ips.size());
				}
			}
		}
	}
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



