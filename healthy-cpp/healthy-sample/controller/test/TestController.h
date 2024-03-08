#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2024/01/07 22:34:20

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
#ifndef _TESTCONTROLLER_H_
#define _TESTCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/PageQuery.h"
#include "domain/GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class TestController : public oatpp::web::server::api::ApiController
{
	// 定义控制器访问入口
	API_ACCESS_DECLARE(TestController);
public: // 定义接口
	ENDPOINT_INFO(queryTest) {
		// 定义接口描述
		info->summary = ZH_WORDS_GETTER("test.get.summary");
		// 定义接口响应参数类型
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		// 定义分页参数描述
		API_DEF_ADD_PAGE_PARAMS();
	}
	ENDPOINT(API_M_GET, "/test", queryTest, QUERIES(QueryParams, params)) {
		// 将查询参数转换成PageQuery查询模型
		API_HANDLER_QUERY_PARAM(pq, PageQuery, params);
		// 执行业务逻辑处理
		API_HANDLER_RESP_VO(execQueryTest(pq));
	}
private: // 定义接口执行函数
	StringJsonVO::Wrapper execQueryTest(const PageQuery::Wrapper& pq);
};

#include OATPP_CODEGEN_END(ApiController)

#endif // !_TESTCONTROLLER_H_