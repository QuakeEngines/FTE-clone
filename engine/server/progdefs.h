/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/* file generated by qcc, do not modify */

typedef struct globalvars_s
{
	int null;
	union {
		vec3_t vec;
		float f;
		int i;
	} ret;
	union {
		vec3_t vec;
		float f;
		int i;
	} param[8];
} globalvars_t;

#define	NUM_SPAWN_PARMS			64
typedef struct nqglobalvars_s
{
	int	*self;
	int	*other;
	int	*world;
	float	*time;
	float	*frametime;	
	int		*newmis;
	float	*force_retouch;
	string_t	*mapname;
	float	*deathmatch;
	float	*coop;
	float	*teamplay;
	float	*serverflags;
	float	*total_secrets;
	float	*total_monsters;
	float	*found_secrets;
	float	*killed_monsters;
	vec3_t	*v_forward;
	vec3_t	*v_up;
	vec3_t	*v_right;
	float	*trace_allsolid;
	float	*trace_startsolid;
	float	*trace_fraction;
	float	*trace_surfaceflags;
	float	*trace_endcontents;
	vec3_t	*trace_endpos;
	vec3_t	*trace_plane_normal;
	float	*trace_plane_dist;
	int	*trace_ent;
	float	*trace_inopen;
	float	*trace_inwater;
	int	*msg_entity;
	func_t	*main;
	func_t	*StartFrame;
	func_t	*PlayerPreThink;
	func_t	*PlayerPostThink;
	func_t	*ClientKill;
	func_t	*ClientConnect;
	func_t	*PutClientInServer;
	func_t	*ClientDisconnect;
	func_t	*SetNewParms;
	func_t	*SetChangeParms;
	float *cycle_wrapped;
	float *dimension_send;

	float *physics_mode;
	float *clientcommandframe;
	float *input_timelength;
	vec3_t *input_angles;
	vec3_t *input_movevalues;
	float *input_buttons;
	float *spawnparamglobals[NUM_SPAWN_PARMS];
} globalptrs_t;

#define P_VEC(v) (pr_global_struct->v)


/*my hands are tied when it comes to the layout of this structure
On the server side, the structure *must* match original quakeworld, or we break compatibility with mvdsv's qvm api
On the client, it really doesn't matter what order fields are in, qclib will remap.
But fields that are actually useful on both sides need to be in the same locations.
But if we include all, that's a waste for csqc...
But we can overlap useful csqc-only ones with ssqc ones that are not going to be used on the client, so long as the types match.
This list isn't shared with the menu.

so the base fields are a fixed size
and the extension fields are added on the end and can have extra vm-specific stuff added on the end
*/
/*DO NOT ADD TO THIS STRUCTURE (base-qw-compat for q1qvm)*/
#define comqcfields	\
	comfieldfloat(modelindex)\
	comfieldvector(absmin)\
	comfieldvector(absmax)\
	comfieldfloat(ltime)\
	comfieldfloat(lastruntime)	/*type doesn't match the qc, we use a hidden double instead. this is dead.*/ 	\
	comfieldfloat(movetype)\
	comfieldfloat(solid)\
	comfieldvector(origin)\
	comfieldvector(oldorigin)\
	comfieldvector(velocity)\
	comfieldvector(angles)\
	comfieldvector(avelocity)\
	comfieldstring(classname)\
	comfieldstring(model)\
	comfieldfloat(frame)\
	comfieldfloat(skin)\
	comfieldfloat(effects)\
	comfieldvector(mins)\
	comfieldvector(maxs)\
	comfieldvector(size)\
	comfieldfunction(touch, ".void()")\
	comfieldfunction(use, ".void()")\
	comfieldfunction(think, ".void()")\
	comfieldfunction(blocked, ".void()")\
	comfieldfloat(nextthink)\
	comfieldentity(groundentity)\
	comfieldfloat(health)\
	comfieldfloat(frags)\
	comfieldfloat(weapon)\
	comfieldstring(weaponmodel)\
	comfieldfloat(weaponframe)\
	comfieldfloat(currentammo)\
	comfieldfloat(ammo_shells)\
	comfieldfloat(ammo_nails)\
	comfieldfloat(ammo_rockets)\
	comfieldfloat(ammo_cells)\
	comfieldfloat(items)\
	comfieldfloat(takedamage)\
	comfieldentity(chain)\
	comfieldfloat(deadflag)\
	comfieldvector(view_ofs)\
	comfieldfloat(button0)\
	comfieldfloat(button1)	/*dead field in nq mode*/	\
	comfieldfloat(button2)\
	comfieldfloat(impulse)\
	comfieldfloat(fixangle)\
	comfieldvector(v_angle)\
	comfieldstring(netname)\
	comfieldentity(enemy)\
	comfieldfloat(flags)\
	comfieldfloat(colormap)\
	comfieldfloat(team)\
	comfieldfloat(max_health)\
	comfieldfloat(teleport_time)\
	comfieldfloat(armortype)\
	comfieldfloat(armorvalue)\
	comfieldfloat(waterlevel)\
	comfieldfloat(watertype)\
	comfieldfloat(ideal_yaw)\
	comfieldfloat(yaw_speed)\
	comfieldentity(aiment)\
	comfieldentity(goalentity)\
	comfieldfloat(spawnflags)\
	comfieldstring(target)\
	comfieldstring(targetname)\
	comfieldfloat(dmg_take)\
	comfieldfloat(dmg_save)\
	comfieldentity(dmg_inflictor)\
	comfieldentity(owner)\
	comfieldvector(movedir)\
	comfieldstring(message)	/*don't use directly, hexen2 uses floats, so we go via qclib for message*/\
	comfieldfloat(sounds)\
	comfieldstring(noise)\
	comfieldstring(noise1)\
	comfieldstring(noise2)\
	comfieldstring(noise3)
/*DO NOT ADD TO THE ABOVE STRUCTURE*/

#define comextqcfields	\
	comfieldvector(punchangle) /*std in nq*/\
	comfieldfloat(gravity)	/*added in quake 1.09 (for hipnotic)*/\
	comfieldfloat(hull)/*PEXT_HEXEN2*/\
	comfieldentity(movechain)/*hexen2*/\
	comfieldfunction(chainmoved, ".void()")/*hexen2*/\
	comfieldfunction(contentstransition, ".void(float old, float new)")/*ENTITYCONTENTSTRANSITION*/\
	comfieldfloat(dimension_solid)/*EXT_DIMENSION_PHYSICS*/\
	comfieldfloat(dimension_hit)/*EXT_DIMENSION_PHYSICS*/\
	comfieldfloat(hitcontentsmask)\
	comfieldfloat(scale)/*DP_ENT_SCALE*/\
	comfieldfloat(fatness)/*FTE_PEXT_FATNESS*/\
	comfieldfloat(alpha)/*DP_ENT_ALPHA*/\
	comfieldentity(tag_entity)\
	comfieldfloat(skeletonindex)		/*FTE_CSQC_SKELETONOBJECTS*/\
	comfieldvector(colormod)\
	comfieldvector(glowmod)\
	comfieldvector(gravitydir)\
	comfieldfloat(pmove_flags)/*EXT_CSQC_1*/\
	comfieldfloat(friction)/*DP_...PHYSICS*/\
	comfieldfloat(erp)/*DP_...PHYSICS*/\
	comfieldfloat(jointtype)/*DP_...PHYSICS*/\
	comfieldfloat(mass)/*DP_...PHYSICS*/\
	comfieldfloat(bouncefactor)/*DP_...PHYSICS*/\
	comfieldfloat(bouncestop)/*DP_...PHYSICS*/

#define svextqcfields \
	comfieldfloat(maxspeed)/*added in quake 1.09*/\
	comfieldfloat(items2)		/*added in quake 1.09 (for hipnotic)*/\
	comfieldentity(view2)/*FTE_PEXT_VIEW2*/\
	comfieldvector(movement)\
	comfieldfloat(vw_index)\
	comfieldentity(nodrawtoclient)\
	comfieldentity(drawonlytoclient)\
	comfieldentity(viewmodelforclient)/*DP_ENT_VIEWMODEL*/\
	comfieldentity(exteriormodeltoclient)\
	comfieldfloat(button3)	/*DP_INPUTBUTTONS (note in qw, we set 1 to equal 3, to match zquake/fuhquake/mvdsv)*/\
	comfieldfloat(button4)\
	comfieldfloat(button5)\
	comfieldfloat(button6)\
	comfieldfloat(button7)\
	comfieldfloat(button8)\
	comfieldfloat(viewzoom)/*DP_VIEWZOOM*/\
	comfieldfloat(tag_index)\
	comfieldfloat(glow_size)\
	comfieldfloat(glow_color)\
	comfieldfloat(glow_trail)\
	comfieldfloat(traileffectnum)/*DP_ENT_TRAILEFFECTNUM*/\
	comfieldvector(color)/*Hexen2 has a .float color, the warnings should be benign*/ \
	comfieldfloat(light_lev)\
	comfieldfloat(style)\
	comfieldfloat(pflags)\
	comfieldfloat(clientcolors)\
	comfieldfloat(dimension_see)/*EXT_DIMENSION_VISIBLE*/\
	comfieldfloat(dimension_seen)/*EXT_DIMENSION_VISIBLE*/\
	comfieldfloat(dimension_ghost)/*EXT_DIMENSION_GHOST*/\
	comfieldfloat(dimension_ghost_alpha)/*EXT_DIMENSION_GHOST*/\
	comfieldfloat(playerclass)/*hexen2 requirements*/\
	comfieldfloat(drawflags)/*hexen2*/\
	comfieldfloat(hasted)/*hexen2 uses this AS WELL as maxspeed*/\
	comfieldfloat(light_level)/*hexen2's grabbing light level from client*/\
	comfieldfloat(abslight)/*hexen2's force a lightlevel*/\
	comfieldfunction(SendEntity, ".float(entity playerent, float changedflags)")/*EXT_CSQC*/\
	comfieldfloat(SendFlags)/*EXT_CSQC_1 (one of the DP guys came up with it)*/\
	comfieldfloat(Version)/*EXT_CSQC (obsolete)*/\
	comfieldfloat(pvsflags)/*EXT_CSQC_1*/\
	comfieldfloat(modelflags)\
	comfieldfloat(uniquespawnid)/*FTE_ENT_UNIQUESPAWNID*/\
	comfieldfunction(customizeentityforclient, ".float()")

//this is the list for all the csqc fields.
//(the #define is so the list always matches the ones pulled out)
#define csqcextfields	\
	comfieldfloat(entnum)		\
	comfieldfloat(frame2)		/*EXT_CSQC_1*/\
	comfieldfloat(frame1time)	/*EXT_CSQC_1*/\
	comfieldfloat(frame2time)	/*EXT_CSQC_1*/\
	comfieldfloat(lerpfrac)	/*EXT_CSQC_1*/\
	comfieldfloat(renderflags)\
	comfieldfloat(forceshader)/*FTE_CSQC_SHADERS*/\
							\
	comfieldfloat(baseframe)	/*FTE_CSQC_BASEFRAME*/\
	comfieldfloat(baseframe2)	/*FTE_CSQC_BASEFRAME*/\
	comfieldfloat(baseframe1time)	/*FTE_CSQC_BASEFRAME*/\
	comfieldfloat(baseframe2time)	/*FTE_CSQC_BASEFRAME*/\
	comfieldfloat(baselerpfrac)	/*FTE_CSQC_BASEFRAME*/\
	comfieldfloat(basebone)	/*FTE_CSQC_BASEFRAME*/\
							\
  	comfieldfloat(bonecontrol1)	/*FTE_CSQC_HALFLIFE_MODELS*/\
	comfieldfloat(bonecontrol2)	/*FTE_CSQC_HALFLIFE_MODELS*/\
	comfieldfloat(bonecontrol3)	/*FTE_CSQC_HALFLIFE_MODELS*/\
	comfieldfloat(bonecontrol4)	/*FTE_CSQC_HALFLIFE_MODELS*/\
	comfieldfloat(bonecontrol5)	/*FTE_CSQC_HALFLIFE_MODELS*/\
	comfieldfloat(subblendfrac)	/*FTE_CSQC_HALFLIFE_MODELS*/\
	comfieldfloat(basesubblendfrac)	/*FTE_CSQC_HALFLIFE_MODELS+FTE_CSQC_BASEFRAME*/\
							\
	comfieldfloat(drawmask)	/*So that the qc can specify all rockets at once or all bannanas at once*/	\
	comfieldfunction(predraw, ".void()")	/*If present, is called just before it's drawn.*/	\
							\
	comfieldfloat(ideal_pitch)\
	comfieldfloat(pitch_speed)

typedef struct stdentvars_s //standard = standard for qw
{
#define comfieldfloat(sharedname) float sharedname;
#define comfieldvector(sharedname) vec3_t sharedname;
#define comfieldentity(sharedname) int sharedname;
#define comfieldstring(sharedname) string_t sharedname;
#define comfieldfunction(sharedname, typestr) func_t sharedname;
comqcfields
#undef comfieldfloat
#undef comfieldvector
#undef comfieldentity
#undef comfieldstring
#undef comfieldfunction
#ifdef VM_Q1
} stdentvars_t;

typedef struct extentvars_s
{
#endif
#define comfieldfloat(name) float name;
#define comfieldvector(name) vec3_t name;
#define comfieldentity(name) int name;
#define comfieldstring(name) string_t name;
#define comfieldfunction(name, typestr) func_t name;
comextqcfields
svextqcfields
#undef comfieldfloat
#undef comfieldvector
#undef comfieldentity
#undef comfieldstring
#undef comfieldfunction

#ifdef VM_Q1
} extentvars_t;
#else
} stdentvars_t;
#endif

typedef struct {
#define comfieldfloat(sharedname) float sharedname;
#define comfieldvector(sharedname) vec3_t sharedname;
#define comfieldentity(sharedname) int sharedname;
#define comfieldstring(sharedname) string_t sharedname;
#define comfieldfunction(sharedname, typestr) func_t sharedname;
comqcfields
#undef comfieldfloat
#undef comfieldvector
#undef comfieldentity
#undef comfieldstring
#undef comfieldfunction

#ifdef VM_Q1
} comentvars_t;
typedef struct {
#endif

#define comfieldfloat(name) float name;
#define comfieldvector(name) vec3_t name;
#define comfieldentity(name) int name;
#define comfieldstring(name) string_t name;
#define comfieldfunction(name, typestr) func_t name;
comextqcfields
#undef comfieldfloat
#undef comfieldvector
#undef comfieldentity
#undef comfieldstring
#undef comfieldfunction

#ifdef VM_Q1
} comextentvars_t;
#else
} comentvars_t;
#endif



#if defined(CSQC_DAT) || !defined(CLIENTONLY)
	#if defined(ODE_STATIC) || defined(ODE_DYNAMIC)
		#define USEODE 1
	#endif
#endif

#ifdef USEODE
typedef struct {
	// physics parameters
	qboolean ode_physics;
	void *ode_body;
	void *ode_geom;
	void *ode_joint;
	float *ode_vertex3f;
	int *ode_element3i;
	int ode_numvertices;
	int ode_numtriangles;
	vec3_t ode_mins;
	vec3_t ode_maxs;
	vec_t ode_mass;
	vec3_t ode_origin;
	vec3_t ode_velocity;
	vec3_t ode_angles;
	vec3_t ode_avelocity;
	qboolean ode_gravity;
	int ode_modelindex;
	vec_t ode_movelimit; // smallest component of (maxs[]-mins[])
	float ode_offsetmatrix[16];
	float ode_offsetimatrix[16];
	float ode_friction;
	int ode_joint_type;
	int ode_joint_enemy;
	int ode_joint_aiment;
	vec3_t ode_joint_origin; // joint anchor
	vec3_t ode_joint_angles; // joint axis
	vec3_t ode_joint_velocity; // second joint axis
	vec3_t ode_joint_movedir; // parameters
	void *ode_massbuf;
} entityode_t;
/*
typedef struct
{
	void *ode_body;
} skelbodyode_t;
typedef struct
{
	int dummy;
} skeljointode_t;
*/
typedef struct
{
	// for ODE physics engine
	qboolean ode; // if true then ode is activated
	qboolean hasodeents; // if true then we have some ode body somewhere, and we consume more cycles processing full physics, instead of trying to skip as much as we can
	void *ode_world;
	void *ode_space;
	void *ode_contactgroup;
	// number of constraint solver iterations to use (for dWorldStepFast)
	int ode_iterations;
	// actual step (server frametime / ode_iterations)
	vec_t ode_step;
	// max velocity for a 1-unit radius object at current step to prevent
	// missed collisions
	vec_t ode_movelimit;
} worldode_t;
#endif
