/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "IBM 3270:size=10:bold" };
static const char dmenufont[]       = "IBM 3270:size=10:bold";
/*
 * default : delete later
 * static const char *fonts[]          = { "monospace:size=10" };
 * static const char dmenufont[]       = "monospace:size=10";
*/
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#ffffe6";
static const char col_gray4[]       = "#111111";
static const char col_white[]       = "#ffffe6";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_white,  col_white  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

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
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-b", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_white, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };

/* volume commands */
static const char *volup[] = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ +10% && kill -44 $(pidof dwmblocks)", NULL };
static const char *voldown[] = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ -10% && kill -44 $(pidof dwmblocks)", NULL };
static const char *mute_audio[] = { "sh", "-c", "pactl set-sink-mute @DEFAULT_SINK@ toggle && kill -44 $(pidof dwmblocks)", NULL };
static const char *toggle_mic[] = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };

/* brightness commands */
static const char *brightness_down[] = { "brightnessctl", "set", "5%-", NULL };
static const char *brightness_up[] = { "brightnessctl", "set", "+5%", NULL };

/* lockscreen command */
static const char *lock_screen[] = { "i3lock", "--image", "/home/bk/.config/my_system_things/my_system_photos/Wallpaper/ThinkPad_Theme/ThinkPad_x220.png", "--tiling", "--show-failed-attempts", "--ignore-empty-password", NULL };

/* multi monitor */
static const char *connect_second_screen[] = { "/home/bk/.config/my_system_things/my_system_scripts/bk-change_monitors", NULL };

/* screenshot */
// scrot --select --freeze --format png /home/bk/Photos/Screenshots/screenshot
static const char *make_screenshot[] = { "flameshot", "gui", NULL };

/* shutdown script */
static const char *logout_cmd[] = { "/home/bk/.config/my_system_things/my_system_scripts/bk-logout", NULL };

/* change layout script */
static const char *change_layout[] = { "/home/bk/.config/my_system_things/my_system_scripts/bk-change_layout", NULL };

static const Key keys[] = {
	/* modifier                     key         function        argument */
	{ MODKEY,                       XK_p,       spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,       togglebar,      {0} },
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,     view,           {0} },
	{ MODKEY,                       XK_q,       killclient,     {0} },
	{ MODKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
	{ MODKEY,                       XK_0,       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,   setgaps,        {.i = -1 } },
    { MODKEY,                       XK_equal,   setgaps,        {.i = +1 } },
    { MODKEY|ShiftMask,             XK_equal,   setgaps,        {.i = 0  } },
    { 0,                            0x1008ff02, spawn,          {.v = brightness_up } },     // brightness up
    { 0,                            0x1008ff03, spawn,          {.v = brightness_down } },   // brightness down
    { 0,                            0x1008ff13, spawn,          {.v = volup } },        // volume up
    { 0,                            0x1008ff11, spawn,          {.v = voldown } },      // volume down
    { 0,                            0x1008ff12, spawn,          {.v = mute_audio } },   // mute button
    { 0,                            0x1008ffb2, spawn,          {.v = toggle_mic } },   // mute source
    { 0,                            0x1008ff2a, spawn,          {.v = logout_cmd } },   // power off button
    { 0,                            0x1008ff2d, spawn,          {.v = lock_screen } },  // screen lock
    { 0,                            0x1008ff59, spawn,          {.v = connect_second_screen } },    // second screen
    { Mod1Mask,                     XK_Shift_L, spawn,          {.v = change_layout} }, // Alt + Shift, Still makin changes possible
    { MODKEY|ShiftMask,             XK_s,       spawn,          {.v = make_screenshot} }, // screenshot
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
	{ MODKEY|ShiftMask,             XK_q,       quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

