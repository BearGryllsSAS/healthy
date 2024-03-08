/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2024/01/07 22:54:38

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
#include "TestController.h"

StringJsonVO::Wrapper TestController::execQueryTest(const PageQuery::Wrapper& pq)
{
	StringJsonVO::Wrapper vo = StringJsonVO::createShared();
	if (pq->pageIndex == 1)
	{
		vo->success(String(ZH_WORDS_GETTER("test.field.succ")));
	}
	else
	{
		vo->fail(String(ZH_WORDS_GETTER("test.field.fail")));
	}
	return vo;
}
