#include "lua_utils.h"
#include <assert.h>

static const host_key = 0;

void lua_script_param_init(lua_script_param* self)
{
	assert(self);
	self->rc = -1;
	self->index = 0;
	for (int i = 0; i < LUA_PARAM_SIZE; ++i)
	{
		self->params[i] = -1;
	}
}

int lua_script_param_push(lua_script_param* self, int val)
{
	assert(self);

	if (self->index == LUA_PARAM_SIZE)
	{
		return -1;
	}

	self->params[self->index] = val;
	self->index++;
	return 1;
}

int lua_script_param_cnt(lua_script_param* self)
{
	assert(self);
	return self->index;
}

lua_State* lua_init()
{
	lua_State* L = luaL_newstate();

	assert(L);

	//open std libs
	luaL_openlibs(L);

	return L;
}

int lua_load_file(lua_State* L, const char* path)
{
	assert(L);

	if (!path)
	{
		return -1;
	}

	//load
	int rc = luaL_loadfile(L, path);
	if (rc != LUA_OK)
	{
		return -1;
	}

	//compile lua file
	rc = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (rc != LUA_OK)
	{
		return -1;
	}

	return 1;
}

int lua_add_registery(lua_State* L, void* key, void* user_data)
{
	assert(L);

	//registryindex is pseudo-indices
	//a good way to store global data in lua

	//retrive address of static data to ensure the unique key
	lua_pushlightuserdata(L, key);
	lua_pushlightuserdata(L, user_data);

	//registry[key] = host
	lua_settable(L, LUA_REGISTRYINDEX);

	return 1;
}

void* lua_get_registery(lua_State* L, void* key)
{
	assert(L);

	lua_pushlightuserdata(L, key);
	lua_gettable(L, LUA_REGISTRYINDEX);

	return lua_touserdata(L, -1);
}

int lua_add_light_user_data(lua_State* L, void* user_data)
{
	//light_user_data is under C's control, just like a pointer
	//no metatables 
	//no gc
	assert(L);
	lua_pushlightuserdata(L, user_data);
	return lua_gettop(L);
}

void* lua_get_light_user_data(lua_State* L, int index)
{
	assert(L);
	return lua_touserdata(L, index);
}

int lua_check_func_exist(lua_State* L, const char* func)
{
	if (!L)
	{
		return -1;
	}

	if (!func)
	{
		return -1;
	}

	//load the func add push it to lua stack top
	lua_getglobal(L, func);

	//check if the func is valid or not
	int isnil = lua_isnil(L, -1);

	if (!isnil)
	{
		//remove the function we find to ensure we can use it the next time
		lua_pop(L, 1);
		return 1;
	}

	return -1;
}

lua_script_param lua_call_func(lua_State* L, const char* func, lua_script_param* param)
{
	assert(lua_check_func_exist(L, func));

	int old_stack_top_index = lua_gettop(L);
	lua_getglobal(L, func);

	//prepare function call params
	int param_cnt = 0;
	if (param)
	{
		param_cnt = lua_script_param_cnt(param);
		for (int i = 0; i < lua_script_param_cnt(param); ++i)
		{
			lua_pushnumber(L, param->params[i]);
		}
	}

	//MULTRET-for return multi results
	int rc = lua_pcall(L, param_cnt, LUA_MULTRET, 0);
	assert(rc == LUA_OK);

	lua_script_param ret;
	lua_script_param_init(&ret);

	int new_stack_top_index = lua_gettop(L);
	for (int i = old_stack_top_index + 1; i <= new_stack_top_index; ++i)
	{
		lua_script_param_push(&ret, (int)(lua_tonumber(L, i)));
	}

	//go back to the old stack index
	lua_settop(L, old_stack_top_index);

	return ret;
}

double lua_get_global_number(lua_State* L, const char* name)
{
	assert(L);

	lua_getglobal(L, name);

	int isnil = lua_isnil(L, -1);

	double ret = 0;
	if (!isnil)
	{
		ret = lua_tonumber(L, -1);
	}
	lua_pop(L, 1);

	return ret;
}

void lua_set_global_number(lua_State* L, const char* name, double val)
{
	assert(L);

	lua_pushnumber(L, val);

	//set global variable name with stack top val
	lua_setglobal(L, name);
}

long long lua_get_global_integer(lua_State* L, const char* name)
{
	assert(L);

	lua_getglobal(L, name);

	int isnil = lua_isnil(L, -1);

	long long ret = -1;
	if (!isnil)
	{
		ret = lua_tointeger(L, -1);
	}
	lua_pop(L, 1);

	return ret;
}

void lua_set_global_integer(lua_State* L, const char* name, int val)
{
	assert(L);

	lua_pushinteger(L, val);

	//set global variable name with stack top val
	lua_setglobal(L, name);
}

const char* lua_get_global_string(lua_State* L, const char* name)
{
	assert(L);

	lua_getglobal(L, name);

	int isnil = lua_isnil(L, -1);

	const char* ret = NULL;
	size_t size = 0;
	if (!isnil)
	{
		ret = lua_tolstring(L, -1, &size);
		assert(ret[size] == '\0');
	}
	lua_pop(L, 1);
	return ret;
}

void lua_set_global_string(lua_State* L, const char* name, const char* val)
{
	assert(L);

	lua_pushstring(L, val);

	//set global variable name with stack top val
	lua_setglobal(L, name);
}
