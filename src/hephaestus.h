#ifndef HEPHAESTUS_H
#define HEPHAESTUS_H
#include "fz_base.h"

const char* token_kind_string[] = {
  "Token_Unknown",
  "Token_EOF",
  "Token_Space",
  "Token_Tab",
  "Token_Newline",
  "Token_CarriageReturn",
  "Token_Null",
  "Token_Bell",
  "Token_Backspace",
  "Token_VerticalTab",
  "Token_FormFeed",
  "Token_Escape",
  "Token_Delete",
  "Token_String_Literal",
  "Token_Number",
  "Token_Exclamation",
  "Token_Quote",
  "Token_Apostrophe",
  "Token_Hash",
  "Token_Dollar",
  "Token_Percent",
  "Token_Ampersand",
  "Token_ParenOpen",
  "Token_ParenClose",
  "Token_Asterisk",
  "Token_Plus",
  "Token_Comma",
  "Token_Minus",
  "Token_Dot",
  "Token_Slash",
  "Token_Colon",
  "Token_Semicolon",
  "Token_Less",
  "Token_Equals",
  "Token_Greater",
  "Token_Question",
  "Token_At",
  "Token_BracketOpen",
  "Token_BracketClose",
  "Token_Backslash",
  "Token_Caret",
  "Token_Underscore",
  "Token_Backtick",
  "Token_BraceOpen",
  "Token_BraceClose",
  "Token_Pipe",
  "Token_Tilde",
  "Token_Count",
};

typedef enum
{
  Token_Unknown = 0,
  Token_EOF,

  Token_Space,               /* ' ' */
  Token_Tab,                 /* '\t' */
  Token_Newline,             /* '\n' */
  Token_CarriageReturn,      /* '\r' */
  Token_Null,                /* '\0' */
  Token_Bell,                /* '\a' */
  Token_Backspace,           /* '\b' */
  Token_VerticalTab,         /* '\v' */
  Token_FormFeed,            /* '\f' */
  Token_Escape,              /* '\x1B' */
  Token_Delete,              /* 0x7F */

  Token_String_Literal,      /* sequence of [A-Za-z0-9_] */
  Token_Number,              /* sequence of digits, optional '.' */

  Token_Exclamation,         /* ! */
  Token_Quote,               /* " */
  Token_Apostrophe,          /* ' */
  Token_Hash,                /* # */
  Token_Dollar,              /* $ */
  Token_Percent,             /* % */
  Token_Ampersand,           /* & */
  Token_ParenOpen,           /* ( */
  Token_ParenClose,          /* ) */
  Token_Asterisk,            /* * */
  Token_Plus,                /* + */
  Token_Comma,               /* , */
  Token_Minus,               /* - */
  Token_Dot,                 /* . */
  Token_Slash,               /* / */
  Token_Colon,               /* : */
  Token_Semicolon,           /* ; */
  Token_Less,                /* < */
  Token_Equals,              /* = */
  Token_Greater,             /* > */
  Token_Question,            /* ? */
  Token_At,                  /* @ */
  Token_BracketOpen,         /* [ */
  Token_BracketClose,        /* ] */
  Token_Backslash,           /* \ */
  Token_Caret,               /* ^ */
  Token_Underscore,          /* _ */
  Token_Backtick,            /* ` */
  Token_BraceOpen,           /* { */
  Token_BraceClose,          /* } */
  Token_Pipe,                /* | */
  Token_Tilde,               /* ~ */

  Token_Count,
} Token_Kind;

typedef struct Token Token;
struct Token
{
  Token_Kind kind;
  String8 value;
  u32 start_offset;
  u32 end_offset;
  u32 line;
  u32 column;
};

typedef struct Token_Array Token_Array;
struct Token_Array
{
  Token* tokens;
  u64 count;
};
#define TOKEN_ARRAY_SIZE 4096

typedef struct Token_Iterator Token_Iterator;
struct Token_Iterator
{
  Token_Array* array;
  Token* current_token;
  u32 cursor;
};

typedef struct Lexer Lexer;
struct Lexer
{
  Arena* arena;

  File_Data file;
  u8* file_start;
  u8* file_end;
  u8* current_character;

  u32 line;
  u32 column;

  Token current_token;
};

function Token_Array* load_all_tokens(String8 file_path);
function Token        next_token(Lexer* lexer);

function void         parse_tokens(Token_Array* array);
function void         next_non_whitespace_token(Token_Iterator* iterator);
function b32          advance_iterator(Token_Iterator* iterator);
function b32          advance_iterator_to(Token_Iterator* iterator, Token_Kind kind);

#define hph_fatal(str8) emit_fatal(string8_concat(g_log_context.arena, S("[Hephaestus]: "), str8));
#define hph_warn(str8)  emit_warn(string8_concat(g_log_context.arena, S("[Hephaestus]: "), str8));

#endif // HEPHAESTUS_H