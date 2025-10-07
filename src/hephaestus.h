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

  Token_String_Identifier,   /* sequence of [A-Za-z0-9_] */
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

typedef enum
{
  Generator_Command_Unknown = 0,

  Generator_Command_String,
  Generator_Command_Foreach,

  Generator_Command_Count,
} Generator_Command_Kind;

typedef enum
{
  Template_String_Variable_Unknown = 0,

  Template_String_Variable_Replace, /* Replace this argument with string */
  Template_String_Variable_Time_Now, /* Replace this argument with time_now */

  Template_String_Variable_Count,
} Template_String_Variable_Kind;

typedef enum 
{
  Template_Arg_Method_None = 0,

  Template_Arg_Method_Slice,

  Template_Arg_Method_Count,
} Template_Arg_Method_Kind;

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

typedef struct Template_String8_Arg Template_String8_Arg;
struct Template_String8_Arg
{
  Template_String_Variable_Kind kind;

  String8 arg_name; /* String inside $(...). E.g. For $(name), arg_name is 'name' */
  u32 start_index;  /* Index of $ into the string */
  u32 size;         /* Size of the whole arg. E.g. For $(name), size <- 7 */

  Template_Arg_Method_Kind method_kind;
  union
  {
    s32 offset; /* Argument for .slice(<offset>) */
  } method_arguments;
};

#define MAX_TEMPLATE_STRING_ARGS 16
typedef struct Template_String8 Template_String8;
struct Template_String8
{
  String8 string;
  Template_String8_Arg args[MAX_TEMPLATE_STRING_ARGS];
  u32 args_count;
};

typedef struct Table_Entry Table_Entry;
struct Table_Entry
{
  String8 value;
  u64 column_index;
};

#define HPH_MAX_ENTRIES_PER_ROW 16
typedef struct Table_Row Table_Row;
struct Table_Row
{
  Table_Entry* entries;
  u64 entries_count;
};

#define HPH_MAX_ROWS 128
typedef struct Table Table;
struct Table
{
  Table_Row* rows;
  u32 rows_count;

  String8  name;
  String8* columns;
  u32 columns_count;
};

typedef struct Generator_Command Generator_Command;
struct Generator_Command
{
  Generator_Command_Kind kind;
  Table* table;
  Template_String8 template_string;
};

#define HPH_MAX_GENERATOR_COMMANDS 8
typedef struct Generator Generator;
struct Generator
{
  Generator_Command* command_queue;
  u32 command_count;
};

#define HPH_MAX_TABLES 16
#define HPH_MAX_GENERATORS 16
typedef struct Hephaestus Hephaestus;
struct Hephaestus
{
  Arena* arena;

  String8 output_file_name;
  String8 output_file_path;

  Table* table;
  u32 table_count;
  Generator* generator;
  u32 generator_count;
};

global Lexer lexer;
global Hephaestus hephaestus;

function Token_Array* load_all_tokens(String8 file_path);
function Token        next_token();

function void             process_tokens(Token_Array* array);
function b32              advance_iterator(Token_Iterator* iterator, b32 skip_whitespace);
function b32              advance_iterator_to(Token_Iterator* iterator, Token_Kind kind);
function Template_String8 parse_template_string(Token_Iterator* iterator);
function b32              is_token_whitespace(Token* token);
function void             consume_whitespace(Token_Iterator* iterator);

#define hph_fatal(str8) emit_fatal(string8_concat(g_log_context.arena, S("[Hephaestus]: "), str8));
#define hph_warn(str8)  emit_warn(string8_concat(g_log_context.arena, S("[Hephaestus]: "), str8));

#endif // HEPHAESTUS_H