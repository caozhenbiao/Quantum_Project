
/*
** $Id: ltable.h,v 2.16.1.2 2013/08/30 15:49:41 roberto Exp $
** Lua tables (hash)
** See Copyright Notice in lua.h
*/

#ifndef lmathlib_h
#define lmathlib_h

#include <stddef.h>

#include "llimits.h"
#include "lua.h"

LUAI_FUNC int math_abs (lua_State *L);
LUAI_FUNC int math_sin (lua_State *L);
LUAI_FUNC int math_sinh (lua_State *L);
LUAI_FUNC int math_cos (lua_State *L);
LUAI_FUNC int math_cosh (lua_State *L);
LUAI_FUNC int math_tan (lua_State *L);
LUAI_FUNC int math_tanh (lua_State *L);
LUAI_FUNC int math_asin (lua_State *L);
LUAI_FUNC int math_acos (lua_State *L);
LUAI_FUNC int math_atan (lua_State *L);
LUAI_FUNC int math_atan2 (lua_State *L);
LUAI_FUNC int math_ceil (lua_State *L);
LUAI_FUNC int math_floor (lua_State *L);
LUAI_FUNC int math_fmod (lua_State *L);
LUAI_FUNC int math_modf (lua_State *L);
LUAI_FUNC int math_sqrt (lua_State *L);
LUAI_FUNC int math_pow (lua_State *L);
LUAI_FUNC int math_log (lua_State *L);
LUAI_FUNC int math_exp (lua_State *L);
LUAI_FUNC int math_deg (lua_State *L);
LUAI_FUNC int math_rad (lua_State *L);
LUAI_FUNC int math_frexp (lua_State *L);
LUAI_FUNC int math_ldexp (lua_State *L);
LUAI_FUNC int math_min (lua_State *L);
LUAI_FUNC int math_max (lua_State *L);
LUAI_FUNC int math_random (lua_State *L);
LUAI_FUNC int math_randomseed (lua_State *L);
LUAI_FUNC int luaopen_math (lua_State *L);
#endif