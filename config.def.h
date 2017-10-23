/* valid curses attributes are listed below they can be ORed
 *
 * A_NORMAL        Normal display (no highlight)
 * A_STANDOUT      Best highlighting mode of the terminal.
 * A_UNDERLINE     Underlining
 * A_REVERSE       Reverse video
 * A_BLINK         Blinking
 * A_DIM           Half bright
 * A_BOLD          Extra bright or bold
 * A_PROTECT       Protected mode
 * A_INVIS         Invisible or blank mode
 */

enum {
	DEFAULT,
	BLUE,
};

static Color colors[] = {
	[DEFAULT] = { .fg = -1,         .bg = -1, .fg256 = -1, .bg256 = -1, },
	[BLUE]    = { .fg = COLOR_BLUE, .bg = -1, .fg256 = 68, .bg256 = -1, },
};

#define COLOR(c)        COLOR_PAIR(colors[c].pair)
/* curses attributes for the currently focused window */
#define SELECTED_ATTR   (COLOR(BLUE) | A_NORMAL)
/* curses attributes for normal (not selected) windows */
#define NORMAL_ATTR     (COLOR(DEFAULT) | A_NORMAL)
/* curses attributes for a window with pending urgent flag */
#define URGENT_ATTR     NORMAL_ATTR
/* curses attributes for the status bar */
#define BAR_ATTR        (COLOR(BLUE) | A_NORMAL)
/* characters for beginning and end of status bar message */
#define BAR_BEGIN       '['
#define BAR_END         ']'
/* status bar (command line option -s) position */
#define BAR_POS         BAR_OFF /* BAR_BOTTOM, BAR_OFF */
/* whether status bar should be hidden if only one client exists */
#define BAR_AUTOHIDE    true
/* master width factor [0.1 .. 0.9] */
#define MFACT { 1, 1, 1, 1, 1, 1 }
/* number of clients in master area */
#define NMASTER 1
/* scroll back buffer size in lines */
#define SCROLL_HISTORY 500
/* printf format string for the tag in the status bar */
#define TAG_SYMBOL   "[%s]"
/* curses attributes for the currently selected tags */
#define TAG_SEL      (COLOR(BLUE) | A_BOLD)
/* curses attributes for not selected tags which contain no windows */
#define TAG_NORMAL   (COLOR(DEFAULT) | A_NORMAL)
/* curses attributes for not selected tags which contain windows */
#define TAG_OCCUPIED (COLOR(BLUE) | A_NORMAL)
/* curses attributes for not selected tags which with urgent windows */
#define TAG_URGENT (COLOR(BLUE) | A_NORMAL | A_BLINK)

const char tags[][8] = { "1", "2", "3", "4", "5" };

#include "tile.c"
//#include "grid.c"
//#include "bstack.c"
//#include "tstack.c"
//#include "vstack.c"
#include "fullscreen.c"

/* by default the first layout entry is used */
static Layout layouts[] = {
    { "[]=", tile },
    { "[ ]", fullscreen },
};

#define MOD  CTRL('g')
#define TAGKEYS(FOCUS,SET,TAG) \
    { { MOD, FOCUS, }, { focustagged, { TAG } } }, \
    { { MOD, SET,   }, { tag,         { TAG } } },

/* you can specifiy at most 3 arguments */
static KeyBinding bindings[] = {
    { { MOD, CTRL('c'), }, { create,         { NULL }                    } },
    { { MOD, 'c',       }, { create,         { NULL, NULL, "$CWD" }      } },
    { { MOD, CTRL('q'), }, { killclient,     { NULL }                    } },
    { { MOD, CTRL('t'), }, { focusdown,      { NULL }                    } },
    { { MOD, CTRL('n'), }, { focusup,        { NULL }                    } },
    { { MOD, CTRL('h'), }, { focusleft,      { NULL }                    } },
    { { MOD, CTRL('s'), }, { focusright,     { NULL }                    } },
    { { MOD, CTRL('b'), }, { setlayout,      { "[]=" }                   } },
    { { MOD, CTRL('z'), }, { setlayout,      { "[ ]" }                   } },
    { { MOD, ' ',       }, { setlayout,      { NULL }                    } },
    { { MOD, ',',       }, { setmfact,       { "-0.1" }                  } },
    { { MOD, '.',       }, { setmfact,       { "+0.1" }                  } },
    { { MOD, '-',       }, { focuslast,      { NULL }                    } },
    { { MOD, 'q', 'q',  }, { quit,           { NULL }                    } },
    { { MOD, CTRL('a')  }, { togglerunall,   { NULL }                    } },
    { { MOD, CTRL('l'), }, { redraw,         { NULL }                    } },
    { { MOD, 'r',       }, { redraw,         { NULL }                    } },
    { { MOD, 'y',       }, { copymode,       { "dvtm-editor" }           } },
    { { MOD, 'l',       }, { copymode,       { "dvtm-pager" }            } },
    { { MOD, '/',       }, { copymode,       { "dvtm-pager", "/" }       } },
    { { MOD, CTRL('p'), }, { paste,          { NULL }                    } },
    { { MOD, KEY_PPAGE, }, { scrollback,     { "-1" }                    } },
    { { MOD, KEY_NPAGE, }, { scrollback,     { "1"  }                    } },
    { { MOD, MOD,       }, { send,           { (const char []){MOD, 0} } } },
    { { KEY_SPREVIOUS,  }, { scrollback,     { "-1" }                    } },
    { { KEY_SNEXT,      }, { scrollback,     { "1"  }                    } },
    TAGKEYS( CTRL('e'), 'e', tags[0] )
    TAGKEYS( CTRL('o'), 'o', tags[1] )
    TAGKEYS( CTRL('u'), 'u', tags[2] )
    TAGKEYS( CTRL('r'), 'r', tags[3] )
};

static const ColorRule colorrules[] = {
    { "", A_NORMAL, &colors[DEFAULT] }, /* default */
};

/* possible values for the mouse buttons are listed below:
 *
 * BUTTON1_PRESSED          mouse button 1 down
 * BUTTON1_RELEASED         mouse button 1 up
 * BUTTON1_CLICKED          mouse button 1 clicked
 * BUTTON1_DOUBLE_CLICKED   mouse button 1 double clicked
 * BUTTON1_TRIPLE_CLICKED   mouse button 1 triple clicked
 * BUTTON2_PRESSED          mouse button 2 down
 * BUTTON2_RELEASED         mouse button 2 up
 * BUTTON2_CLICKED          mouse button 2 clicked
 * BUTTON2_DOUBLE_CLICKED   mouse button 2 double clicked
 * BUTTON2_TRIPLE_CLICKED   mouse button 2 triple clicked
 * BUTTON3_PRESSED          mouse button 3 down
 * BUTTON3_RELEASED         mouse button 3 up
 * BUTTON3_CLICKED          mouse button 3 clicked
 * BUTTON3_DOUBLE_CLICKED   mouse button 3 double clicked
 * BUTTON3_TRIPLE_CLICKED   mouse button 3 triple clicked
 * BUTTON4_PRESSED          mouse button 4 down
 * BUTTON4_RELEASED         mouse button 4 up
 * BUTTON4_CLICKED          mouse button 4 clicked
 * BUTTON4_DOUBLE_CLICKED   mouse button 4 double clicked
 * BUTTON4_TRIPLE_CLICKED   mouse button 4 triple clicked
 * BUTTON_SHIFT             shift was down during button state change
 * BUTTON_CTRL              control was down during button state change
 * BUTTON_ALT               alt was down during button state change
 * ALL_MOUSE_EVENTS         report all button state changes
 * REPORT_MOUSE_POSITION    report mouse movement
 */

#ifdef NCURSES_MOUSE_VERSION
# define CONFIG_MOUSE /* compile in mouse support if we build against ncurses */
#endif

#define ENABLE_MOUSE true /* whether to enable mouse events by default */

#ifdef CONFIG_MOUSE
static Button buttons[] = {
    { BUTTON1_CLICKED,        { mouse_focus,      { NULL  } } },
    { BUTTON1_DOUBLE_CLICKED, { mouse_fullscreen, { "[ ]" } } },
    { BUTTON2_CLICKED,        { mouse_zoom,       { NULL  } } },
    { BUTTON3_CLICKED,        { mouse_minimize,   { NULL  } } },
};
#endif /* CONFIG_MOUSE */

static Cmd commands[] = {
    { "copymode",       { copymode,       { NULL } } },
    { "create",         { create,         { NULL } } },
    { "focusdown",      { focusdown,      { NULL } } },
    { "focuslast",      { focuslast,      { NULL } } },
    { "focusleft",      { focusleft,      { NULL } } },
    { "focusnext",      { focusnext,      { NULL } } },
    { "focusn",         { focusn,         { NULL } } },
    { "focusprev",      { focusprev,      { NULL } } },
    { "focusright",     { focusright,     { NULL } } },
    { "focusup",        { focusup,        { NULL } } },
    { "focustagged",    { focustagged,    { NULL } } },
    { "incnmaster",     { incnmaster,     { NULL } } },
    { "killclient",     { killclient,     { NULL } } },
    { "paste",          { paste,          { NULL } } },
    { "quit",           { quit,           { NULL } } },
    { "redraw",         { redraw,         { NULL } } },
    { "scrollback",     { scrollback,     { NULL } } },
    { "send",           { send,           { NULL } } },
    { "replsend",       { replsend,       { NULL } } },
    { "setlayout",      { setlayout,      { NULL } } },
    { "setmfact",       { setmfact,       { NULL } } },
    { "tag",            { tag,            { NULL } } },
    { "togglebarpos",   { togglebarpos,   { NULL } } },
    { "togglebar",      { togglebar,      { NULL } } },
    { "toggleminimize", { toggleminimize, { NULL } } },
    { "togglemouse",    { togglemouse,    { NULL } } },
    { "togglerunall",   { togglerunall,   { NULL } } },
    { "viewprevtag",    { viewprevtag,    { NULL } } },
    { "view",           { view,           { NULL } } },
    { "zoom",           { zoom,           { NULL } } },
};

/* gets executed when dvtm is started */
static Action actions[] = {
    { create, { NULL } },
};

static char const * const keytable[] = {
    /* add your custom key escape sequences */
};
