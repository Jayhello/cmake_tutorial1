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

using namespace std;

/*
 * 场景: 多次订阅，判断是否订阅到全部数据；
 */

template<class T, void (T::*method)(int), class W> void ev_io_cb(EV_P_ W *ev, int events)
{
	T *o = static_cast<T*>(ev->data);
	(o->*method)(events);
}

std::vector<std::string> g_subscribes;
uint32_t g_subIndex = 0;

std::string getNextSubscribe()
{
	std::string name;
	if (g_subIndex < g_subscribes.size())
	{
		name = g_subscribes[g_subIndex++];
	}

	return name;
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

		setMine();

		return 0;
	}

	void setMine()
	{
		std::string data = "service info";
		ptr->setMine(data);
	}

	void subscribe(const std::string &name)
	{
		SubFilter f;
		//f.interestedName = "uniap";
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
		printf("meta status =%u\n", status);

		bool isPullAll = ptr->isPullAllSub();
		printf("before isPullAll = %s\n", isPullAll ? "true" : "false");

		hanldeMetas(metas); // 处理订阅到的服务

		std::string name = getNextSubscribe();
		if (!name.empty())
		{
			subscribe(name);
		}

		isPullAll = ptr->isPullAllSub();
		bool ispull = ptr->isSubscribePulled(name);
		printf("after isPullAll = %s\n", isPullAll ? "true" : "false");
		printf("after name:%s isPullAll = %s\n", name.c_str(), ispull ? "true" : "false");
	}

	struct	ev_loop *loop;
private:
	void hanldeMetas(std::vector<S2sMeta>& metas)
	{
		for(size_t i = 0; i<metas.size(); ++i)
		{
			S2sMeta& m = metas[i];
			bool isPulled = ptr->isSubscribePulled(m.name);
			printf("serverId = %lld, name = %s, groupId =%d,type =%d, timeStamp = %lld, status = %d isPulled=%s\n",
				m.serverId, m.name.c_str(), m.groupId, m.type, m.timestamp,m.status, isPulled?"true":"false");

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
	bool isSet;
	IMetaServer* ptr;
	int fd;
	struct ev_timer timer;
	struct ev_io ioRead;
};

int main(int argc, char*argv[])
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    g_subscribes.push_back("s2sdemo2");
    g_subscribes.push_back("exuinfo_db_proxy_mysql_s2sd");
    g_subscribes.push_back("bt_wd_pub_s2sd");
    g_subIndex = 0;

	MyApp app;

  	struct	ev_loop *loop = ev_default_loop(EVBACKEND_EPOLL);
	app.loop = loop;

	app.init();

	ev_run(EV_A);
}



