/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "push.c"

#define TERM "st"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"xft:Bitstream Vera Sans:size=10",
	"FontAwesome:style=Regular:size=9",
};
static const char dmenufont[]       = "xft:Bitstream Vera Sans:size=10";
static const char normfg[]          = "#bbbbbb";
static const char normbg[]          = "#191b1c";
static const char normbd[]          = "#444444";
static const char selefg[]          = "#eeeeee";
static const char selebg[]          = "#263238";
static const char selebd[]          = "#263238";
static const char *colors[][3]      = {
	/*               fg         bg          border   */
	[SchemeNorm] = { normfg,    normbg,     normbd },
	[SchemeSel]  = { selefg,    selebg,     selebd },
};

/* tagging */
static const char *tags[] = { "︁", "︁", "︁", "︁" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask   isfloating  monitor */
	{ "Blender",        NULL,       NULL,       1 << 3,     0,          -1 },
	{ "Blueman-manager",NULL,       NULL,       0,          1,          -1 },
	{ "Feh",            NULL,       NULL,       0,          1,          -1 },
	{ "Gimp",           NULL,       NULL,       1 << 3,     0,          -1 },
	{ "Inkscape",       NULL,       NULL,       1 << 3,     0,          -1 },
	{ "krita",          NULL,       NULL,       1 << 3,     0,          -1 },
	{ "LBRY",           NULL,       NULL,       1 << 3,     0,          -1 },
	{ "Lxappearance",   NULL,       NULL,       0,          1,          -1 },
	{ "Piper",          NULL,       NULL,       0,          1,          -1 },
	{ "Qalculate-gtk",  NULL,       NULL,       0,          1,          -1 },
	{ "qt5ct",          NULL,       NULL,       0,          1,          -1 },
	{ "Sxiv",           NULL,       NULL,       0,          1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.58; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static int attachbelow       = 0;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },

	{ "[M]",      monocle },
	{ "[D]",      deck },

	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },

	{ "><>",      NULL },    /* no layout function means floating behavior */
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbg, "-nf", normfg, "-sb", selebg, "-sf", selefg, NULL };
static const char *termcmd[]  = { TERM , NULL };

static Key keys[] = {
	/* modifier                 key             function        argument */
	{ MODKEY,                   XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,         XK_Return,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ControlMask,           XK_Return,  spawnbelow,     {.v = termcmd } },
	{ MODKEY|ControlMask|ShiftMask, XK_Return,  spawnbelow,     {.v = dmenucmd } },

	{ MODKEY,                   XK_j,           focusstack,     {.i = +1 } },
	{ MODKEY,                   XK_k,           focusstack,     {.i = -1 } },
	{ MODKEY,                   XK_apostrophe,  focusmaster,    {0} },
	{ MODKEY|ShiftMask,         XK_j,           pushdown,       {0} },
	{ MODKEY|ShiftMask,         XK_k,           pushup,         {0} },
	{ MODKEY,                   XK_semicolon,   zoom,           {0} },
	{ MODKEY,                   XK_Tab,         view,           {0} },
	{ MODKEY|ShiftMask,         XK_period,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,         XK_comma,       incnmaster,     {.i = -1 } },
	{ MODKEY,                   XK_h,           setmfact,       {.f = -0.08} },
	{ MODKEY,                   XK_l,           setmfact,       {.f = +0.08} },
	{ MODKEY|ShiftMask,         XK_h,           setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,         XK_l,           setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,         XK_semicolon,   setcfact,       {.f =  0.00} },
	{ MODKEY,                   XK_comma,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                   XK_period,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                   XK_slash,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                   XK_m,           setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,         XK_m,           setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,       XK_m,           setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,         XK_n,           setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                   XK_n,           togglefloating, {0} },
	{ MODKEY|ShiftMask,         XK_slash,       togglefullscr,  {0} },

	{ MODKEY|ControlMask,           XK_j,       moveresize,     {.v = "0x 40y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_k,       moveresize,     {.v = "0x -40y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_l,       moveresize,     {.v = "40x 0y 0w 0h" } },
	{ MODKEY|ControlMask,           XK_h,       moveresize,     {.v = "-40x 0y 0w 0h" } },
	{ MODKEY|ControlMask|ShiftMask, XK_j,       moveresize,     {.v = "0x 0y 0w 40h" } },
	{ MODKEY|ControlMask|ShiftMask, XK_k,       moveresize,     {.v = "0x 0y 0w -40h" } },
	{ MODKEY|ControlMask|ShiftMask, XK_l,       moveresize,     {.v = "0x 0y 40w 0h" } },
	{ MODKEY|ControlMask|ShiftMask, XK_h,       moveresize,     {.v = "0x 0y -40w 0h" } },
	{ MODKEY|ControlMask,       XK_comma,       moveresizeedge, {.v = "v"} },
	{ MODKEY|ControlMask,       XK_period,      moveresizeedge, {.v = "c"} },
	{ MODKEY|ControlMask,       XK_slash,       moveresizeedge, {.v = "p"} },
	{ MODKEY|ControlMask,       XK_Up,          moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,       XK_Down,        moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,       XK_Left,        moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,       XK_Right,       moveresizeedge, {.v = "r"} },
	{ MODKEY|ShiftMask,         XK_Up,          moveresizeedge, {.v = "T"} },
	{ MODKEY|ShiftMask,         XK_Down,        moveresizeedge, {.v = "B"} },
	{ MODKEY|ShiftMask,         XK_Left,        moveresizeedge, {.v = "L"} },
	{ MODKEY|ShiftMask,         XK_Right,       moveresizeedge, {.v = "R"} },

	{ MODKEY,                   XK_equal,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,         XK_equal,       tag,            {.ui = ~0 } },
	TAGKEYS(                    XK_u,                           0)
	TAGKEYS(                    XK_i,                           1)
	TAGKEYS(                    XK_o,                           2)
	TAGKEYS(                    XK_p,                           3)
	{ MODKEY,                   XK_bracketleft, focusmon,       {.i = -1 } },
	{ MODKEY,                   XK_bracketright,focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,         XK_bracketleft, tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,         XK_bracketright,tagmon,         {.i = +1 } },

	{ MODKEY,                   XK_b,           togglebar,      {0} },
	{ MODKEY,                   XK_y,           killclient,     {0} },
	{ MODKEY|ShiftMask,         XK_q,           quit,           {0} },
	{ MODKEY|ShiftMask,         XK_y,           spawn,          SHCMD("dmenuexit") },

	/* ~/scripts/dwm-executer */
	{ MODKEY,                   XK_q,           spawn,          SHCMD("dwmexecuter -q") },
	{ MODKEY,                   XK_w,           spawn,          SHCMD("dwmexecuter -w") },
	{ MODKEY,                   XK_e,           spawn,          SHCMD("dwmexecuter -e") },
	{ MODKEY,                   XK_r,           spawn,          SHCMD("dwmexecuter -r") },
	{ MODKEY,                   XK_a,           spawn,          SHCMD("dwmexecuter -a") },
	{ MODKEY,                   XK_s,           spawn,          SHCMD("dwmexecuter -s") },
	{ MODKEY,                   XK_d,           spawn,          SHCMD("dwmexecuter -d") },
	{ MODKEY,                   XK_f,           spawn,          SHCMD("dwmexecuter -f") },
	{ MODKEY,                   XK_z,           spawn,          SHCMD("dwmexecuter -z") },
	{ MODKEY,                   XK_x,           spawn,          SHCMD("dwmexecuter -x") },
	{ MODKEY,                   XK_c,           spawn,          SHCMD("dwmexecuter -c") },
	{ MODKEY,                   XK_v,           spawn,          SHCMD("dwmexecuter -v") },
	{ MODKEY,                   XK_1,           spawn,          SHCMD("dwmexecuter -1") },
	{ MODKEY,                   XK_2,           spawn,          SHCMD("dwmexecuter -2") },
	{ MODKEY,                   XK_3,           spawn,          SHCMD("dwmexecuter -3") },
	{ MODKEY,                   XK_4,           spawn,          SHCMD("dwmexecuter -4") },
	{ MODKEY,                   XK_5,           spawn,          SHCMD("dwmexecuter -5") },
	{ MODKEY,                   XK_6,           spawn,          SHCMD("dwmexecuter -6") },
	{ MODKEY,                   XK_7,           spawn,          SHCMD("dwmexecuter -7") },
	{ MODKEY,                   XK_8,           spawn,          SHCMD("dwmexecuter -8") },
	{ MODKEY,                   XK_9,           spawn,          SHCMD("dwmexecuter -9") },
	{ MODKEY,                   XK_0,           spawn,          SHCMD("dwmexecuter -0") },

	{ 0,                        XK_Print,       spawn,          SHCMD("dwmscrot -d") },
	{ MODKEY,                   XK_Print,       spawn,          SHCMD("dwmscrot -w") },
	{ MODKEY|ShiftMask,         XK_x,           spawn,          SHCMD("xkill") },
	{ MODKEY,                   XK_F7,          spawn,          SHCMD(TERM " -e alsamixer") },
	{ MODKEY,                   XK_F8,          spawn,          SHCMD("pkill picom ; sleep 1 ; picom -CGb") },
	{ 0, XK_ISO_Next_Group,     spawn,          SHCMD("pkill -SIGRTMIN+12 dwmblocks") },

	{ 0, XF86XK_Explorer,       spawn,          SHCMD(TERM " -e vifmrun ~ /") },
	{ 0, XF86XK_Calculator,     spawn,          SHCMD("qalculate-gtk") },
	{ 0, XF86XK_HomePage,       spawn,          SHCMD("$BROWSER") },
	{ 0, XF86XK_AudioPlay,      spawn,          SHCMD("playerctl play-pause") },
	{ 0, XF86XK_AudioNext,      spawn,          SHCMD("playerctl next") },
	{ 0, XF86XK_AudioPrev,      spawn,          SHCMD("playerctl previous") },
	{ 0, XF86XK_AudioMute,      spawn,          SHCMD("set-pavolume    ; pkill -SIGRTMIN+10 dwmblocks") },
	{ 0, XF86XK_AudioLowerVolume,   spawn,      SHCMD("set-pavolume -m ; pkill -SIGRTMIN+10 dwmblocks") },
	{ 0, XF86XK_AudioRaiseVolume,   spawn,      SHCMD("set-pavolume -p ; pkill -SIGRTMIN+10 dwmblocks") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[6]} },
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
