/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/24 23:02:34

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "StringUtil.h"
#include "ServerInfo.h"
#include "HttpServer.h"
#include "controller/Router.h"
#include "controller/OtherComponent.hpp"
#include "DbInit.h"
#ifdef HTTP_SERVER_DEMO
#include "uselib/jwt/TestToken.h"
#endif

#ifdef LINUX
#include "NacosClient.h"
#endif
#include "ServerInfo.h"
#include "NacosClient.h"
#include "FastDfsClient.h"
#include "ExcelComponent.h"
#include "uselib/rocketmq/TestRocket.h"

#include "MongoClient.h"
#include <iostream>
#include <bsoncxx/json.hpp>

#include "RedisClient.h"
#include "uselib/sms/TestAliSms.h"
#include "EmailSender.h"
#include "uselib/pdf/TestPdf.h"

// 是否是发布Swagger文档包
#ifndef _RELEASE_DOC_
// 查看Swagger文档的时候不需要连接数据库，解开下面的注释关闭启动连接数据库
//#define _RELEASE_DOC_
#endif

/**
 * 解析启动参数
 * 注意：
 * 参数中数据需要满足一定的格式，如：sp=8090、sn=feign-cpp-sample
 * 前缀与真实值之间使用=分隔
 */
bool getStartArg(int argc, char* argv[]) {
	// 服务器端口
	std::string serverPort = "8090";
	// 数据库连接信息
	std::string dbUsername = "root";
	std::string dbPassword = "123456";
	std::string dbName = "test";
	std::string dbHost = "192.168.220.128";
	int dbPort = 3306;
	int dbMax = 5;
#ifdef LINUX
	// Nacos配置参数
	std::string nacosAddr = "192.168.220.128:8848";
	std::string nacosNs = "4833404f-4b82-462e-889a-3c508160c6b4";
	std::string serviceName = "";
	std::string regIp = "";
#endif

	// 开始解析
	int currIndex = 1;
	bool isSetDb = false;
	while (currIndex < argc)
	{
		// 拆分字符串
		auto args = StringUtil::split(argv[currIndex], "=");
		// 判断参数是否合法
		if (args.size() != 2)
		{
			cout << "arg: " << argv[currIndex] << ", format error." << endl;
			exit(1);
		}

		// 根据参数前缀对不同属性赋值
		std::string prefix = args[0];
		std::string val = args[1];
		if (prefix == "sp") serverPort = val;
		else if (prefix == "du")
		{
			isSetDb = true;
			dbUsername = val;
		}
		else if (prefix == "dp")
		{
			isSetDb = true;
			dbPassword = val;
		}
		else if (prefix == "dn")
		{
			isSetDb = true;
			dbName = val;
		}
		else if (prefix == "dh")
		{
			isSetDb = true;
			dbHost = val;
		}
		else if (prefix == "dP")
		{
			isSetDb = true;
			dbPort = atoi(val.c_str());
		}
		else if (prefix == "dm") dbMax = atoi(val.c_str());
#ifdef LINUX
		else if (prefix == "na") nacosAddr = val;
		else if (prefix == "ns") nacosNs = val;
		else if (prefix == "sn") serviceName = val;
		else if (prefix == "ip") regIp = val;
#endif
		// 更新索引
		currIndex++;
	}

	// 记录服务器配置到内存中方便使用
	ServerInfo::getInstance().setServerPort(serverPort);
	ServerInfo::getInstance().setDbUsername(dbUsername);
	ServerInfo::getInstance().setDbPassword(dbPassword);
	ServerInfo::getInstance().setDbName(dbName);
	ServerInfo::getInstance().setDbHost(dbHost);
	ServerInfo::getInstance().setDbPort(dbPort);
	ServerInfo::getInstance().setDbMax(dbMax);
#ifdef LINUX
	ServerInfo::getInstance().setNacosAddr(nacosAddr);
	ServerInfo::getInstance().setNacosNs(nacosNs);
	ServerInfo::getInstance().setRegIp(regIp);
	ServerInfo::getInstance().setServiceName(serviceName);
#endif
	return isSetDb;
}

void testDfs(string fileName)
{
	// 定义DFS客户端对象和URL前缀
	ZO_CREATE_DFS_CLIENT_URL(client, urlPrefix);
	// 开始上传文件
	std::string fieldName = client.uploadFile(fileName);
	// 构建下载路径
	std::string downloadUrl = urlPrefix + fieldName;
	// 输出下载路径
	std::cout << "download url: " << downloadUrl << std::endl;

// #ifdef LINUX
// 	//定义客户端对象
// 	FastDfsClient client("conf/client.conf");
// #else
// 	//定义客户端对象
// 	FastDfsClient client("192.168.220.128");
// #endif
// 
// 	//测试上传
// 	std::string fieldName = client.uploadFile(fileName);
// 	std::cout << "upload fieldname is : " << fieldName << std::endl;
// 	//测试下载
// 	if (!fieldName.empty())
// 	{
// 		std::string path = "./public/fastdfs";
// 		fileName = client.downloadFile(fieldName, &path);
// 		std::cout << "download savepath is : " << fileName << std::endl;
// 	}
// 	//测试删除文件
// 	if (!fieldName.empty())
// 	{
// 		std::cout << "delete file result is : " << client.deleteFile(fieldName) << std::endl;
// 	}
}

void testExcel()
{
	// 创建测试数据
	std::vector<std::vector<std::string>> data;
	stringstream ss;
	for (int i = 1; i <= 10; i++)
	{
		std::vector<std::string> row;
		for (int j = 1; j <= 5; j++)
		{
			ss.clear();
			ss << "pos(" << i << "," << j << ")";
			row.push_back(ss.str());
			ss.str("");
		}
		data.push_back(row);
	}

	// 定义保存数据位置和页签名称
	// 注意：文件件名称和文件路径不能出现中文
	std::string fileName = "./public/excel/1.xlsx";
	// 注意：因为xlnt不能存储非utf8编码的字符，所以中文字需要从配置文件中获取
	std::string sheetName = ZH_WORDS_GETTER("excel.sheet.s1");

	// 保存到文件
	ExcelComponent excel;
	excel.writeVectorToFile(fileName, sheetName, data);

	// 从文件中读取
	auto readData = excel.readIntoVector(fileName, sheetName);
	for (auto row : readData)
	{
		for (auto cellVal : row)
		{
			cout << cellVal << ",";
		}
		cout << endl;
	}

	// 测试创建第二个页签
	sheetName = ZH_WORDS_GETTER("excel.sheet.s2");
	excel.writeVectorToFile(fileName, sheetName, data);

	// 测试覆盖第一个页签
	sheetName = ZH_WORDS_GETTER("excel.sheet.s1");
	data.insert(data.begin(), {
		ZH_WORDS_GETTER("excel.header.h1") ,
		ZH_WORDS_GETTER("excel.header.h2") ,
		ZH_WORDS_GETTER("excel.header.h3") ,
		ZH_WORDS_GETTER("excel.header.h4") ,
		ZH_WORDS_GETTER("excel.header.h5") ,
		});
	excel.writeVectorToFile(fileName, sheetName, data);
}

void testUseMongo()
{
	//创建连接对象
	ZO_CREATE_MONGO_CLIENT(mc);
	//添加单条数据
	auto docBuilder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc = docBuilder
		<< "name" << "MongoDB"
		<< "type" << "database"
		<< "count" << 1
		<< "versions" << open_array
		<< "v3.2" << "v3.0" << "v2.6"
		<< close_array
		<< "info" << open_document
		<< "x" << 203
		<< "y" << 102
		<< close_document
		<< finalize;
	auto res = mc.addOne("t1", doc.view());
	if (res.type() != bsoncxx::type::k_null)
	{
		std::cout << res.get_oid().value.to_string() << std::endl;
	}

	//添加多条数据
	std::vector<bsoncxx::document::value> documents;
	for (int i = 0; i < 10; i++) {
		documents.push_back(bsoncxx::builder::stream::document{} << "i" << i << finalize);
	}
	int32_t addNum = mc.addMultiple("t2", documents);
	cout << "add data:" << addNum << endl;

	//执行查询调用
	mc.execute("t2",
		[](mongocxx::collection* collection) {
			auto cursor = collection->find({});
			for (auto doc : cursor) {
				cout << bsoncxx::to_json(doc) << endl;
			}
		});
}

void testUseRedis()
{
	// 定义Redis客户端对象
	ZO_CREATE_REDIS_CLIENT(rc);
	// 添加值
	bool res = rc.execute<bool>(
		[](Redis* r) {
			return r->set("01", "star");
		});
	std::cout << res << std::endl;
	// 获取值
	string val = rc.execute<string>(
		[](Redis* r) {
			return r->get("01").value();
		});
	std::cout << val << std::endl;
}

void testMail()
{
	std::string topic = ZH_WORDS_GETTER("mail.topic");
	std::string body1 = ZH_WORDS_GETTER("mail.body1");
	std::string body2 = ZH_WORDS_GETTER("mail.body2");
	// 创建邮件发送对象
	ZO_CREATE_MAIL_SENDER(emailSender);
	emailSender.setCharset("utf8");
	emailSender.addRecvEmailAddr("2069682479@qq.com", "awei");
	emailSender.addCcEmailAddr("2949543550@qq.com", ZH_WORDS_GETTER("mail.revname"));
	emailSender.setEmailContent(topic, body1);
	emailSender.addAttachment("/home/file/1.zip");
	if (emailSender.send())
		std::cout << "mail send ok" << std::endl;
	else
		std::cout << "mail send fail" << std::endl;
}

int main(int argc, char* argv[]) {
#ifdef HTTP_SERVER_DEMO
	// 测试生成 JWT Token
	TestToken::generateToken();
#endif

	// 服务器参数初始化
	bool isSetDb = getStartArg(argc, argv);
	//testDfs("E:\\Images\\20141011112404344.jpg.source.jpg");
	//testExcel();
	//TestRocket tr;
	//tr.testRocket();
	//testUseMongo();
	//testUseRedis();
	//TestAliSms::test();
	//testMail();
	TestPdf::testText();
	TestPdf::testTpl();
#ifdef LINUX
	// 创建Nacos客户端对象
	NacosClient nacosClient(
		ServerInfo::getInstance().getNacosAddr(),
		ServerInfo::getInstance().getNacosNs());
	// 从Nacos配置中心中获取数据库配置
	if (!isSetDb)
	{
		YAML::Node node = nacosClient.getConfig("data-source.yaml");
		YamlHelper yaml;
		std::string dbUrl = yaml.getString(&node, "spring.datasource.url");
		if (dbUrl != "")
		{
			int dbPort = 0;
			std::string dbHost = "";
			std::string dbName = "";
			// 解析数据库连接字符串
			yaml.parseDbConnUrl(dbUrl, &dbHost, &dbPort, &dbName);
			// 获取数据库用户名和密码
			std::string dbUsername = yaml.getString(&node, "spring.datasource.username");
			std::string dbPassword = yaml.getString(&node, "spring.datasource.password");
			// 重新修改缓存中的数据
			ServerInfo::getInstance().setDbUsername(dbUsername);
			ServerInfo::getInstance().setDbPassword(dbPassword);
			ServerInfo::getInstance().setDbName(dbName);
			ServerInfo::getInstance().setDbHost(dbHost);
			ServerInfo::getInstance().setDbPort(dbPort);
		}
	}

	// 注册服务
	if (!ServerInfo::getInstance().getServiceName().empty() && !ServerInfo::getInstance().getRegIp().empty())
	{
		nacosClient.registerInstance(
			ServerInfo::getInstance().getRegIp(),
			atoi(ServerInfo::getInstance().getServerPort().c_str()),
			ServerInfo::getInstance().getServiceName());
	}
#endif

#ifndef _RELEASE_DOC_
	// 初始数据库连接
	bool initConnPool = DbInit::initDbPool(DBConfig(
		ServerInfo::getInstance().getDbUsername(),
		ServerInfo::getInstance().getDbPassword(),
		ServerInfo::getInstance().getDbName(),
		ServerInfo::getInstance().getDbHost(),
		ServerInfo::getInstance().getDbPort(),
		ServerInfo::getInstance().getDbMax()));
	if (!initConnPool) return -1;
#endif

	// 启动HTTP服务器
	HttpServer::startServer(ServerInfo::getInstance().getServerPort(),
		[=](Endpoints* doc, HttpRouter* router) {
			Router(doc, router).initRouter();
		},
		[](std::shared_ptr<AbstractComponentReg>* o) {
			*o = std::make_shared<OtherComponent>();
		});

#ifndef _RELEASE_DOC_
	// 释放数据库连接
	DbInit::releasePool();
#endif

#ifdef LINUX
	// 反注册服务
	if (!ServerInfo::getInstance().getServiceName().empty() && !ServerInfo::getInstance().getRegIp().empty())
	{
		nacosClient.deregisterInstance(
			ServerInfo::getInstance().getRegIp(),
			atoi(ServerInfo::getInstance().getServerPort().c_str()),
			ServerInfo::getInstance().getServiceName());
	}
#endif
	return 0;
}
