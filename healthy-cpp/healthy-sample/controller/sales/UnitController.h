#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2024/01/13 19:38:04

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
#ifndef _UNITCONTROLLER_H_
#define _UNITCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "../../domain/vo/sales/UnitVO.h"
#include "../../domain/query/sales/UnitQuery.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class UnitController : public oatpp::web::server::api::ApiController
{
	// ����������������
	API_ACCESS_DECLARE(UnitController);
public: // ����ӿ�
	// �����ҳ��ѯ��λ�б��ӿ�����
	ENDPOINT_INFO(queryUnit) {
		// ����ӿڱ���
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("unit.query.summary"));
		// ����Ĭ����Ȩ��������ѡ���壬��������ˣ�����ENDPOINT������Ҫ����API_HANDLER_AUTH_PARAME��
		API_DEF_ADD_AUTH();
		// ������Ӧ������ʽ
		API_DEF_ADD_RSP_JSON_WRAPPER(UnitListPageJsonVO);
		// �����ҳ��ѯ��������
		API_DEF_ADD_PAGE_PARAMS();
		// ����������ѯ��������
		API_DEF_ADD_QUERY_PARAMS(UInt32, "type", ZH_WORDS_GETTER("unit.field.type"), 1, true);
		API_DEF_ADD_QUERY_PARAMS(String, "name", ZH_WORDS_GETTER("unit.field.name"), "z company", false);
		API_DEF_ADD_QUERY_PARAMS(String, "uscc", ZH_WORDS_GETTER("unit.field.uscc"), "1wwwwe", false);
		API_DEF_ADD_QUERY_PARAMS(String, "linkMan", ZH_WORDS_GETTER("unit.field.linkMan"), "1wwwwe", false);
		API_DEF_ADD_QUERY_PARAMS(String, "linkPhone", ZH_WORDS_GETTER("unit.field.linkPhone"), "1wwwwe", false);
	}
	// �����ҳ��ѯ��λ�б��ӿ�
	ENDPOINT(API_M_GET, "/sales/query-unit", queryUnit, QUERIES(QueryParams, params), API_HANDLER_AUTH_PARAME) {
		// ������ѯ����ΪQuery����ģ��
		API_HANDLER_QUERY_PARAM(uq, UnitQuery, params);
		// ����ִ�к�����Ӧ���
		API_HANDLER_RESP_VO(execQueryUnit(uq));
	}
private: // ����ӿ�ִ�к���
	UnitListPageJsonVO::Wrapper execQueryUnit(const UnitQuery::Wrapper& query);
};

#include OATPP_CODEGEN_END(ApiController)

#endif // !_UNITCONTROLLER_H_