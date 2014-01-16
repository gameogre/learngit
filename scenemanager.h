/******************************************************************************
文件名: scenemanager.h
编写者: csdn_viva@foxmail.com ccbase1.0
编写日期: 2013-5-2
功能描述: 场景管理器，提供对场景生命周期进行操作的API
******************************************************************************/
#ifndef __CCBASE_SCENEMANAGER_H__
#define __CCBASE_SCENEMANAGER_H__

///消息体结构描述
typedef struct{
	int		nMsg;	 //消息号
	ARRAY	wParam;	 //数组参数，保存一定数据，能自动释放
	PARAM	lParam;	 //用户指定类型数据，需要手动释放
	CScene* pScene;  //要接受消息的场景
} TMessage;

/******************************************************************************
类    名: CSceneManager
功能描述: 场景管理器，提供对场景生命周期进行操作的API
******************************************************************************/
class CSceneManager : public CCNode
{
public:
	///构造方法，开启帧函数。
	CSceneManager(void);
	///析构，关闭帧函数，释放资源
	virtual ~CSceneManager(void);

public:
	///注册场景实例，通过类名映射
	void registeSceneClass(const char* pClassName, PFNCREATESCENE pFunction);
	///注册对话框实例，通过类名映射
	void registePopupBoxClass(const char* pClassName, PFNCREATEPOPUPBOX pFunction);
	///退出游戏的接口，代替游戏导演
	void exit();
	///帧回调函数
	void visit();

	///增加一个帧回调事件
	void addLooper(CLooper* pLooper);
	///移除一个帧回调事件
	void removeLooper(CLooper* pLooper);
	///移除所有帧事件
	void removeAllLooper();
	///给栈顶的场景发消息，lParam指针要手动释放
	void PostMessageA(int nMsg, ARRAY wParam, PARAM lParam);

public:
	///运行场景，只能调用一次
	void runScene(CCScene *pScene);
	///替换当前栈顶的场景 出栈并压栈
	void replaceScene(CCScene *pScene);
	///增加一个场景到栈顶 压栈
	void pushScene(CCScene *pScene);
	///移除栈顶的场景 出栈
	void popScene();
	///回到栈底的场景，连续出栈
	void popToRootScene();

	///打开对话框，可以同时打开多个
	void openPopupBox(const char* pClassName, PARAM param = NULL);
	///关闭对话框
	void closePopupBox(const char* pClassName);

	///通过场景类名获得场景实例，从缓存池中取出，如果不存在于缓存池，就new
	CScene* getScene(const char* pClassName);
	///通过对话框类名获得对话框实例，从缓存池中取出，如果不存在于缓存池，就new
	CPopupBox* getPopupBox(const char* pClassName);

	///释放所有不在运行场景栈 但是在场景缓存列表里的场景
	void removeAllCachedScene();
	///释放所有不在运行弹出框组里 但是在缓存里的弹出框
	void removeAllCachedPopupBox();

protected:
	///每个场景注册的获得实例的函数表
	map<string, PFNCREATESCENE>	m_mSceneFunMap;
	///缓存的场景
	map<string, CScene*> m_mSceneCacheMap;
	///消息列队 每次PostMessageA后 再一下帧将消息发出
	list<TMessage> m_MessageQueue;

	///每个弹出框注册的获得实例的函数表
	map<string, PFNCREATEPOPUPBOX> m_mPopupBoxFunMap;
	///缓存的弹出框
	map<string, CPopupBox*> m_mPopupBoxCacheMap;
	///运行的弹出框根节点
	CCNode *pRunningBoxNode;

	///帧事件接收列表
	vector<CLooper*> m_vLooperList;

public:
	///单例
	static CSceneManager* sharedSceneManager(); 

};

/******************************************************************************
类    名: CLooper
功能描述: CLooper接口，继承的类拥有处理每帧回调的功能
******************************************************************************/
class CLooper
{
public:
	//每帧回调方法
	virtual void loop() = 0;
};



#endif //__CCBASE_SCENEMANAGER_H__
