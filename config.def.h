#include <X11/XF86keysym.h>

/* media key */
static const char *upvol[]   		= { "/usr/bin/pactl", "set-sink-volume", "1", "+5%",     NULL };
static const char *downvol[] 		= { "/usr/bin/pactl", "set-sink-volume", "1", "-5%",     NULL };
static const char *mutevol[] 		= { "/usr/bin/pactl", "set-sink-mute",   "1", "toggle",  NULL };

/* brighness key */
static const char *brightup[]		= { "/usr/bin/xbacklight", "-inc", "5" };
static const char *brightdown[]		= { "/usr/bin/xbacklight", "-dec", "5" };

/* appearance */
static const unsigned int borderpx    	= 0;        /* border pixel of windows */
static const unsigned int gappx     	= 5;
static const unsigned int snap	    	= 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray            = 1;     /* 0 means no systray */
static const int showbar       	    	= 1;        /* 0 means no bar */
static const int topbar        	    	= 1;        /* 0 means bottom bar */
static const int user_bh	    	= 34;
static const char *fonts[]          	= { "SauceCodePro Nerd Font:size=10" };
static const char dmenufont[]       	= "SauceCodePro Nerd Font:size=10";
static const char col_gray1[]       	= "#222222";
static const char col_gray2[]       	= "#444444";
static const char col_gray3[]       	= "#bbbbbb";
static const char col_gray4[]       	= "#eeeeee";
static const char col_cyan[]        	= "#005577";

/* solarized colors http://ethanschoonover.com/solarized */
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#073642";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#657b83";
static const char s_base0[]         = "#839496";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#eee8d5";
static const char s_base3[]         = "#fdf6e3";

static const char *colors[][3]     	= {
       /*               fg         bg         border   */
       { s_base0, s_base03, s_base2 },      /* SchemeNorm dark */
       { s_base0, s_base02, s_base2 },      /* SchemeSel dark */
       { s_base00, s_base3, s_base02 },     /* SchemeNorm light */
       { s_base00, s_base2, s_base02},      /* SchemeSel light */
       { col_gray3, col_gray1, col_gray2 }, /* SchemeNorm orig */
       { col_gray4, col_cyan,  col_cyan  }, /* SchemeSel orig */
 };

/* tagging */
static const char *tags[] = { " ", " ", " ", " ", " ", " " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",  NULL,       NULL,       1<<1,         0,           -1 },
	{ "spotify",  "Spotify",  NULL,	      1<<2,	    0,		 -1 },
	{ "Inkscape", NULL,	  NULL,	      1<<3,	    0,		 -1 },
	{ "Gimp",     NULL,       NULL,       1<<3,         0,           -1 },
	{ "Steam",    NULL,       NULL,       1<<4,         0,           -1 },
	{ "lutris",   NULL,       NULL,       1<<4,         0,           -1 },
	{ "atom",     NULL,       NULL,       1<<5,         0,           -1 },
	{ "jetbrains-studio",     NULL,       NULL,       1<<5,         1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.56; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-sb", "#0d3d4e", "-nb", "#073442", "-sf", "#80959a", "-nf", "#7f949b", NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {

	{ 0,                       	XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       	XF86XK_AudioMute, 	 spawn, {.v = mutevol } },
	{ 0,                       	XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ 0,				XF86XK_MonBrightnessUp,  spawn, {.v = brightup} },
	{ 0,				XF86XK_MonBrightnessDown,spawn, {.v = brightdown} },

	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,		        XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,   		XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,	   togglefullscr,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  setlayout,      {0} },
        { MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
        { MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
        { MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
        { MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
        { MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
        { MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
        { MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
        { MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
        { MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
        { MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
        { MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
        { MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
        { MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
        { MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
        { MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
        { MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_t,      schemeToggle,   {0} },
	{ MODKEY|ShiftMask,             XK_z,      schemeCycle,    {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

