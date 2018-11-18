#ifndef __S2S_CLIENT_IF_HEADER__
#define __S2S_CLIENT_IF_HEADER__
/***************************************************
Version Record:
current version: 1.0.0
version               modification list                                   date
0.0.0                 initial version                                   	-
1.0.0                 add API:                                              2014/03/19
			onSessionLost
			onSessionRecovery
			getAllInfos
			add S2sDataEncoder/S2sDataDecoder
1.1.0                Add API to config console port 
1.2.0                Add API readIpList and WriteIpList
1.3.0                define  __STDC_FORMAT_MACROS
1.4.0                Add API delMine
2.0.0                re-design the APIs, to make it more simple
2.1.0:  		add configRecoverTime api
2.2.0: 		��װbson�⣬��ֹ��ͻ
2.2.1:      �ڲ�log, �����Ż���
2.3.0: 		a.�޸�����ʱm_condition��sql�﷨�����bug b.�������Դ��� RECONN_THRESHOLD_SC��Ϊ2 c.�޸�mini version�Ƚϵ�bug
3.0.0:		�����½ӿ�: a.�Ƿ���ȡ��ȫ������(������ʵ����ȡ������������ע����߼�)
						b.�Ƿ���ȡ��ĳ���������������
						c.�������Ľӿ�
3.0.1:		�޸�Ӧ���߳����յ��¼�֪ͨʱisPullAllSub�ӿڿ��ܷ���false��bug.(���߳�ʱ������)
****************************************************/
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <map>
#include <set>
#include <vector>
#include <string>


#define S2S_VERSION "3.0.1"

namespace S2S
{
enum ISPType{
	AUTO_DETECT = 0,
	CTL  = 1,	 //����
	CNC  = 2,	 //��ͨ
	MULTI= 3,  //˫��
	CNII = 4,	 //��ͨ
	EDU  = 8,	 //������
	WBN  = 16, //���ǿ��
	MOB  = 32, //�ƶ�
	BGP  = 64,  //BGP
	HK  = 128, // ���
	BRA = 256, // ����
};
}

enum LostCheckType
{
   NoLostCheck_C =0,
//   SinglePointTcpCheck_C,
//   SinglePointUdpCheck_C,   // not supported now.
   MulPointTcpCheck_C =3,
//   MulPointUdpCheck_C,      //  not supported now
   DaemonCheck =5,              // compatible with old daemon
   ClientServerDoubleCheck_C =6,
   ClientCheckOnly_C = 7,
   ServerCheckOnly_C = MulPointTcpCheck_C,
};


enum MetaType
{
	ANY_TYPE = 0,
//	DAEMON_TYPE = 2,		  	// �Ӿ�daemon������������
//  1~127: reserved by s2s

	// ���¿�����������data�ı����Э��
	TEXTPLAIN = 128, 
	S2SDECODER = 129,
	YYPROTOCOL = 130,
	TEXTJSON = 131,
	MUSIC_PROC = 4096
};

struct SubFilter // and relation
{
  	std::string interestedName; // ģ��ƥ��"prefix"+"%" , like "serviceapp%"
  	int32_t interestedGroup;   // 0��ʾ���л���; 
  	int32_t s2sType;            // ANY_TYPE��ʾ��ע����type

	SubFilter():interestedGroup(0),s2sType(ANY_TYPE)
	{
	}
};

enum S2sMetaStatus
{
	S2SMETA_OK_C,
	S2SMETA_DIED_C      // be killed; 
};

struct S2sMeta
{
	int64_t serverId;    // server�����Ωһ��ʶid;
	MetaType type;
	std::string name;    // �������ƣ�
	int32_t groupId;    // ����id
	std::string data;   // ������Ϣ
	int64_t timestamp;	 // set by s2s server
	S2sMetaStatus status;

	S2sMeta():
		serverId(-1),
		type(ANY_TYPE),
		groupId(0),
		timestamp(0),
		status(S2SMETA_OK_C)
	{
	}
};

enum S2sSessionStatus
{
	S2S_SESSIONOFF_C = 0,
	S2S_SESSIONON_C,
	S2S_SESSIONBIND_C,
	S2S_DNSERROR_C,
	S2S_AUTHFAILURE_C,
	S2S_ERROR_C
};


class IMetaServer
{
public:
	virtual ~IMetaServer(){}

	/*
	   ��ʼ��
	   ����:
	   myName:  �������֣���ʼ������Ҫ�����룻
	   s2sKey:    �����������ʱ�����ɵ�һ���ַ�����������ticket; 
	   myType:   ע��ʱdata�ֶεı����Э�飻
	   ����: �ɹ�����һ��fd, ʹ������Ҫ�������fd�Ķ��¼����Ի�ȡMetaServer��״̬���ºͶ��ĸ��£� ʧ�ܷ���-1; 
	*/
	virtual int initialize(const std::string& myName,const std::string& s2sKey, MetaType myType) = 0;

	/*
	  �����˶���:
	  ����:
	  filters: ��������, SubFilter����or�Ĺ�ϵ��SubFilter����ĳ�Ա��and�Ĺ�ϵ��
	  ����: �ɹ�0, ʧ��-1; 
	*/
	virtual int subscribe(const std::vector<SubFilter>& filters) = 0;

	/*
	  �������Ľӿ�(������ֱ����ȡ���Ľڵ���Ϣ, Ԥ���ӿ�, 3.0.0�汾δʵ��)
	  ����:
	  filters: ��������, SubFilter����or�Ĺ�ϵ��SubFilter����ĳ�Ա��and�Ĺ�ϵ��
	  metas: ���Ľڵ���Ϣ
	  ����: �ɹ�0, ʧ��-1;
	*/
	virtual int syncSubscribe(const std::vector<SubFilter>& filters, std::vector<S2sMeta> &metas) = 0;

	/*
	��ȡMetaServer��״̬���»��߶��ĸ��£�
	����:
	metas:   ������������ض��ĸ��£�
	����:  ����MetaServer�ĵ�ǰ״̬��
	 */
	virtual S2sSessionStatus pollNotify(std::vector<S2sMeta> &metas) = 0;

	/*
	��s2s�����ע�������Ϣ��
	����:
	binData:   ������Ϣ���������S2sDataEncoder/S2sDataDecoder���б����.
	����:  �ɹ�����0,  ʧ�ܷ���-1; 
	*/
	virtual int setMine(const std::string& binData) = 0;

	/*
	��s2s�����ע���Լ���
	����:  �ɹ�����0,  ʧ�ܷ���-1; 
	*/
	virtual int delMine()=0;

	/*
	  ��ȡ�Լ���meta, ��Ҫ��ȡserverId, ͨ������mine������أ�
	  ����: �ɹ�����0,  ʧ�ܷ���-1; 
	  */
	virtual int getMine(S2sMeta & mine) = 0;

	/*
	�Ƿ���ȡ�����ж�������
	ע: 1. һ����ȡ����������, �ýӿ���Զ����true;
		2. ���Ƕ�ζ���, ֻҪ��һ�ζ��Ľ��δ����, �򷵻�false
		3. ���Ľ��Ϊ�գ�Ҳ����true;
	*/
	virtual bool isPullAllSub() = 0;

	/*
	�Ƿ���ȡ��������=name����������
	ע: 1. ��֧��ģʽƥ��
		2. ���ķ�����Ϊ��ʱ������false
	*/
	virtual bool isSubscribePulled(const std::string& name) = 0;

	/*
	���������ӿڣ�ֻ�ڵ���initializeǰ���ò���Ч���ɹ�����0��ʧ�ܷ���-1;
	*/

	/*����lostCheckģʽ,����Ļ���Ĭ��ΪNoLostCheck_C*/
	virtual int setLostCheckType(LostCheckType checkType) = 0;

	/*���û���id, ����Ļ���Ĭ�ϴӻ�����hostinfo.ini����� ������αװgroupId*/
	virtual int setGroupId(uint32_t groupId) = 0;

	/*��������Ŀ�꣬Ĭ�������ĵ㣬һЩС�ڻ���������localDaemon������£��������ó���localDaemon, ������Ϊfalse*/
	virtual int setTarget(bool isToDaemon) = 0;
};



/*namespace mongo
{
class BSONObj;
class BSONObjBuilder;
};
*/
class S2sDataDecoder
{
public:
	S2sDataDecoder(const std::string & data);
	~S2sDataDecoder();

	/*@return value-- the same for all the following methods
	* 0: ok
	* -1: error
	*/
	int readIpList(std::map<S2S::ISPType, uint32_t>& ips);  // <ispType, ipValue>
	int readTcpPort(uint32_t& port);
	int readUdpPort(uint32_t& port);
	int select(const std::string& key, bool& value);
	int select(const std::string& key, int32_t& value);
	int select(const std::string& key, int64_t& value);
	int select(const std::string& key, uint32_t& value);
	int select(const std::string& key, uint64_t& value);	
	int select(const std::string& key, std::string& value);

	int select(const std::string& key, std::map<uint32_t, uint32_t>& pairs);
	int select(const std::string& key, std::vector<int32_t>& value);
	int select(const std::string& key, std::vector<int64_t>& value);
	int select(const std::string& key, std::vector<std::string>& value);
private:
	void* bDecoder;

};

class S2sDataEncoder
{
public:
	S2sDataEncoder();
	~S2sDataEncoder();

	/*@return value--the same for all the following methods
	* 0: ok
	* -1: error
	*/
	int writeIpList(const std::map<S2S::ISPType, uint32_t>&ips);  // <ispType, ipvalue>
	int writeTcpPort(uint32_t port);
	int writeUdpPort(uint32_t port);
	int insert(const std::string& key, bool value);
	int insert(const std::string& key, int32_t value);
	int insert(const std::string& key, int64_t value);
	int insert(const std::string& key, uint32_t value);
	int insert(const std::string& key, uint64_t value);
	int insert(const std::string& key, const std::string& value);
	int insert(const std::string& key, const char* value);

	int insert(const std::string& key, const std::map<uint32_t, uint32_t>& pairs);
	int insert(const std::string& key,const std::vector<int32_t>& value);
	int insert(const std::string& key,const std::vector<int64_t>& value);
	int insert(const std::string& key,const std::vector<std::string>& value);
	const std::string endToString();
private:
	void* builder;
	std::set<std::string> existedKeys;
};




IMetaServer* newMetaServer(const std::string version = S2S_VERSION);


/*
*  please invoke before doing anything.
*/
void configConsolePort(uint32_t port);
void configRecoverTime(uint32_t interval);   // unit: second


#endif

