//read menu.h

#include "quakedef.h"
#include "winquake.h"

//these are awkward/strange
qboolean M_Options_AlwaysRun (menucheck_t *option, struct menu_s *menu, chk_set_t set)
{
	if (set == CHK_CHECKED)
		return cl_forwardspeed.value > 200;
	else if (cl_forwardspeed.value > 200)
	{
		Cvar_SetValue (&cl_forwardspeed, 200);
		Cvar_SetValue (&cl_backspeed, 200);
		return false;
	}
	else
	{
		Cvar_SetValue (&cl_forwardspeed, 400);
		Cvar_SetValue (&cl_backspeed, 400);
		return true;
	}
}
qboolean M_Options_InvertMouse (menucheck_t *option, struct menu_s *menu, chk_set_t set)
{
	if (set == CHK_CHECKED)
		return m_pitch.value < 0;
	else
	{
		Cvar_SetValue (&m_pitch, -m_pitch.value);
		return m_pitch.value < 0;
	}
}

//options menu.
void M_Menu_Options_f (void)
{
	int mgt;
	extern cvar_t cl_standardchat;
	extern cvar_t cl_standardmsg, crosshair;
#ifdef _WIN32
	extern qboolean vid_isfullscreen;
#endif
	menu_t *menu;
	int y = 32;

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(0);

	mgt = M_GameType();
	if (mgt == MGT_QUAKE2)
	{	//q2...
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{	//h2
		MC_AddPicture(menu, 16, 0, "gfx/menu/hplaque.lmp");
		MC_AddCenterPicture(menu, 0, "gfx/menu/title3.lmp");
		y+=32;
	}
	else
	{	//q1
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	menu->selecteditem = (union menuoption_s *)
	MC_AddConsoleCommand(menu, 16, y,	"       Customize controls", "menu_keys\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,	"            Go to console", "toggleconsole\nplay misc/menu2.wav\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,	"        Reset to defaults", "exec default.cfg\nplay misc/menu2.wav\n"); y+=8;

	MC_AddSlider(menu, 16, y,			"              Mouse Speed", &sensitivity,		1,		10, 0.5); y+=8;
	MC_AddSlider(menu, 16, y,			"                Crosshair", &crosshair,		0,		22, 1); y+=8;

	MC_AddCheckBox(menu, 16, y,			"               Always Run", NULL,0)->func = M_Options_AlwaysRun; y+=8;
	MC_AddCheckBox(menu, 16, y,			"             Invert Mouse", NULL,0)->func = M_Options_InvertMouse; y+=8;
	MC_AddCheckBox(menu, 16, y,			"               Lookspring", &lookspring,0); y+=8;
	MC_AddCheckBox(menu, 16, y,			"               Lookstrafe", &lookstrafe,0); y+=8;
	MC_AddCheckBox(menu, 16, y,			"       Use old status bar", &cl_sbar,0); y+=8;
	MC_AddCheckBox(menu, 16, y,			"         HUD on left side", &cl_hudswap,0); y+=8;
	MC_AddCheckBox(menu, 16, y,			"       Old-style chatting", &cl_standardchat,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"       Old-style messages", &cl_standardmsg,0);y+=8;
	y+=4;MC_AddEditCvar(menu, 16, y,	"              Imitate FPS", "cl_netfps"); y+=8+4;

	MC_AddConsoleCommand(menu, 16, y,	"            Video Options", "menu_video\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,   "Shadow & Lighting Options", "menu_shadow_lighting\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,   "     3D Rendering Options", "menu_3d\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,   "          Texture Options", "menu_textures\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,   "         Particle Options", "menu_particles\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,	"              FPS Options", "menu_fps\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,	"            Audio Options", "menu_audio\n"); y+=8;
	MC_AddConsoleCommand(menu, 16, y,	"                Downloads", "menu_download\n"); y+=8;

#ifdef _WIN32
	if (!vid_isfullscreen)
#endif
	{
		MC_AddCheckBox(menu, 16, y,	"             Use Mouse", &_windowed_mouse,0); y+=8;
	}

	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 215, 32, NULL, false);
}

#ifndef __CYGWIN__
typedef struct {
	int cursorpos;
	menuoption_t *cursoritem;

	menutext_t *speaker[6];
	menutext_t *testsoundsource;

	soundcardinfo_t *card;
} audiomenuinfo_t;

qboolean M_Audio_Key (int key, struct menu_s *menu)
{
	int i, x, y;
	audiomenuinfo_t *info = menu->data;
	soundcardinfo_t *sc;
	for (sc = sndcardinfo; sc; sc = sc->next)
	{
		if (sc == info->card)
			break;
	}
	if (!sc)
	{
		M_RemoveMenu(menu);
		return true;
	}


	if (key == K_DOWNARROW)
	{
		info->testsoundsource->common.posy+=10;
	}
	if (key == K_UPARROW)
	{
		info->testsoundsource->common.posy-=10;
	}
	if (key == K_RIGHTARROW)
	{
		info->testsoundsource->common.posx+=10;
	}
	if (key == K_LEFTARROW)
	{
		info->testsoundsource->common.posx-=10;
	}
	if (key >= '0' && key <= '5')
	{
		i = key - '0';
		x = info->testsoundsource->common.posx - 320/2;
		y = info->testsoundsource->common.posy - 200/2;
		sc->yaw[i] = (-atan2 (y,x)*180/M_PI) - 90;

		sc->dist[i] = 50/sqrt(x*x+y*y);
	}

	menu->selecteditem = NULL;

	return false;
}

void M_Audio_StartSound (struct menu_s *menu)
{
	int i;
	vec3_t org;
	audiomenuinfo_t *info = menu->data;
	soundcardinfo_t *sc;
	vec3_t mat[4];

	static float lasttime;

	for (sc = sndcardinfo; sc; sc = sc->next)
	{
		if (sc == info->card)
			break;
	}
	if (!sc)
	{
		M_RemoveMenu(menu);
		return;
	}

	for (i = 0; i < sc->sn.numchannels; i++)
	{
		info->speaker[i]->common.posx = 320/2 - sin(sc->yaw[i]*M_PI/180) * 50/sc->dist[i];
		info->speaker[i]->common.posy = 200/2 - cos(sc->yaw[i]*M_PI/180) * 50/sc->dist[i];
	}
	for (; i < 6; i++)
		info->speaker[i]->common.posy = -100;

	if (lasttime+0.5 < Sys_DoubleTime())
	{
		S_GetListenerInfo(mat[0], mat[1], mat[2], mat[3]);

		lasttime = Sys_DoubleTime();
		org[0] = mat[0][0] + 2*(mat[1][0]*(info->testsoundsource->common.posx-320/2) + mat[1][0]*(info->testsoundsource->common.posy-200/2));
		org[1] = mat[0][1] + 2*(mat[1][1]*(info->testsoundsource->common.posx-320/2) + mat[1][1]*(info->testsoundsource->common.posy-200/2));
		org[2] = mat[0][2] + 2*(mat[1][2]*(info->testsoundsource->common.posx-320/2) + mat[1][2]*(info->testsoundsource->common.posy-200/2));
		S_StartSound(-2, 0, S_PrecacheSound("player/pain3.wav"), org, 1, 4);
	}
}

void M_Menu_Audio_Speakers_f (void)
{
	int i;
	audiomenuinfo_t *info;
	menu_t *menu;

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(sizeof(audiomenuinfo_t));
	info = menu->data;
	menu->key = M_Audio_Key;
	menu->event = M_Audio_StartSound;

	for (i = 0; i < 6; i++)
		info->speaker[i] = MC_AddBufferedText(menu, 0, 0, va("%i", i), false, true);

	info->testsoundsource = MC_AddBufferedText(menu, 0, 0, "X", false, true);

	info->card = sndcardinfo;

	menu->selecteditem = NULL;
}

menucombo_t *MC_AddCvarCombo(menu_t *menu, int x, int y, const char *caption, cvar_t *cvar, const char **ops, const char **values);
void M_Menu_Audio_f (void)
{
	int y = 32;
	menu_t *menu;
	int mgt;
	extern cvar_t nosound, precache, snd_leftisright, snd_khz, snd_eax, snd_speakers, ambient_level, bgmvolume, snd_playersoundvolume, ambient_fade, cl_staticsounds, snd_inactive, _snd_mixahead, snd_usemultipledevices, snd_noextraupdate, snd_show, bgmbuffer;

	static const char *soundqualityoptions[] = {
		"11025 Hz",
		"22050 Hz",
		"44100 Hz",
		NULL
	};

	static const char *soundqualityvalues[] = {
		"11.025",
		"22.050",
		"44.100",
		NULL
	};

	static const char *speakeroptions[] = {
		"Mono",
		"Stereo",
		"Quad",
		"5.1 surround",
		NULL
	};

	static const char *speakervalues[] = {
		"1",
		"2",
		"4",
		"6",
		NULL
	};

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(0);

	mgt = M_GameType();

	if (mgt == MGT_QUAKE2)
	{
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{
	}
	else
	{
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	menu->selecteditem = (union menuoption_s *)

	MC_AddSlider(menu, 16, y,			"          CD Music Volume", &bgmvolume,		0,		1, 0.1);y+=8;
	MC_AddSlider(menu, 16, y,			"          CD Music Buffer", &bgmbuffer,		0,		10240, 1024);y+=8;
	MC_AddSlider(menu, 16, y,			"             Sound Volume", &volume,			0,		1, 0.1);y+=8;
	MC_AddSlider(menu, 16, y,			"      Player Sound Volume", &snd_playersoundvolume,0,1,0.1);y+=8;
	MC_AddSlider(menu, 16, y,			"           Ambient Volume", &ambient_level,	0,		1, 0.1);y+=8;
	MC_AddSlider(menu, 16, y,			"             Ambient Fade", &ambient_fade,	0,		1000, 1);y+=8;
	MC_AddCheckBox(menu, 16, y,			"                 No Sound", &nosound,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"            Static Sounds", &cl_staticsounds,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"                 Precache", &precache,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			" Low Quality Sound (8bit)", &loadas8bit,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"               Flip Sound", &snd_leftisright,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"Play Sound While Inactive", &snd_inactive,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"       Experimental EAX 2", &snd_eax,0);y+=8;
	MC_AddCvarCombo(menu, 16, y,		"            Speaker Setup", &snd_speakers, speakeroptions, speakervalues);y+=8;
	MC_AddCvarCombo(menu, 16, y,		"              Sound Speed", &snd_khz, soundqualityoptions, soundqualityvalues);y+=8;
	MC_AddSlider(menu, 16, y,			"           Sound Mixahead", &_snd_mixahead,0,1,0.05);y+=8;
	MC_AddCheckBox(menu, 16, y,			"         Multiple Devices", &snd_usemultipledevices,0);y+=8;
	MC_AddCheckBox(menu, 16, y,			"    No Extra Sound Update", &snd_noextraupdate,0);y+=8;
	//MC_AddCombo(menu, 16, y,			"      Show Sounds Playing", &snd_show,0);y+=8;
	y+=8;
	MC_AddConsoleCommand(menu, 16, y,	"        = Restart Sound =", "snd_restart\n");y+=8;

	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 225, 32, NULL, false);
}

#else
void M_Menu_Audio_f (void)
{
	Con_Printf("No sound in cygwin\n");
}
#endif



void M_Menu_Particles_f (void)
{
	int y = 32;
	menu_t *menu;
	int mgt;
	extern cvar_t r_bouncysparks, r_part_rain, gl_part_flame, r_particlesystem, r_grenadetrail, r_rockettrail, r_part_sparks_textured, r_part_sparks_trifan, r_part_rain_quantity, r_part_beams, r_part_beams_textured, r_particle_tracelimit;

	char *psystemopts[] =
	{
		"fixed/classic(faster)",
		"scripted",
		NULL
	};
	char *psystemvals[] =
	{
		"classic",
		"script",
		NULL
	};

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(0);

	mgt = M_GameType();

	if (mgt == MGT_QUAKE2)
	{
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{
	}
	else
	{
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	menu->selecteditem = (union menuoption_s *)

	MC_AddCvarCombo(menu, 16, y,							"        particle system", &r_particlesystem, psystemopts, psystemvals);y+=8;
	//fixme: hide the rest of the options if r_particlesystem==classic
	MC_AddConsoleCommand(menu, 16, y,						"    Choose particle set", "menu_particlesets");y+=8;
//	MC_AddSlider(menu, 16, y,								"        exp spark count", &r_particles_in_explosion, 16, 1024);y+=8;

	MC_AddSlider(menu,	16, y,								"          Grenade Trail", &r_grenadetrail,0,10,1);	y+=8;
	MC_AddSlider(menu,	16, y,								"           Rocket Trail", &r_rockettrail,0,10,1);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"          Bouncy Sparks", &r_bouncysparks,0);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"        Textured Sparks", &r_part_sparks_textured,0);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"          Trifan Sparks", &r_part_sparks_trifan,0);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"                   Rain", &r_part_rain,0);	y+=8;
	MC_AddSlider(menu,	16, y,								"          Rain Quantity", &r_part_rain_quantity,0,10,1);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"            Torch Flame", &gl_part_flame,0);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"         Particle Beams", &r_part_beams,0);	y+=8;
	MC_AddCheckBox(menu,	16, y,							"Textured Particle Beams", &r_part_beams_textured,0);	y+=8;
	MC_AddSlider(menu,	16, y,								"    Particle Tracelimit", &r_particle_tracelimit,0,2000,50);	y+=8;

	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 200, 32, NULL, false);
}

enum {
	PRESET_286,
	PRESET_FAST,
	PRESET_DEFAULT,
	PRESET_NICE,
	PRESET_REALTIME,
	PRESET_MAX
};
typedef struct {
	char *cvarname;
	char *value[PRESET_MAX];
} presetinfo_t;
presetinfo_t preset[] =
{
	//default is a reasonable nice look for single player
	//fast is for competetive deathmatch games (equivelent to the default settings of other quakrworld engines)
	//286 is an attempt to get the very vest fps possible, if you're crazy
	{"r_presetname",		{"286",		"fast",		"default",			"nice",				"realtime"}},
	{"gl_texturemode",		{"nn",		"ln",		"ln",				"ll",				"ll"}},
	{"r_particlesdesc",		{"none",	"highfps",	"spikeset tsshaft",	"spikeset tsshaft",	"spikeset tsshaft"}},
	{"r_particlesystem",	{"none",	"classic",	"script",			"script",			"script"}},
	{"r_stains",			{"0",		"0",		"0.75",				"0.75",				"0.75"}},
	{"r_drawflat",			{"1",		"0",		"0",				"0",				"0"}},
	{"r_nolerp",			{"1",		"0",		"0",				"0",				"0"}},
	{"r_nolightdir",		{"1",		"1",		"0",				"0",				"0"}},
	{"r_dynamic",			{"0",		"0",		"1",				"1",				"1"}},
	{"r_bloom",				{"0",		"0",		"0",				"0",				"1"}},
	{"gl_flashblend",		{"0",		"1",		"0",				"1",				"2"}},
	{"gl_bump",				{"0",		"0",		"0",				"1",				"1"}},
	{"gl_specular",			{"0",		"0",		"0",				"1",				"1"}},
	{"r_loadlit",			{"0",		"1",		"1",				"2",				"2"}},
	{"r_fastsky",			{"1",		"0",		"0",				"-1",				"-1"}},
	{"r_waterlayers",		{"0",		"2",		"",					"4",				"4"}},
	{"r_shadows",			{"0",		"0",		"0",				"1",				"1"}},
	{"r_shadow_realtime_world",{"0",	"0",		"0",				"0",				"1"}},
	{"gl_detail",			{"0",		"0",		"0",				"1",				"1"}},
	{"gl_load24bit",		{"0",		"0",		"1",				"1",				"1"}},
	{"r_replacemodels",		{"",		"",			"md3 md2",			"md3 md2",			"md3 md2"}},
	{"r_waterwarp",			{"0",		"-1",		"1",				"1",				"1"}},
	{"r_lightstylesmooth",	{"0",		"0",		"0",				"1",				"1"}},
	{NULL}
};
static void ApplyPreset (int presetnum)
{
	int i;
	for (i = 1; preset[i].cvarname; i++)
	{
		Cbuf_AddText(preset[i].cvarname, Cmd_ExecLevel);
		Cbuf_AddText(" ", Cmd_ExecLevel);
		Cbuf_AddText(preset[i].value[presetnum], Cmd_ExecLevel);
		Cbuf_AddText("\n", Cmd_ExecLevel);
	}
}

void FPS_Preset_f (void)
{
	char *arg = Cmd_Argv(1);
	int i;
	for (i = 0; i < PRESET_MAX; i++)
	{
		if (!strcmp(preset[0].value[i], arg))
		{
			ApplyPreset(i);
			return;
		}
	}

	Con_Printf("Preset %s not recognised\n", arg);
	Con_Printf("Valid presests:\n");
	for (i = 0; i < PRESET_MAX; i++)
		Con_Printf("%s\n", preset[0].value[i]);
}


void M_Menu_FPS_f (void)
{
	int y = 32;
	menu_t *menu;
	int mgt;
	int i, len;

	extern cvar_t v_contentblend, show_fps, cl_r2g, cl_gibfilter, cl_expsprite, cl_deadbodyfilter;

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(0);

	mgt = M_GameType();

	if (mgt == MGT_QUAKE2)
	{
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{
	}
	else
	{
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	menu->selecteditem = (union menuoption_s *)

	MC_AddConsoleCommand(menu, 16, y,			"  Particle Options", "menu_particles\n"); y+=8;

	for (i = 0; i < PRESET_MAX; i++)
	{
		len = strlen(preset[0].value[i]);
		MC_AddConsoleCommand(menu, 115+8*(9-len), y,		va("(preset) %s", preset[0].value[i]), va("fps_preset %s\n", preset[0].value[i])); y+=8;
	}
	MC_AddCheckBox(menu, 16, y,					"                   Show FPS", &show_fps,0);y+=8;

	MC_AddCheckBox(menu, 16, y,					"              Content blend", &v_contentblend,0);y+=8;
	MC_AddCheckBox(menu,	16, y,				"Convert Rockets to Grenades", &cl_r2g,0);	y+=8;
	MC_AddCheckBox(menu,	16, y,				"                 Gib Filter", &cl_gibfilter,0);	y+=8;
	MC_AddSlider(menu,	16, y,					"           Dead Body Filter", &cl_deadbodyfilter,0,2,1);	y+=8;
	MC_AddCheckBox(menu,	16, y,				"           Explosion Sprite", &cl_expsprite,0);	y+=8;
	y+=4;MC_AddEditCvar(menu, 16, y,			"                     Skybox", "r_skybox");y+=8;y+=4;

	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 250, 32, NULL, false);
}

//copy and pasted from renderer.c
qboolean M_VideoApply2 (union menuoption_s *op,struct menu_s *menu,int key)
{
	if (key != K_ENTER)
		return false;

	// r_shadows too


	Cbuf_AddText("vid_restart\n", RESTRICT_LOCAL);

	M_RemoveMenu(menu);
	Cbuf_AddText("menu_options\n", RESTRICT_LOCAL);
	return true;
}

typedef struct {
	menucombo_t *multisamplingcombo;
} threeDmenuinfo_t;

qboolean M_VideoApply3D (union menuoption_s *op,struct menu_s *menu,int key)
{
	threeDmenuinfo_t *info = menu->data;

	if (key != K_ENTER)
		return false;

	/*switch(info->multisamplingcombo->selectedoption)
	{
	case 0:
		Cbuf_AddText("vid_multisample 0\n", RESTRICT_LOCAL);
		break;
	case 1:
		Cbuf_AddText("vid_multisample 2\n", RESTRICT_LOCAL);
		break;
	case 2:
		Cbuf_AddText("vid_multisample 4\n", RESTRICT_LOCAL);
		break;
	case 3:
		Cbuf_AddText("vid_multisample 6\n", RESTRICT_LOCAL);
		break;
	case 4:
		Cbuf_AddText("vid_multisample 8\n", RESTRICT_LOCAL);
		break;
	}*/

	Cbuf_AddText("vid_restart\n", RESTRICT_LOCAL);

	M_RemoveMenu(menu);
	Cbuf_AddText("menu_3d\n", RESTRICT_LOCAL);
	return true;
}

void M_Menu_3D_f (void)
{
	static const char *msaalevels[] =
		{
			"Off",
			"2x",
			"4x",
			"6x",
			"8x",
			NULL
	};


	int y = 32;
	threeDmenuinfo_t *info;
	menu_t *menu;
	int mgt;
	int i, len;
	int currentmsaalevel;
#ifdef RGLQUAKE
#endif
	extern cvar_t r_novis, gl_dither, cl_item_bobbing, r_waterwarp, r_nolerp, r_fastsky, gl_shadeq1, gl_shadeq3, gl_nocolors, gl_lerpimages, gl_keeptjunctions, gl_lateswap, r_mirroralpha, r_wateralpha, r_drawviewmodel, r_xflip, gl_maxdist, gl_motionblur, gl_motionblurscale, gl_blend2d, gl_blendsprites, r_flashblend, gl_cshiftenabled;

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(sizeof(threeDmenuinfo_t));
	info = menu->data;

	mgt = M_GameType();

	if (mgt == MGT_QUAKE2)
	{
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{
	}
	else
	{
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	/*if (vid_multisample.value == 8)
		currentmsaalevel = 4;
	else if (vid_multisample.value == 6)
		currentmsaalevel = 3;
	else if (vid_multisample.value == 4)
		currentmsaalevel = 2;
	else if (vid_multisample.value == 2)
		currentmsaalevel = 1;
	else if (vid_multisample.value <= 1)
		currentmsaalevel = 0;
	else
		currentmsaalevel = 0;*/

	menu->selecteditem = (union menuoption_s *)

		//info->multisamplingcombo = MC_AddCombo(menu, 16, y,		" Multisample Anti-Aliasing", msaalevels, currentmsaalevel); y+=8;
		MC_AddCheckBox(menu,	16, y,							"             Calculate VIS", &r_novis,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"                Water Warp", &r_waterwarp,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"      Model Interpollation", &r_nolerp,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"                Toggle Sky", &r_fastsky,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"                Q1 Shaders", &gl_shadeq1,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"                Q3 Shaders", &gl_shadeq3,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"Ignore Player Model Colors", &gl_nocolors,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"               Lerp Images", &gl_lerpimages,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"  Toggle Colinear Vertexes", &gl_keeptjunctions,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"       GL Swapbuffer Delay", &gl_lateswap,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"        Mirror Reflections", &r_mirroralpha,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"        Water Transparency", &r_wateralpha,0);	y+=8;
		MC_AddSlider(menu,	16, y,								"   View Model Transparency", &r_drawviewmodel,0,1,0.1);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"      Flip Horizontal View", &r_xflip,0);	y+=8;
		MC_AddSlider(menu,	16, y,								"          Maximum Distance", &gl_maxdist,1,8192,128);	y+=8;
		MC_AddSlider(menu,	16, y,								"               Motion Blur", &gl_motionblur,0,1,0.5);	y+=8;
		MC_AddSlider(menu,	16, y,								"         Motion Blur Scale", &gl_motionblurscale,0,1,0.5);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"               2D Blending", &gl_blend2d,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"           Sprite Blending", &gl_blendsprites,0);	y+=8;
		MC_AddSlider(menu,	16, y,								"            Flash Blending", &r_flashblend,0,2,1);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"             Poly Blending", &gl_cshiftenabled,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"     16bit Color Dithering", &gl_dither,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,							"             Model Bobbing", &cl_item_bobbing,0);	y+=8;
		y+=8;
		MC_AddCommand(menu,	16, y,								"           Apply", M_VideoApply3D);	y+=8;

	//menu->selecteditem = (union menuoption_s *)info->multisamplingcombo;
	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 225, 32, NULL, false);
}

typedef struct {
	menucombo_t *texturefiltercombo;
	menucombo_t *anisotropycombo;
	menucombo_t *maxtexturesizecombo;
} texturemenuinfo_t;

qboolean M_VideoApplyTextures (union menuoption_s *op,struct menu_s *menu,int key)
{
	texturemenuinfo_t *info = menu->data;

	if (key != K_ENTER)
		return false;

	switch(info->texturefiltercombo->selectedoption)
	{
	case 0:
		Cbuf_AddText("gl_texturemode gl_nearest_mipmap_nearest\n", RESTRICT_LOCAL);
		break;
	case 1:
		Cbuf_AddText("gl_texturemode gl_linear_mipmap_nearest\n", RESTRICT_LOCAL);
		break;
	case 2:
		Cbuf_AddText("gl_texturemode gl_linear_mipmap_linear\n", RESTRICT_LOCAL);
		break;
	}

	switch(info->anisotropycombo->selectedoption)
	{
	case 0:
		Cbuf_AddText("gl_texture_anisotropic_filtering 0\n", RESTRICT_LOCAL);
		break;
	case 1:
		Cbuf_AddText("gl_texture_anisotropic_filtering 2\n", RESTRICT_LOCAL);
		break;
	case 2:
		Cbuf_AddText("gl_texture_anisotropic_filtering 4\n", RESTRICT_LOCAL);
		break;
	case 3:
		Cbuf_AddText("gl_texture_anisotropic_filtering 8\n", RESTRICT_LOCAL);
		break;
	case 4:
		Cbuf_AddText("gl_texture_anisotropic_filtering 16\n", RESTRICT_LOCAL);
		break;
	}

	switch(info->maxtexturesizecombo->selectedoption)
	{
	case 0:
		Cbuf_AddText("gl_max_size 1\n", RESTRICT_LOCAL);
		break;
	case 1:
		Cbuf_AddText("gl_max_size 2\n", RESTRICT_LOCAL);
		break;
	case 2:
		Cbuf_AddText("gl_max_size 4\n", RESTRICT_LOCAL);
		break;
	case 3:
		Cbuf_AddText("gl_max_size 8\n", RESTRICT_LOCAL);
		break;
	case 4:
		Cbuf_AddText("gl_max_size 16\n", RESTRICT_LOCAL);
		break;
	case 5:
		Cbuf_AddText("gl_max_size 32\n", RESTRICT_LOCAL);
		break;
	case 6:
		Cbuf_AddText("gl_max_size 64\n", RESTRICT_LOCAL);
		break;
	case 7:
		Cbuf_AddText("gl_max_size 128\n", RESTRICT_LOCAL);
		break;
	case 8:
		Cbuf_AddText("gl_max_size 256\n", RESTRICT_LOCAL);
		break;
	case 9:
		Cbuf_AddText("gl_max_size 512\n", RESTRICT_LOCAL);
		break;
	case 10:
		Cbuf_AddText("gl_max_size 1024\n", RESTRICT_LOCAL);
		break;
	case 11:
		Cbuf_AddText("gl_max_size 2048\n", RESTRICT_LOCAL);
		break;
	case 12:
		Cbuf_AddText("gl_max_size 4096\n", RESTRICT_LOCAL);
		break;
	case 13:
		Cbuf_AddText("gl_max_size 8192\n", RESTRICT_LOCAL);
		break;
	}

	Cbuf_AddText("vid_restart\n", RESTRICT_LOCAL);

	M_RemoveMenu(menu);
	Cbuf_AddText("menu_textures\n", RESTRICT_LOCAL);
	return true;
}

void M_Menu_Textures_f (void)
{
	static const char *texturefilternames[] =
	{
		"Nearest",
		"Bilinear",
		"Trilinear",
		NULL
	};

	static const char *anisotropylevels[] =
	{
		"Off",
		"2x",
		"4x",
		"8x",
		"16x",
		NULL
	};

	static const char *texturesizeoptions[] =
	{
		"1x1",
		"2x2",
		"4x4",
		"8x8",
		"16x16",
		"32x32",
		"64x64",
		"128x128",
		"256x256",
		"512x512",
		"1024x1024",
		"2048x2048",
		"4096x4096",
		"8192x8192",
		NULL
	};

	int y = 32;
	texturemenuinfo_t *info;
	menu_t *menu;
	int mgt;
	int i, len;
	int currenttexturefilter;
	int currentanisotropylevel;
	int currentmaxtexturesize;
	extern cvar_t r_bloom, gl_load24bit, gl_specular, gl_fontinwardstep, gl_smoothfont, r_waterlayers, gl_bump, gl_detail, gl_detailscale, gl_compress, gl_savecompressedtex, gl_ztrick, gl_triplebuffer, gl_picmip, gl_picmip2d, gl_playermip, gl_max_size, r_stains, r_bloodstains, r_stainfadetime, r_stainfadeammount, gl_skyboxdist, r_drawflat, gl_schematics, gl_texturemode, gl_texture_anisotropic_filtering;

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(sizeof(texturemenuinfo_t));
	info = menu->data;

	mgt = M_GameType();

	if (mgt == MGT_QUAKE2)
	{
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{
	}
	else
	{
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	if (!Q_strcasecmp(gl_texturemode.string, "gl_nearest_mipmap_nearest"))
		currenttexturefilter = 0;
	else if (!Q_strcasecmp(gl_texturemode.string, "gl_linear_mipmap_linear"))
		currenttexturefilter = 2;
	else if (!Q_strcasecmp(gl_texturemode.string, "gl_linear_mipmap_nearest"))
		currenttexturefilter = 1;
	else
		currenttexturefilter = 1;

	if (gl_texture_anisotropic_filtering.value == 16)
		currentanisotropylevel = 4;
	else if (gl_texture_anisotropic_filtering.value == 8)
		currentanisotropylevel = 3;
	else if (gl_texture_anisotropic_filtering.value == 4)
		currentanisotropylevel = 2;
	else if (gl_texture_anisotropic_filtering.value == 2)
		currentanisotropylevel = 1;
	else if (gl_texture_anisotropic_filtering.value <= 1)
		currentanisotropylevel = 0;
	else
		currentanisotropylevel = 0;

	if (gl_max_size.value == 8192)
		currentmaxtexturesize = 13;
	else if (gl_max_size.value == 4096)
		currentmaxtexturesize = 12;
	else if (gl_max_size.value == 2048)
		currentmaxtexturesize = 11;
	else if (gl_max_size.value == 1024)
		currentmaxtexturesize = 10;
	else if (gl_max_size.value == 512)
		currentmaxtexturesize = 9;
	else if (gl_max_size.value == 256)
		currentmaxtexturesize = 8;
	else if (gl_max_size.value == 128)
		currentmaxtexturesize = 7;
	else if (gl_max_size.value == 64)
		currentmaxtexturesize = 6;
	else if (gl_max_size.value == 32)
		currentmaxtexturesize = 5;
	else if (gl_max_size.value == 16)
		currentmaxtexturesize = 4;
	else if (gl_max_size.value == 8)
		currentmaxtexturesize = 3;
	else if (gl_max_size.value == 4)
		currentmaxtexturesize = 2;
	else if (gl_max_size.value == 2)
		currentmaxtexturesize = 1;
	else if (gl_max_size.value <= 1)
		currentmaxtexturesize = 0;
	else
		currentmaxtexturesize = 0;


		info->texturefiltercombo = MC_AddCombo(menu, 16, y,	"          Texture Filter", texturefilternames, currenttexturefilter); y+=8;
		info->anisotropycombo =	MC_AddCombo(menu, 16, y,    "        Anisotropy Level", anisotropylevels, currentanisotropylevel); y+=8;
		MC_AddCheckBox(menu,	16, y,						"          32bit Textures", &gl_load24bit,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                   Bloom", &r_bloom,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"             Bumpmapping", &gl_bump,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"     Specular Highlights", &gl_specular,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"          Texture Detail", &gl_detail,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"    Texture Detail Scale", &gl_detailscale,0,10,1);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"     Texture Compression", &gl_compress,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"Save Compressed Textures", &gl_savecompressedtex,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                 Z Trick", &gl_ztrick,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"        Triple Buffering", &gl_triplebuffer,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"       3D Texture Picmip", &gl_picmip,0,16,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"       2D Texture Picmip", &gl_picmip2d,0,16,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"    Model Texture Picmip", &gl_playermip,0,16,1);	y+=8;
		info->maxtexturesizecombo = MC_AddCombo(menu,16, y,	"    Maximum Texture Size", texturesizeoptions, currentmaxtexturesize);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"              Stain Maps", &r_stains,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"            Blood Stains", &r_bloodstains,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"  Stain Fade Time (secs)", &r_stainfadetime,0,30,0.5);	y+=8;
		MC_AddSlider(menu,	16, y,							"      Stain Fade Ammount", &r_stainfadeammount,0,30,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"         Skybox Distance", &gl_skyboxdist,0,10000,100);	y+=8;
		MC_AddSlider(menu,	16, y,							"      Draw Flat Surfaces", &r_drawflat,0,2,1);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"          Map Schematics", &gl_schematics,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"         Font Edge Clamp", &gl_fontinwardstep,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"             Smooth Font", &gl_smoothfont,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"           Smooth Models", &gl_smoothmodels,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"            Water Layers", &r_waterlayers,0,10,1);	y+=8;
		y+=8;
		MC_AddCommand(menu,	16, y,								"           Apply", M_VideoApplyTextures);	y+=8;

	menu->selecteditem = (union menuoption_s *)info->texturefiltercombo;
	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 220, 32, NULL, false);
}

typedef struct {
} shadowlightingmenuinfo_t;

qboolean M_VideoApplyShadowLighting (union menuoption_s *op,struct menu_s *menu,int key)
{
	shadowlightingmenuinfo_t *info = menu->data;

	if (key != K_ENTER)
		return false;

	Cbuf_AddText("vid_restart\n", RESTRICT_LOCAL);

	M_RemoveMenu(menu);
	Cbuf_AddText("menu_shadow_lighting\n", RESTRICT_LOCAL);
	return true;
}

void M_Menu_Shadow_Lighting_f (void)
{
	int y = 32;
	menu_t *menu;
	int mgt;
	int i, len;
#ifdef RGLQUAKE
#endif
	extern cvar_t r_noaliasshadows, r_shadows, r_shadow_realtime_world, r_loadlits, gl_maxshadowlights, r_lightmap_saturation, r_dynamic, r_vertexlight, r_vertexdlights, r_lightstylesmooth, r_lightstylespeed, r_nolightdir, r_shadow_realtime_world_lightmaps, r_shadow_glsl_offsetmapping, r_shadow_glsl_offsetmapping_bias, r_shadow_glsl_offsetmapping_scale, r_shadow_bumpscale_basetexture, r_shadow_bumpscale_bumpmap, r_fb_bmodels, r_fb_models, gl_overbright, r_rocketlight, r_powerupglow, v_powerupshell, r_lightflicker, r_explosionlight;

	key_dest = key_menu;
	m_state = m_complex;

	menu = M_CreateMenu(0);

	mgt = M_GameType();

	if (mgt == MGT_QUAKE2)
	{
		MC_AddCenterPicture(menu, 4, "pics/m_banner_options");
		y += 32;
	}
	else if (mgt == MGT_HEXEN2)
	{
	}
	else
	{
		MC_AddPicture(menu, 16, 4, "gfx/qplaque.lmp");
		MC_AddCenterPicture(menu, 4, "gfx/p_option.lmp");
	}

	menu->selecteditem = (union menuoption_s *)


		MC_AddCheckBox(menu,	16, y,						"              Alias Model Shadows", &r_noaliasshadows,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"                          Shadows", &r_shadows,0,2,1);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"Realtime World Shadows & Lighting", &r_shadow_realtime_world,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"         Realtime World Lightmaps", &r_shadow_realtime_world_lightmaps,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"        GLSL Shadow Offsetmapping", &r_shadow_glsl_offsetmapping,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"   GLSL Shadow Offsetmapping Bias", &r_shadow_glsl_offsetmapping_bias,0,1,0.01);	y+=8;
		MC_AddSlider(menu,	16, y,							"  GLSL Shadow Offsetmapping Scale", &r_shadow_glsl_offsetmapping_scale,0,-1,0.01);	y+=8;
		MC_AddSlider(menu,	16, y,							"     Shadow Bumpscale Basetexture", &r_shadow_bumpscale_basetexture,0,10,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"         Shadow Bumpscale Bumpmap", &r_shadow_bumpscale_bumpmap,0,50,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"                      LIT Loading", &r_loadlits,0,2,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"            Maximum Shadow Lights", &gl_maxshadowlights,0,1000,2);	y+=8;
		//MC_AddSlider(menu,	16, y,						"                   Light Map Mode", &gl_lightmapmode,0,2,1);	y+=8;
		MC_AddSlider(menu,	16, y,							"             Light Map Saturation", &r_lightmap_saturation,0,1,0.1);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                 Dynamic Lighting", &r_dynamic,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                  Vertex Lighting", &r_vertexlight,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"            Dynamic Vertex Lights", &r_vertexdlights,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"             Lightstyle Smoothing", &r_lightstylesmooth,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"       Lightstyle Animation Speed", &r_lightstylespeed,0,50,1);	y+=8;
		MC_AddCheckBox(menu,	16, y,						" Surface Direction Model Lighting", &r_nolightdir,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"            Fullbright BSP Models", &r_fb_bmodels,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"          Fullbright Alias Models", &r_fb_models,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                  Overbright Bits", &gl_overbright,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"          Rocket Dynamic Lighting", &r_rocketlight,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"                     Powerup Glow", &r_powerupglow,0,2,1);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                    Powerup Shell", &v_powerupshell,0);	y+=8;
		MC_AddCheckBox(menu,	16, y,						"                 Light Flickering", &r_lightflicker,0);	y+=8;
		MC_AddSlider(menu,	16, y,							"                  Explosion Light", &r_explosionlight,0,1,0.1);	y+=8;
		y+=8;
		MC_AddCommand(menu,	16, y,							"                            Apply", M_VideoApplyShadowLighting);	y+=8;

	menu->cursoritem = (menuoption_t*)MC_AddWhiteText(menu, 285, 32, NULL, false);
}