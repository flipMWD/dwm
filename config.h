/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#263238";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "U", "I", "O", "P" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.58; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },

	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },

	{ "TTT",      bstack },
	{ "===",      bstackhoriz },

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "urxvt", NULL };

static Key keys[] = {
	/* modifier                 key             function        argument */
	{ MODKEY|ShiftMask,         XK_Return,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                   XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                   XK_b,           togglebar,      {0} },
	{ MODKEY,                   XK_j,           focusstack,     {.i = +1 } },
	{ MODKEY,                   XK_k,           focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,         XK_period,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,         XK_comma,       incnmaster,     {.i = -1 } },
	{ MODKEY,                   XK_h,           setmfact,       {.f = -0.08} },
	{ MODKEY,                   XK_l,           setmfact,       {.f = +0.08} },
	{ MODKEY,                   XK_semicolon,   zoom,           {0} },
	{ MODKEY,                   XK_Tab,         view,           {0} },
	{ MODKEY,                   XK_y,           killclient,     {0} },
	{ MODKEY,                   XK_comma,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                   XK_slash,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                   XK_m,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,         XK_m,           setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                   XK_period,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                   XK_a,           setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,         XK_n,           setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                   XK_space,       setlayout,      {0} },
	{ MODKEY,                   XK_n,           togglefloating, {0} },
	{ MODKEY|ShiftMask,         XK_slash,       togglefullscr,  {0} },
	{ MODKEY,                   XK_0,           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,         XK_0,           tag,            {.ui = ~0 } },
	{ MODKEY,                   XK_bracketleft, focusmon,       {.i = -1 } },
	{ MODKEY,                   XK_bracketright,focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,         XK_bracketleft, tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,         XK_bracketright,tagmon,         {.i = +1 } },
	TAGKEYS(                    XK_u,                           0)
	TAGKEYS(                    XK_i,                           1)
	TAGKEYS(                    XK_o,                           2)
	TAGKEYS(                    XK_p,                           3)
	{ MODKEY|ShiftMask,         XK_q,           quit,           {0} },
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

