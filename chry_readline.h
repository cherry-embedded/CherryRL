/*
 * Copyright (c) 2022, Egahp
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CHRY_READLINE_H
#define CHRY_READLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "chry_readline_keycode.h"

#ifndef CONFIG_READLINE_DEBUG
#define CONFIG_READLINE_DEBUG 0
#endif

#ifndef CONFIG_READLINE_DFTROW
#define CONFIG_READLINE_DFTROW 20
#endif

#ifndef CONFIG_READLINE_DFTCOL
#define CONFIG_READLINE_DFTCOL 80
#endif

#ifndef CONFIG_READLINE_HISTORY
#define CONFIG_READLINE_HISTORY 1
#endif

#ifndef CONFIG_READLINE_COMPLETION
#define CONFIG_READLINE_COMPLETION 1
#endif

#ifndef CONFIG_READLINE_PROMPTEDIT
#define CONFIG_READLINE_PROMPTEDIT 1
#endif

#ifndef CONFIG_READLINE_PROMPTSEG
#define CONFIG_READLINE_PROMPTSEG 7
#endif

#ifndef CONFIG_READLINE_XTERM
#define CONFIG_READLINE_XTERM 0
#endif

#ifndef CONFIG_READLINE_NEWLINE
#define CONFIG_READLINE_NEWLINE "\r\n"
#endif

#ifndef CONFIG_READLINE_SPACE
#define CONFIG_READLINE_SPACE 4
#endif

#ifndef CONFIG_READLINE_CTRLMAP
#define CONFIG_READLINE_CTRLMAP 0
#endif

#ifndef CONFIG_READLINE_ALTMAP
#define CONFIG_READLINE_ALTMAP 0
#endif

#ifndef CONFIG_READLINE_REFRESH_PROMPT
#define CONFIG_READLINE_REFRESH_PROMPT 0
#endif

#ifndef CONFIG_READLINE_NOBLOCK
#define CONFIG_READLINE_NOBLOCK 0
#endif

#if defined(CONFIG_READLINE_XTERM) && defined(CONFIG_READLINE_NOBLOCK)
#if CONFIG_READLINE_XTERM && CONFIG_READLINE_NOBLOCK
#error "CONFIG_READLINE_XTERM cannot be configured at the same time as CONFIG_READLINE_NOBLOCK"
#endif
#endif

#ifndef CONFIG_READLINE_HELP
#define CONFIG_READLINE_HELP                                    \
    "\r\n"                                                      \
    "\t+-------------------------------------+\r\n"             \
    "\t|           \e[1;31mCherry ReadLine \e[m          |\r\n" \
    "\t|                                     |\r\n"             \
    "\t| -> CTRL + ^  return normal screen   |\r\n"             \
    "\t| -> CTRL + -  show this help         |\r\n"             \
    "\t| -> CTRL + C  abort line             |\r\n"             \
    "\t| -> CTRL + K  delete cursor to end   |\r\n"             \
    "\t| -> CTRL + L  clear screen           |\r\n"             \
    "\t| -> CTRL + U  delete whole line      |\r\n"             \
    "\t| -> CTRL + W  delete prev word       |\r\n"             \
    "\t| -> CTRL + Z  abort line             |\r\n"             \
    "\t+-------------------------------------+\r\n"
#endif

enum {
    CHRY_READLINE_SGR_NONE = 0,
    CHRY_READLINE_SGR_BLACK = 1,
    CHRY_READLINE_SGR_RED = 2,
    CHRY_READLINE_SGR_GREEN = 3,
    CHRY_READLINE_SGR_YELLOW = 4,
    CHRY_READLINE_SGR_BLUE = 5,
    CHRY_READLINE_SGR_MAGENTA = 6,
    CHRY_READLINE_SGR_CYAN = 7,
    CHRY_READLINE_SGR_WHITE = 8,
    CHRY_READLINE_SGR_DEFAULT = 10
};

typedef struct
{
    char *prompt; /*!< prompt pointer */
    uint16_t (*sput)(void *, uint16_t);
    uint16_t (*sget)(void *, uint16_t);

    struct
    {
        struct {
            uint16_t size; /*!< linesize         */
            char pbuf[0];  /*!< linebuff pointer */
        } *buff;

        uint16_t pptsize; /*!< prompt size   */
        uint16_t pptoff;  /*!< prompt offset */
        uint16_t pptlen;  /*!< prompt strlen */
        uint16_t lnmax;   /*!< linebuff max  */
        uint16_t curoff;  /*!< cursor offset */
        uint16_t mask;    /*!< line mask     */
    } ln;

    struct
    {
        union {
            uint16_t altnsupt; /*!< is screen not supported but alt buffer */
            struct {
                uint8_t nsupt; /*!< is screen not supported */
                uint8_t alt;   /*!< is screen alt buffer */
            };
        };
        uint16_t row; /*!< terminal row     */
        uint16_t col; /*!< terminal column  */
    } term;

#if defined(CONFIG_READLINE_COMPLETION) && CONFIG_READLINE_COMPLETION
    struct {
        uint16_t (*acb)(char *pre, uint16_t size, const char **plist[]);
    } cplt;
#endif

#if defined(CONFIG_READLINE_HISTORY) && CONFIG_READLINE_HISTORY
    struct {
        char *pbuf;     /*!< histfifo buffer  */
        uint16_t in;    /*!< histfifo in pos  */
        uint16_t out;   /*!< histfifo out pos */
        uint16_t mask;  /*!< histfifo mask    */
        uint16_t size;  /*!< histfifo size    */
        uint16_t cache; /*!< history cache    */
        uint16_t index; /*!< history index    */
    } hist;
#endif

    int (*fcb)(uint8_t exec);
    int (*ucb)(uint8_t exec);

#if defined(CONFIG_READLINE_CTRLMAP) && CONFIG_READLINE_CTRLMAP
    uint8_t ctrlmap[32];
#endif

#if defined(CONFIG_READLINE_ALTMAP) && CONFIG_READLINE_ALTMAP
    uint8_t altmap[26];
#endif

#if defined(CONFIG_READLINE_PROMPTEDIT) && CONFIG_READLINE_PROMPTEDIT
    uint8_t pptseglen[CONFIG_READLINE_PROMPTSEG + 1];
#endif

#if defined(CONFIG_READLINE_NOBLOCK) && CONFIG_READLINE_NOBLOCK
    uint8_t noblock;
#endif

} chry_readline_t;

typedef struct {
    char *prompt;      /*!< prompt buffer pointer */
    uint16_t pptsize;  /*!< prompt buffer size */
    char *history;     /*!< history buffer pointer */
    uint16_t histsize; /*!< history buffer size, must be a power of 2 */
    uint16_t (*sput)(void *, uint16_t);
    uint16_t (*sget)(void *, uint16_t);
} chry_readline_init_t;

typedef struct
{
    union {
        uint16_t raw;
        struct
        {
            uint16_t foreground : 6;
            uint16_t bold       : 1;
            uint16_t underline  : 1;
            uint16_t background : 6;
            uint16_t blink      : 1;
            uint16_t inverse    : 1;
        };
    };
} chry_readline_sgr_t;

extern int chry_readline_init(chry_readline_t *rl, chry_readline_init_t *init);
extern void chry_readline_debug(chry_readline_t *rl);
extern char *chry_readline(chry_readline_t *rl, char *linebuff, uint32_t buffsize, uint32_t *linesize);

extern int chry_readline_edit_refresh(chry_readline_t *rl);
extern int chry_readline_edit_clear(chry_readline_t *rl);
extern int chry_readline_edit_insert(chry_readline_t *rl, char c);
extern int chry_readline_edit_backspace(chry_readline_t *rl);
extern int chry_readline_edit_delete(chry_readline_t *rl);
extern int chry_readline_edit_moveleft(chry_readline_t *rl);
extern int chry_readline_edit_moveright(chry_readline_t *rl);
extern int chry_readline_edit_movehome(chry_readline_t *rl);
extern int chry_readline_edit_moveend(chry_readline_t *rl);
extern int chry_readline_edit_delline(chry_readline_t *rl);
extern int chry_readline_edit_delend(chry_readline_t *rl);
extern int chry_readline_edit_delword(chry_readline_t *rl);

extern int chry_readline_complete(chry_readline_t *rl);

extern void chry_readline_newline(chry_readline_t *rl);
extern void chry_readline_detect(chry_readline_t *rl);
extern void chry_readline_clear(chry_readline_t *rl);
extern void chry_readline_mask(chry_readline_t *rl, uint8_t enable);
extern int chry_readline_altscreen(chry_readline_t *rl, uint8_t enable);

extern void chry_readline_set_completion_cb(chry_readline_t *rl, uint16_t (*acb)(char *pre, uint16_t size, const char **plist[]));
extern void chry_readline_set_function_cb(chry_readline_t *rl, int (*fcb)(uint8_t exec));
extern void chry_readline_set_user_cb(chry_readline_t *rl, int (*ucb)(uint8_t exec));
extern void chry_readline_set_ctrlmap(chry_readline_t *rl, uint8_t mapidx, uint8_t exec);
extern void chry_readline_set_altmap(chry_readline_t *rl, uint8_t mapidx, uint8_t exec);

extern int chry_readline_prompt_edit(chry_readline_t *rl, uint8_t segidx, uint16_t sgrraw, const char *format, ...);
extern void chry_readline_prompt_clear(chry_readline_t *rl);

#ifdef __cplusplus
}
#endif

#endif
