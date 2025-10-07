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
      string8_print_line(Sf(scratch.arena, "Token :: Kind:  %s\n         Value: "S_FMT"\n         Line,Col: %d,%d\n", token_kind_string[token.kind], S_ARG(token.value), token.line, token.column));
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
    result->tokens[result->count] = token;
    result->count += 1;

    if (token.kind == Token_EOF)
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
    token.kind       = Token_EOF;
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
    case ' ':  token.kind = Token_Space;          return token;
    case '\t': token.kind = Token_Tab;            return token;
    case '\n': token.kind = Token_Newline;        return token;
    case '\r': token.kind = Token_CarriageReturn; return token;
    case '\0': token.kind = Token_Null;           return token;
    case '\a': token.kind = Token_Bell;           return token;
    case '\b': token.kind = Token_Backspace;      return token;
    case '\v': token.kind = Token_VerticalTab;    return token;
    case '\f': token.kind = Token_FormFeed;       return token;
    case 0x1B: token.kind = Token_Escape;         return token;
    case 0x7F: token.kind = Token_Delete;         return token;
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
    token.kind = Token_Number;
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
    token.kind       = Token_String_Literal;
    token.value      = (String8){(u64)(lexer->current_character - start), start};
    token.end_offset = (u32)(lexer->current_character - lexer->file_start);
    return token;
  }

  // Symbols
  switch (c)
  {
    case '!':  token.kind = Token_Exclamation;  break;
    case '"':  token.kind = Token_Quote;        break;
    case '\'': token.kind = Token_Apostrophe;   break;
    case '#':  token.kind = Token_Hash;         break;
    case '$':  token.kind = Token_Dollar;       break;
    case '%':  token.kind = Token_Percent;      break;
    case '&':  token.kind = Token_Ampersand;    break;
    case '(':  token.kind = Token_ParenOpen;    break;
    case ')':  token.kind = Token_ParenClose;   break;
    case '*':  token.kind = Token_Asterisk;     break;
    case '+':  token.kind = Token_Plus;         break;
    case ',':  token.kind = Token_Comma;        break;
    case '-':  token.kind = Token_Minus;        break;
    case '.':  token.kind = Token_Dot;          break;
    case '/':  token.kind = Token_Slash;        break;
    case ':':  token.kind = Token_Colon;        break;
    case ';':  token.kind = Token_Semicolon;    break;
    case '<':  token.kind = Token_Less;         break;
    case '=':  token.kind = Token_Equals;       break;
    case '>':  token.kind = Token_Greater;      break;
    case '?':  token.kind = Token_Question;     break;
    case '@':  token.kind = Token_At;           break;
    case '[':  token.kind = Token_BracketOpen;  break;
    case ']':  token.kind = Token_BracketClose; break;
    case '\\': token.kind = Token_Backslash;    break;
    case '^':  token.kind = Token_Caret;        break;
    case '_':  token.kind = Token_Underscore;   break;
    case '`':  token.kind = Token_Backtick;     break;
    case '{':  token.kind = Token_BraceOpen;    break;
    case '}':  token.kind = Token_BraceClose;   break;
    case '|':  token.kind = Token_Pipe;         break;
    case '~':  token.kind = Token_Tilde;        break;
    default:   token.kind = Token_Unknown;      break;
  }

  token.value = string8_new(1, lexer->current_character - 1);
  return token;
}

function void
parse_tokens(Token_Array* array)
{
  if (array->count <= 0)
  {
    hph_fatal(S("No tokens in Token_Array"));
    return;
  }

  Scratch scratch = scratch_begin(0,0);

  Token_Iterator token_iterator = {0};
  token_iterator.array  = array;
  token_iterator.cursor = 0;
  token_iterator.current_token = token_iterator.array[token_iterator.cursor];

  Token* token = next_non_whitespace_token(&token_iterator);

  while (token->kind != Token_EOF)
  {
    if (token->kind == Token_At)
    {
      // Parse Config
      if (string8_match(token->value, S("config"), false))
      {
        while(token->kind != Token_BraceOpen) 
        {
          token = next_non_whitespace_token(&token_iterator);
        }
        token = next_non_whitespace_token(&token_iterator);

        if (token->kind == Token_At)
        {
          token = next_non_whitespace_token(&token_iterator);
          if (string8_match(token->value, S("output_file_name"), false))
          {
          
          }
          if (string8_match(token->value, S("output_file_location"), false))
          {
          
          }
        }
        else
        {
          hph_fatal(Sf(scratch.arena, "L:%d C:%d Unexpected Token inside @config. Expected Token_At, got "S_FMT"", token->line, token->column, token_kind_string[token->kind]));
        }
      }

      // Parse tables
      else if (string8_match(token->value, S("table"), false))
      {
        token = next_token();      
      }

      // Parse generates
      else if (string8_match(token->value, S("generate"), false))
      {
        token = next_token();
      }
    }
    else
    {
      hph_fatal(Sf(scratch.arena, "L:%d C:%d Expected '@<command>' at the global scope. Instead  got "S_FMT"", token->line, token->column, token_kind_string[token->kind]));
    }
  }
  scratch_end(&scratch);
}

function void
next_non_whitespace_token(Token_Iterator* iterator)
{
  if (advance_iterator(iterator))
  {
    while (iterator->current_token->kind == Token_Space          ||
           iterator->current_token->kind == Token_Tab            ||
           iterator->current_token->kind == Token_Newline        ||
           iterator->current_token->kind == Token_CarriageReturn ||
           iterator->current_token->kind == Token_Null           ||
           iterator->current_token->kind == Token_Bell           ||
           iterator->current_token->kind == Token_Backspace      ||
           iterator->current_token->kind == Token_VerticalTab    ||
           iterator->current_token->kind == Token_FormFeed       ||
           iterator->current_token->kind == Token_Escape         ||
           iterator->current_token->kind == Token_Delete)
    {
      if (!advance_iterator(iterator))
      {
        break;
      }
    }
  }
}

function b32
advance_iterator(Token_Iterator* iterator)
{
  b32 result = true;
  if (iterator->cursor + 1 >= iterator->array->count)
  {
    hph_warn(S("Tried to advance token iterator beyond max."));
    result = false;
  }
  else
  {
    iterator->cursor += 1;
    iterator->current_token = &iterator->array->tokens[iterator->cursor];
  }
  return result;
}

function b32
advance_iterator_to(Token_Iterator* iterator, Token_Kind kind)
{
  b32 result = false;

  for (;;)
  {
    if (!advance_iterator(iterator))
    {
      break;
    }
    if (iterator->current_token->kind == kind)
    {
      result = true;
      break;
    }
  }

  return result;
}