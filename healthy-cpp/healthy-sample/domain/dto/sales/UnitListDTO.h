#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2024/01/13 19:15:28

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
#ifndef _UNITLISTDTO_H_
#define _UNITLISTDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/**
 * 单位列表传输数据对象实体
 */
class UnitListDTO : public oatpp::DTO
{
	DTO_INIT(UnitListDTO, DTO);
	// 唯一编号
	API_DTO_FIELD_DEFAULT(String, id, ZH_WORDS_GETTER("unit.field.id"));
	// 单位名称
	API_DTO_FIELD_DEFAULT(String, name, ZH_WORDS_GETTER("unit.field.name"));
	// 信用代码
	API_DTO_FIELD_DEFAULT(String, uscc, ZH_WORDS_GETTER("unit.field.uscc"));
	// 体检类型
	API_DTO_FIELD_DEFAULT(UInt32, type, ZH_WORDS_GETTER("unit.field.type"));
	// 所属地区
	API_DTO_FIELD_DEFAULT(String, regionName, ZH_WORDS_GETTER("unit.field.regionName"));
	// 行业类别
	API_DTO_FIELD_DEFAULT(String, industryName, ZH_WORDS_GETTER("unit.field.industryName"));
	// 经济类型
	API_DTO_FIELD_DEFAULT(String, economicTypeName, ZH_WORDS_GETTER("unit.field.economicTypeName"));
	// 企业规模
	API_DTO_FIELD_DEFAULT(String, businessScaleName, ZH_WORDS_GETTER("unit.field.businessScaleName"));
	// 联系人
	API_DTO_FIELD_DEFAULT(String, linkMan, ZH_WORDS_GETTER("unit.field.linkMan"));
	// 联系电话
	API_DTO_FIELD_DEFAULT(String, linkPhone, ZH_WORDS_GETTER("unit.field.linkPhone"));
};

/**
 * 单位列表分页数据实体
 */
class UnitListPageDTO : public PageDTO<UnitListDTO::Wrapper> {
	DTO_INIT(UnitListPageDTO, PageDTO<UnitListDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_UNITLISTDTO_H_