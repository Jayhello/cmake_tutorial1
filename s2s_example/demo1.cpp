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

/*
 * 场景:  只订阅不注册；
 */

template<class T, void (T::*method)(int), class W> void ev_io_cb(EV_P_ W *ev, int events)
{
	T *o = static_cast<T*>(ev->data);
	(o->*method)(events);
}


class MyApp
{
public:
	MyApp(): flag(true) {}

	int init()
	{

		ptr =  newMetaServer();
		fd = ptr->initialize("xy_test_s2s_usage_client","6427d483e41014088b3dc1866ea37034d9ed3a63a7ff752c01d56be7c6b4fe49e4a6896657759e121f734bc75e8ab1a30cb2e0de51526ceb",TEXTPLAIN);

		// monitor fd, sdk有状态更新，订阅更新，通过它通知; 
		ev_io_init(&ioRead, &(ev_io_cb<MyApp, &MyApp::onReadData, ev_io>), fd, EV_READ);
		ioRead.data = this;
		ev_io_start(EV_A_ &ioRead);

		return 0;

	}

	void subscribe(const std::string& name)
	{
		SubFilter f;
		f.interestedName = name;
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

		if (flag)
		{
			subscribe("xy_test_s2s_usage");
			flag = false;
		}
		
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

			std::vector<std::string> value;
			S2sDataDecoder decoder(m.data);
			std::map<S2S::ISPType, uint32_t> iplist;
			int ret = decoder.readIpList(iplist);
			printf("ret = %d, iplist size = %d\n", ret, iplist.size());
			


			if(metas[i].status == S2SMETA_DIED_C)
			{
				// remove
			}
			else
			{
				// update
			}
		}
	}
private:
	IMetaServer* ptr;
	int fd;
	struct ev_timer timer;
	struct ev_io ioRead;
	bool flag;
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
	app.subscribe("xy_test_s2s_usage");

	ev_run(EV_A);
}



