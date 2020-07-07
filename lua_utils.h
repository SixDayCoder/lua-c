#pragma once

#ifdef _cplusplus
extern "C" 
{
#endif

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifdef _cplusplus
}
#endif

#define LUA_PARAM_SIZE (16)

#define LUA_FUNCTION_REGISTER(LUA_STATE, _FUNC_) lua_register(LUA_STATE, #_FUNC_, _FUNC_)

#define LUA_GET_PARAM_NUMBER(LUA_STATE, _INDEX_) \
		{\
			assert(lua_gettop(LUA_STATE) >= _INDEX_);\
			assert(lua_isnumber(LUA_STATE, _INDEX_));\
		}\


#define LUA_GET_PARAM_INTEGER(LUA_STATE, _INDEX_) \
		{\
			assert(lua_gettop(LUA_STATE) >= _INDEX_);\
			assert(lua_isinteger(LUA_STATE, _INDEX_));\
		}\

#define LUA_GET_PARAM_STRING(LUA_STATE, _INDEX_) \
		{\
			assert(lua_gettop(LUA_STATE) >= _INDEX_);\
			assert(lua_isstring(LUA_STATE, _INDEX_));\
		}\


//lua param

struct lua_script_param
{
	int rc;   
	int index;
	int params[LUA_PARAM_SIZE];
};
typedef struct lua_script_param lua_script_param;

void lua_script_param_init(lua_script_param* self);

int  lua_script_param_push(lua_script_param* self, int val);

int  lua_script_param_cnt(lua_script_param* self);



//utils func

lua_State*		  lua_init();

int               lua_load_file(lua_State* L, const char* path);

int               lua_add_registery(lua_State* L, void* key, void* user_data);

void*			  lua_get_registery(lua_State* L, void* key);

int				  lua_add_light_user_data(lua_State* L, void* user_data);

void*			  lua_get_light_user_data(lua_State* L, int index);

int				  lua_check_func_exist(lua_State* L, const char* func);

lua_script_param  lua_call_func(lua_State* L, const char* func, lua_script_param* param);

double			  lua_get_global_number(lua_State* L, const char* name);

void			  lua_set_global_number(lua_State* L, const char* name, double val);

long long		  lua_get_global_integer(lua_State* L, const char* name);

void			  lua_set_global_integer(lua_State* L, const char* name, int val);

const char*		  lua_get_global_string(lua_State* L, const char* name);

void			  lua_set_global_string(lua_State* L, const char* name, const char* val);


