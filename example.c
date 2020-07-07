#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "lua_utils.h"

static int hostkey = 0;

struct user_data
{
	int x;
	int y;
	int z;
};
typedef struct user_data user_data;

void test_registery(lua_State* L)
{
	user_data* ud = lua_get_registery(L, &hostkey);
	if (ud)
	{
		printf("user data %d, %d, %d\n", ud->x, ud->y, ud->z);
	}
}

void test_call_lua(lua_State* L)
{
	lua_script_param* req = malloc(sizeof(req));
	lua_script_param_init(req);
	lua_script_param_push(req, 555);
	lua_script_param_push(req, 666);
	lua_script_param_push(req, 777);

	lua_script_param ret = lua_call_func(L, "Hello", req);
	printf("after call lua global function Hello we get : ");
	for (int i = 0; i < lua_script_param_cnt(&ret); ++i)
	{
		printf("%d ", ret.params[i]);
	}
	printf("\n");
}

static int C_Hello(lua_State* L)
{
	LUA_GET_PARAM_NUMBER(L, 1);
	int xx = (int)lua_tonumber(L, 1);

	LUA_GET_PARAM_NUMBER(L, 2);
	double yy = lua_tonumber(L, 2);

	LUA_GET_PARAM_STRING(L, 3);
	const char* zz = lua_tostring(L, 3);

	printf("c   hello first  : %d, %f, %s\n", xx, yy, zz);

	xx += 1;
	yy += 0.1;

	test_registery(L);
	
	lua_pushinteger(L, xx);
	lua_pushnumber(L, yy);
	lua_pushstring(L, zz);

	return 3;
}

int main()
{
	user_data* ud = (user_data*)malloc(sizeof(user_data));
	ud->x = ud->y = ud->z = 12345;

	lua_State* L = lua_init();

	lua_add_registery(L, &hostkey, ud);

	assert(lua_load_file(L, "Main.lua") > 0);

	LUA_FUNCTION_REGISTER(L, C_Hello);

	test_call_lua(L);

	getchar();
	return 0;
}