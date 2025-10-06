#include "hephaestus.h"

function void
entry_point(Command_Line* command_line)
{
  Arena* arena = arena_alloc();
  os_console_init();

  String8 hph_file_path = {0};

  // Get hephaestus file path
  if (command_line->args_count > 0)
  {
    if (command_line->args[0].is_flag)
    {
      hph_file_path = command_line->args[0].value;
    }
    else
    {
      for (u32 i = 0; i < command_line->args_count; i += 1)
      {
        if (string8_match(command_line->args[0].key, S("input"), false))
        {
          hph_file_path = command_line->args[0].value;
          break;
        }
      }
    }
    if (hph_file_path.size > 0)
    {
      hph_file_path = os_absolute_path_from_relative_path(arena, hph_file_path);
    }
    else
    {
      hph_fatal(S("Please provide hph file with -\"/relative/path/to/file\" or --input \"/relative/path/to/file\""));
    }
  }
  else
  {
    hph_fatal(S("Please provide hph file with -\"/relative/path/to/file\" or --input \"/relative/path/to/file\""));
  }

  Token_Array* token_array = load_all_tokens(hph_file_path);
  
  {
    Scratch scratch = scratch_begin(0,0);
    for (u32 i = 0; i < token_array->count; i += 1)
    {
      Token token = token_array->tokens[i];
      string8_print_line(Sf(scratch.arena, "Token :: Type:  %s\n         Value: "S_FMT"\n         Line,Col: %d,%d\n", token_kind_string[token.type], S_ARG(token.value), token.line, token.column));
    }
    scratch_end(&scratch);
  }

  system("pause");
}

function Token_Array*
load_all_tokens(String8 file_path)
{
  Lexer lexer = {0};
  lexer.arena = arena_alloc();

  Token_Array* result = push_array(lexer.arena, Token_Array, 1);
  result->tokens = push_array(lexer.arena, Token, TOKEN_ARRAY_SIZE);
  result->count  = 0;

  File_Data file = os_file_load(lexer.arena, file_path);
  if (file.data.size == 0)
  {
    hph_fatal(Sf(lexer.arena, "Failed to load file: "S_FMT, S_ARG(file_path)));
    return result;
  }

  lexer.file              = file;
  lexer.file_start        = file.data.str;
  lexer.file_end          = file.data.str + file.data.size;
  lexer.current_character = file.data.str;
  lexer.line              = 1;
  lexer.column            = 1;

  for (;;)
  {
    Token token = next_token(&lexer);
    token_array_add(result, token);

    if (token.type == Token_EOF)
      break;

    if (result->count >= TOKEN_ARRAY_SIZE)
    {
      hph_fatal(S("Token array overflow"));
      break;
    }
  }

  return result;
}

function Token
next_token(Lexer* lexer)
{
  Token token = {0};
  token.start_offset = (u32)(lexer->current_character - lexer->file_start);
  token.line         = lexer->line;
  token.column       = lexer->column;

  // End of file
  if (lexer->current_character >= lexer->file_end)
  {
    token.type       = Token_EOF;
    token.value      = S("");
    token.end_offset = token.start_offset;
    return token;
  }

  // Move lexer
  u8 c = *lexer->current_character++;
  if (c == '\n')
  {
    lexer->line += 1;
    lexer->column = 1;
  }
  else
  {
    lexer->column += 1;
  }

  token.end_offset = (u32)(lexer->current_character - lexer->file_start);

  // Control characters
  switch (c)
  {
    case ' ':  token.type = Token_Space;          return token;
    case '\t': token.type = Token_Tab;            return token;
    case '\n': token.type = Token_Newline;        return token;
    case '\r': token.type = Token_CarriageReturn; return token;
    case '\0': token.type = Token_Null;           return token;
    case '\a': token.type = Token_Bell;           return token;
    case '\b': token.type = Token_Backspace;      return token;
    case '\v': token.type = Token_VerticalTab;    return token;
    case '\f': token.type = Token_FormFeed;       return token;
    case 0x1B: token.type = Token_Escape;         return token;
    case 0x7F: token.type = Token_Delete;         return token;
  }

  // Numbers
  if (u8_is_digit(c))
  {
    u8* start = lexer->current_character - 1;
    while (lexer->current_character < lexer->file_end &&
           (u8_is_digit(*lexer->current_character) || *lexer->current_character == '.'))
    {
      lexer->current_character += 1;
      lexer->column += 1;
    }
    token.type = Token_Number;
    token.value = (String8){(u64)(lexer->current_character - start), start};
    token.end_offset = (u32)(lexer->current_character - lexer->file_start);
    return token;
  }

  // String Literal
  if (u8_is_alpha(c))
  {
    u8* start = lexer->current_character - 1;
    while (lexer->current_character < lexer->file_end && u8_is_alphanum(*lexer->current_character))
    {
      lexer->current_character += 1;
      lexer->column += 1;
    }
    token.type       = Token_String_Literal;
    token.value      = (String8){(u64)(lexer->current_character - start), start};
    token.end_offset = (u32)(lexer->current_character - lexer->file_start);
    return token;
  }

  // Symbols
  switch (c)
  {
    case '!':  token.type = Token_Exclamation;  break;
    case '"':  token.type = Token_Quote;        break;
    case '\'': token.type = Token_Apostrophe;   break;
    case '#':  token.type = Token_Hash;         break;
    case '$':  token.type = Token_Dollar;       break;
    case '%':  token.type = Token_Percent;      break;
    case '&':  token.type = Token_Ampersand;    break;
    case '(':  token.type = Token_ParenOpen;    break;
    case ')':  token.type = Token_ParenClose;   break;
    case '*':  token.type = Token_Asterisk;     break;
    case '+':  token.type = Token_Plus;         break;
    case ',':  token.type = Token_Comma;        break;
    case '-':  token.type = Token_Minus;        break;
    case '.':  token.type = Token_Dot;          break;
    case '/':  token.type = Token_Slash;        break;
    case ':':  token.type = Token_Colon;        break;
    case ';':  token.type = Token_Semicolon;    break;
    case '<':  token.type = Token_Less;         break;
    case '=':  token.type = Token_Equals;       break;
    case '>':  token.type = Token_Greater;      break;
    case '?':  token.type = Token_Question;     break;
    case '@':  token.type = Token_At;           break;
    case '[':  token.type = Token_BracketOpen;  break;
    case ']':  token.type = Token_BracketClose; break;
    case '\\': token.type = Token_Backslash;    break;
    case '^':  token.type = Token_Caret;        break;
    case '_':  token.type = Token_Underscore;   break;
    case '`':  token.type = Token_Backtick;     break;
    case '{':  token.type = Token_BraceOpen;    break;
    case '}':  token.type = Token_BraceClose;   break;
    case '|':  token.type = Token_Pipe;         break;
    case '~':  token.type = Token_Tilde;        break;
    default:   token.type = Token_Unknown;      break;
  }

  token.value = string8_new(1, lexer->current_character - 1);
  return token;
}

function void
token_array_add(Token_Array* array, Token token)
{
  array->tokens[array->count] = token;
  array->count += 1;
}