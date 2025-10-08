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
  
  // Init hph
  hephaestus                 = (Hephaestus){0};
  hephaestus.arena           = arena_alloc();
  hephaestus.table           = push_array(hephaestus.arena, Table, HPH_MAX_TABLES);
  hephaestus.table_count     = 0;
  hephaestus.generator       = push_array(hephaestus.arena, Generator, HPH_MAX_GENERATORS);
  hephaestus.generator_count = 0;

  Token_Array* token_array = load_all_tokens(hph_file_path);
  process_tokens(token_array);
  run_hephaestus();
}

function Token_Array*
load_all_tokens(String8 file_path)
{
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
next_token()
{
  Token token = {0};
  token.start_offset = (u32)(lexer.current_character - lexer.file_start);
  token.line         = lexer.line;
  token.column       = lexer.column;

  // End of file
  if (lexer.current_character >= lexer.file_end)
  {
    token.kind       = Token_EOF;
    token.value      = S("");
    token.end_offset = token.start_offset;
    return token;
  }

  // Move lexer
  u8 c = *lexer.current_character++;
  if (c == '\n')
  {
    lexer.line += 1;
    lexer.column = 1;
  }
  else
  {
    lexer.column += 1;
  }

  token.end_offset = (u32)(lexer.current_character - lexer.file_start);

  // Control characters
  switch (c)
  {
    case ' ':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Space;
      return token;
    }
    case '\t':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Tab;
      return token;
    }
    case '\n':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Newline;
      return token;
    }
    case '\r':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_CarriageReturn;
      return token;
    }
    case '\0':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Null;
      return token;
    }
    case '\a':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Bell;
      return token;
    }
    case '\b':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Backspace;
      return token;
    }
    case '\v':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_VerticalTab;
      return token;
    }
    case '\f':
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_FormFeed;
      return token;
    }
    case 0x1B:
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Escape;
      return token;
    }
    case 0x7F:
    {
      u8* start = lexer.current_character - 1;
      token.value = (String8){(u64)(lexer.current_character - start), start};
      token.kind = Token_Delete;
      return token;
    }
  }

  // Numbers
  if (u8_is_digit(c) || (c == '-' && u8_is_digit(*lexer.current_character)))
  {
    u8* start = lexer.current_character - 1;
    while (lexer.current_character < lexer.file_end &&
           (u8_is_digit(*lexer.current_character) || *lexer.current_character == '.'))
    {
      lexer.current_character += 1;
      lexer.column += 1;
    }
    token.kind = Token_Number;
    token.value = (String8){(u64)(lexer.current_character - start), start};
    token.end_offset = (u32)(lexer.current_character - lexer.file_start);
    return token;
  }

  // Identifiers
  if (u8_is_alpha(c) || c == '_')
  {
    u8* start = lexer.current_character - 1;

    // consume valid identifier characters: [A-Za-z0-9_]
    while (lexer.current_character < lexer.file_end)
    {
      u8 ch = *lexer.current_character;
      if (u8_is_alphanum(ch) || ch == '_')
      {
        lexer.current_character += 1;
        lexer.column += 1;
      }
      else
      {
        break;
      }
    }

    token.kind       = Token_String_Identifier;
    token.value      = (String8){(u64)(lexer.current_character - start), start};
    token.end_offset = (u32)(lexer.current_character - lexer.file_start);
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

  token.value = string8_new(1, lexer.current_character - 1);
  return token;
}

function void
process_tokens(Token_Array* array)
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
  token_iterator.current_token = &token_iterator.array->tokens[token_iterator.cursor];

  while (token_iterator.current_token->kind != Token_EOF)
  {
    if (token_iterator.current_token->kind == Token_At)
    {
      advance_iterator(&token_iterator, true);

      // Parse Config
      // --------------------
      if (string8_match(token_iterator.current_token->value, S("config"), false))
      {
        advance_iterator(&token_iterator, true);

        if (token_iterator.current_token->kind != Token_BraceOpen)
        {
          hph_fatal(Sf(scratch.arena, "L:%d C:%d Expected '{' after @config", token_iterator.current_token->line, token_iterator.current_token->column));
        }

        advance_iterator(&token_iterator, true);

        while (token_iterator.current_token->kind != Token_BraceClose && token_iterator.current_token->kind != Token_EOF)
        {
          if (token_iterator.current_token->kind == Token_At)
          {
            advance_iterator(&token_iterator, true);

            if (string8_match(token_iterator.current_token->value, S("output_file_name"), false))
            {
              // output_file_name
              advance_iterator(&token_iterator, true);
              Template_String8 template_string = parse_template_string(&token_iterator);
              hephaestus.output_file_name = template_string.string;
              if (hephaestus.output_file_name.size == 0)
              {
                hph_fatal(S("Must provide a valid @output_file_name inside @config."));
              }
            }
            else if (string8_match(token_iterator.current_token->value, S("output_file_path"), false))
            {
              // handle output_file_location
              advance_iterator(&token_iterator, true);
              Template_String8 template_string = parse_template_string(&token_iterator);
              hephaestus.output_file_path = os_absolute_path_from_relative_path(hephaestus.arena, template_string.string);
              if (hephaestus.output_file_path.size == 0)
              {
                hph_fatal(S("Must provide a valid @output_file_path inside @config."));
              }
            }
            else
            {
              hph_warn(Sf(scratch.arena, "L:%d C:%d Unknown @config field", token_iterator.current_token->line, token_iterator.current_token->column));
            }
          }
          else
          {
            advance_iterator(&token_iterator, true);
          }
        }

        if (token_iterator.current_token->kind == Token_BraceClose)
        {
          advance_iterator(&token_iterator, true);
        }
      }

      // Parse Table
      // --------------------
      else if (string8_match(token_iterator.current_token->value, S("table"), false))
      {
        Table* table = &hephaestus.table[hephaestus.table_count++];
        advance_iterator(&token_iterator, true);

        // table name
        if (token_iterator.current_token->kind == Token_String_Identifier)
        {
          table->name = string8_copy(hephaestus.arena, token_iterator.current_token->value);
          advance_iterator(&token_iterator, true);
        }

        // expect '(' ... ')' for header
        if (token_iterator.current_token->kind == Token_ParenOpen)
        {
          String8_List columns = string8_list_empty();
          advance_iterator(&token_iterator, true);

          while (token_iterator.current_token->kind != Token_ParenClose && token_iterator.current_token->kind != Token_EOF)
          {
            string8_list_push(scratch.arena, &columns, token_iterator.current_token->value);
            advance_iterator(&token_iterator, true);
          }

          if (token_iterator.current_token->kind == Token_ParenClose)
          {
            table->columns_count = columns.node_count;
            table->columns = push_array(hephaestus.arena, String8, columns.node_count);

            u32 index = 0;
            for (String8_Node* node = columns.first; node; node = node->next)
            {
              table->columns[index] = node->value;
              index += 1;
            }

            advance_iterator(&token_iterator, true);
          }
          else
          {
            hph_fatal(S("Expected Token_ParenClose after parsing table columns."));
          }
        }

        table->rows = push_array(hephaestus.arena, Table_Row, HPH_MAX_ROWS);
        table->rows_count = 0;

        // expect '{' ... '}' for table body
        if (token_iterator.current_token->kind == Token_BraceOpen)
        {
          advance_iterator(&token_iterator, true);

          // Parse rows
          while (token_iterator.current_token->kind != Token_BraceClose && token_iterator.current_token->kind != Token_EOF)
          {
            if (token_iterator.current_token->kind == Token_BraceOpen)
            {
              // New row
              Table_Row* row = &table->rows[table->rows_count++];
              row->entries = push_array(hephaestus.arena, Table_Entry, HPH_MAX_ENTRIES_PER_ROW);
              row->entries_count = 0;

              advance_iterator(&token_iterator, true);
              u32 column_index = 0;
              while (token_iterator.current_token->kind != Token_BraceClose && token_iterator.current_token->kind != Token_EOF)
              {
                if (token_iterator.current_token->kind == Token_Backtick)
                {
                  Template_String8 template_string = parse_template_string(&token_iterator);
                  row->entries[row->entries_count].value = template_string.string;
                  row->entries[row->entries_count].column_index = column_index;
                  row->entries_count += 1;
                  column_index += 1;
                }
                else
                {
                  String8 value = token_iterator.current_token->value;
                  advance_iterator(&token_iterator, false);
                  while (!is_token_whitespace(token_iterator.current_token))
                  {
                    value = string8_concat(scratch.arena, value, token_iterator.current_token->value);
                    advance_iterator(&token_iterator, false);
                  }
                  row->entries[row->entries_count].value = value;
                  row->entries[row->entries_count].column_index = column_index;
                  row->entries_count += 1;
                  column_index += 1;
                }
                advance_iterator(&token_iterator, true);

              }
              if (token_iterator.current_token->kind == Token_BraceClose)
              {
                advance_iterator(&token_iterator, true);
              }
            }
            else
            {
              advance_iterator(&token_iterator, true);
            }
          }

          if (token_iterator.current_token->kind == Token_BraceClose)
          {
            advance_iterator(&token_iterator, true);
          }
        }
      }

      // Parse Generate
      // --------------------
      else if (string8_match(token_iterator.current_token->value, S("generate"), false))
      {
        advance_iterator(&token_iterator, true);

        if (token_iterator.current_token->kind != Token_BraceOpen)
        {
          hph_fatal(Sf(scratch.arena, "L:%d C:%d Expected '{' after @generate", token_iterator.current_token->line, token_iterator.current_token->column));
        }

        advance_iterator(&token_iterator, true);

        Generator* generator = &hephaestus.generator[hephaestus.generator_count++];
        generator->command_queue = push_array(hephaestus.arena, Generator_Command, HPH_MAX_GENERATOR_COMMANDS);

        while (token_iterator.current_token->kind != Token_BraceClose && token_iterator.current_token->kind != Token_EOF)
        {
          if (token_iterator.current_token->kind == Token_Backtick)
          {
            // Code block
            Template_String8 template_string = parse_template_string(&token_iterator);
            generator->command_queue[generator->command_count].kind  = Generator_Command_String;
            generator->command_queue[generator->command_count].table = NULL; // These don't need table reference
            generator->command_queue[generator->command_count].template_string = template_string;

            generator->command_count += 1;
          }
          else if (token_iterator.current_token->kind == Token_At)
          {
            advance_iterator(&token_iterator, true);
            if (string8_match(token_iterator.current_token->value, S("foreach"), false))
            {
              Table* table_ptr = NULL;
              advance_iterator(&token_iterator, true);
              if (token_iterator.current_token->kind == Token_ParenOpen)
              {
                advance_iterator(&token_iterator, true);
                for (u32 i = 0; i < hephaestus.table_count; i += 1)
                {
                  if (string8_match(token_iterator.current_token->value, hephaestus.table[i].name, true))
                  {
                    table_ptr = &hephaestus.table[i];

                    // Ensure proper closure
                    advance_iterator(&token_iterator, true);
                    if (token_iterator.current_token->kind != Token_ParenClose)
                    {
                      hph_fatal(S("Expected ')' after table name."));
                    }
                    advance_iterator(&token_iterator, true);
                    break;
                  }
                }
              }
              else
              {
                hph_fatal(S("Expected table name between () after foreach. E.g. @foreach(Table_Name) `My $(template) string.`"));
              }

              // Foreach block
              Template_String8 template_string = parse_template_string(&token_iterator);
              generator->command_queue[generator->command_count].kind  = Generator_Command_Foreach;
              generator->command_queue[generator->command_count].table = table_ptr; // These don't need table reference
              generator->command_queue[generator->command_count].template_string = template_string;

              generator->command_count += 1;
            }
          }
          else
          {
            advance_iterator(&token_iterator, true);
          }
        }

        if (token_iterator.current_token->kind == Token_BraceClose)
        {
          advance_iterator(&token_iterator, true);
        }
      }
    }
    else
    {
      advance_iterator(&token_iterator, true);
    }
  }

  scratch_end(&scratch);
}

function void
run_hephaestus()
{
  // Build output file
  String8 file = string8_concat(hephaestus.arena, hephaestus.output_file_path, hephaestus.output_file_name);
  String8 content = S("");

  for (u32 generator_index = 0; generator_index < hephaestus.generator_count; generator_index += 1)
  {
    Generator* generator = &hephaestus.generator[generator_index];

    for (u32 commang_index = 0; commang_index < generator->command_count; commang_index += 1)
    {
      Generator_Command* command = &generator->command_queue[commang_index];
      switch (command->kind)
      {
        case Generator_Command_String:
        {
          for (u32 arg_index = 0; arg_index < command->template_string.args_count; arg_index += 1)
          {
            String8 string = string8_copy(hephaestus.arena, command->template_string.string);
            Template_String8_Arg* arg = &command->template_string.args[arg_index];
            switch (arg->kind)
            {
              // Replace with date
              case Template_String_Variable_Time_Now:
              {
                String8 time_now = os_datetime_to_string8(hephaestus.arena, os_datetime_now(), false);
                string = string8_place_at(hephaestus.arena, string, time_now, arg->start_index);
              } break;
              default:
              {
                hph_fatal(S("Argument kind not allowed inside a string command."));
              }
            }
            content = string8_concat(hephaestus.arena, content, string);
          }
        } break;
        case Generator_Command_Foreach:
        {
          for (u32 row_index = 0; row_index < command->table->rows_count; row_index += 1)
          {
            Table_Row* row = &command->table->rows[row_index];
            String8 string = string8_copy(hephaestus.arena, command->template_string.string);

            for (u32 arg_index = 0; arg_index < command->template_string.args_count; arg_index += 1)
            {
              Template_String8_Arg* arg = &command->template_string.args[arg_index];
              switch (arg->kind)
              {
                // Replace with date
                case Template_String_Variable_Time_Now:
                {
                  String8 time_now = os_datetime_to_string8(hephaestus.arena, os_datetime_now(), false);
                  string = string8_place_at(hephaestus.arena, string, time_now, arg->start_index);
                } break;

                // Place row index
                case Template_String_Variable_Index:
                {
                  string = string8_place_at(hephaestus.arena, string, Sf(hephaestus.arena, "%d", row_index), arg->start_index);
                } break;

                // Replace with respective table row/col value
                case Template_String_Variable_Replace:
                {
                  u32 index = U32_MAX;
                  for (u32 column_idx = 0; column_idx < command->table->columns_count; column_idx += 1)
                  {
                    if (string8_match(arg->arg_name, command->table->columns[column_idx], true))
                    {
                      index = column_idx;
                    }
                  }
                  if (index == U32_MAX)
                  {
                    hph_fatal(Sf(hephaestus.arena, "Unable to find header "S_FMT" in table "S_FMT"", S_ARG(arg->arg_name), S_ARG(command->table->name)));
                  }
                  Table_Entry entry = row->entries[index];

                  string = string8_place_at(hephaestus.arena, string, entry.value, arg->start_index);
                } break;
                default:
                {
                  hph_fatal(S("Argument kind not allowed inside a string command."));
                }
              }
            }
            content = string8_concat(hephaestus.arena, content, string);
          }
        } break;
        default:
        {
          hph_fatal(S("Unhandled command kind."));
        } break;
      }
    }
  }

  os_file_wipe(file);
  os_file_append(file, content.str, content.size);
}

function b32
advance_iterator(Token_Iterator* iterator, b32 skip_whitespace)
{
  if (iterator->cursor + 1 >= iterator->array->count)
  {
    hph_warn(S("Tried to advance token iterator beyond max."));
    return false;
  }

  b32 result = true;
  b32 am_i_a_string = (iterator->current_token->kind == Token_Backtick); // If it is a string, we ignore the comments rule

  iterator->cursor += 1;
  iterator->current_token = &iterator->array->tokens[iterator->cursor];

  for (;;)
  {
    // always skip comments (defined by //) until newline or carriage return
    if (iterator->current_token->kind == Token_Slash &&
        iterator->cursor + 1 < iterator->array->count &&
        iterator->array->tokens[iterator->cursor + 1].kind == Token_Slash &&
        !am_i_a_string)
    {
      iterator->cursor += 2;
      if (iterator->cursor >= iterator->array->count)
      {
        result = false;
        break;
      }
      iterator->current_token = &iterator->array->tokens[iterator->cursor];

      while (iterator->current_token->kind != Token_Newline &&
             iterator->current_token->kind != Token_CarriageReturn &&
             iterator->current_token->kind != Token_EOF)
      {
        if (iterator->cursor + 1 >= iterator->array->count)
        {
          result = false;
          break;
        }
        iterator->cursor += 1;
        iterator->current_token = &iterator->array->tokens[iterator->cursor];
      }

      if (iterator->cursor + 1 < iterator->array->count)
      {
        iterator->cursor += 1;
        iterator->current_token = &iterator->array->tokens[iterator->cursor];
      }
      else
      {
        result = false;
        break;
      }

      if (!skip_whitespace)
      {
        break;
      }
      else
      {
        continue;
      }
    }

    if (skip_whitespace)
    {
      if (is_token_whitespace(iterator->current_token))
      {
        if (iterator->cursor + 1 >= iterator->array->count)
        {
          result = false;
          break;
        }
        iterator->cursor += 1;
        iterator->current_token = &iterator->array->tokens[iterator->cursor];
        continue;
      }
    }

    break;
  }

  return result;
}

function b32
advance_iterator_to(Token_Iterator* iterator, Token_Kind kind)
{
  b32 result = false;

  for (;;)
  {
    if (!advance_iterator(iterator, false))
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

function Template_String8
parse_template_string(Token_Iterator* iterator)
{
  if (iterator->current_token->kind != Token_Backtick)
  {
    hph_fatal(S("parse_template_string() requires current token to be a backtick"));
  }

  Template_String8 result = {0};
  result.args = push_array(hephaestus.arena, Template_String8_Arg, MAX_TEMPLATE_STRING_ARGS);

  Scratch scratch = scratch_begin(0,0);
  u32 line   = iterator->current_token->line;
  u32 column = iterator->current_token->column;

  // skip opening backtick
  advance_iterator(iterator, false);

  String8_List result_list = {0};
  u32 string_index = 0;

  for (;;)
  {
    Token* token = iterator->current_token;
    if (token->kind == Token_EOF)
    {
      hph_fatal(Sf(scratch.arena, "Backtick string (starting at L:%u C:%u) not closed.", line, column));
    }
    if (token->kind == Token_Backtick)
    {
      break;
    }

    if (token->kind == Token_Dollar)
    {
      advance_iterator(iterator, false);
      if (iterator->current_token->kind != Token_ParenOpen)
      {
        hph_fatal(S("Expected '(' after '$' in template string."));
      }

      advance_iterator(iterator, false);

      Template_String8_Arg* arg = &result.args[result.args_count];
      arg->kind        = Template_String_Variable_Unknown;
      arg->method_kind = Template_Arg_Method_None;
      arg->start_index = string_index;

      if (iterator->current_token->kind == Token_At)
      {
        advance_iterator(iterator, false);
        String8 name = iterator->current_token->value;

        // Special variables
        if (string8_match(name, S("time_now"), true))
        {
          arg->kind = Template_String_Variable_Time_Now;
          arg->arg_name = S("@time_now");
        }
        else if (string8_match(name, S("index"), true))
        {
          arg->kind = Template_String_Variable_Index;
          arg->arg_name = S("@index");
        }
        else
        {
          hph_fatal(Sf(scratch.arena, "Unknown template special variable @" S_FMT, S_ARG(name)));
        }

        advance_iterator(iterator, false);
      }
      else if (iterator->current_token->kind == Token_String_Identifier)
      {
        arg->kind = Template_String_Variable_Replace;
        arg->arg_name = iterator->current_token->value;
        advance_iterator(iterator, false);

        if (iterator->current_token->kind == Token_Dot)
        {
          advance_iterator(iterator, false);

          if (string8_match(iterator->current_token->value, S("truncate"), true))
          {
            advance_iterator(iterator, false);

            if (iterator->current_token->kind != Token_ParenOpen)
            {
              hph_fatal(S("Expected '(' after .slice"));
            }
            advance_iterator(iterator, false);

            if (iterator->current_token->kind != Token_Number)
            {
              hph_fatal(S("Expected integer offset inside .slice(<offset>)"));
            }

            String8 offset_token = iterator->current_token->value;
            s32 offset = atoi(cstring_from_string8(scratch.arena, offset_token));
            arg->method_kind = Template_Arg_Method_Truncate;
            arg->method_arguments.offset = offset;
            advance_iterator(iterator, false);

            if (iterator->current_token->kind != Token_ParenClose)
            {
              hph_fatal(S("Expected ')' after slice offset"));
            }
            advance_iterator(iterator, false);

          }
          else
          {
            hph_fatal(Sf(scratch.arena, "Unknown method '" S_FMT "' after '.' in template string.", S_ARG(iterator->current_token->value)));
          }
        }
        else if (iterator->current_token->kind == Token_Plus)
        {
          advance_iterator(iterator, false);
          
          if (iterator->current_token->kind != Token_Number)
          {
            hph_fatal(S("Cannot do a math operation '+' inside a variable when the variable does no expand to a number."));
          }
      
          s32 operand = atoi(cstring_from_string8(scratch.arena, iterator->current_token->value));
          arg->method_kind = Template_Arg_Method_Add;
          arg->method_arguments.operand = operand;
          advance_iterator(iterator, false);
        }
        else if (iterator->current_token->kind == Token_Minus)
        {
          advance_iterator(iterator, false);
          
          if (iterator->current_token->kind != Token_Number)
          {
            hph_fatal(S("Cannot do a math operation '-' inside a variable when the variable does no expand to a number."));
          }
      
          s32 operand = atoi(cstring_from_string8(scratch.arena, iterator->current_token->value));
          arg->method_kind = Template_Arg_Method_Sub;
          arg->method_arguments.operand = operand;
          advance_iterator(iterator, false);
        }
        else if (iterator->current_token->kind == Token_Asterisk)
        {
          advance_iterator(iterator, false);
          
          if (iterator->current_token->kind != Token_Number)
          {
            hph_fatal(S("Cannot do a math operation '*' inside a variable when the variable does no expand to a number."));
          }
      
          s32 operand = atoi(cstring_from_string8(scratch.arena, iterator->current_token->value));
          arg->method_kind = Template_Arg_Method_Mul;
          arg->method_arguments.operand = operand;
          advance_iterator(iterator, false);
        }
        else if (iterator->current_token->kind == Token_Slash)
        {
          advance_iterator(iterator, false);
          
          if (iterator->current_token->kind != Token_Number)
          {
            hph_fatal(S("Cannot do a math operation '/' inside a variable when the variable does no expand to a number."));
          }
      
          s32 operand = atoi(cstring_from_string8(scratch.arena, iterator->current_token->value));
          arg->method_kind = Template_Arg_Method_Div;
          arg->method_arguments.operand = operand;
          advance_iterator(iterator, false);
        }
        else if (iterator->current_token->kind == Token_Percent)
        {
          advance_iterator(iterator, false);
          
          if (iterator->current_token->kind != Token_Number)
          {
            hph_fatal(S("Cannot do a math operation '%' inside a variable when the variable does no expand to a number."));
          }
      
          s32 operand = atoi(cstring_from_string8(scratch.arena, iterator->current_token->value));
          arg->method_kind = Template_Arg_Method_Mod;
          arg->method_arguments.operand = operand;
          advance_iterator(iterator, false);
        }
      }
      else
      {
        hph_fatal(S("Expected identifier or '@' after '$(' in template string."));
      }

      if (iterator->current_token->kind != Token_ParenClose)
      {
        hph_fatal(S("Expected ')' after variable name in template string."));
      }

      arg->size = 3 + arg->arg_name.size; // "$(" + name + ")"
      result.args_count += 1;
      string_index += arg->size;

      advance_iterator(iterator, false); // skip ')'
    }
    else
    {
      Token* token = iterator->current_token;
      Token* next_token = 0;
      if (iterator->cursor + 1 < iterator->array->count)
      {
        next_token = &iterator->array->tokens[iterator->cursor + 1];
      }

      // Replace string "\n" with new line byte
      if (token->value.size == 1 && token->value.str[0] == '\\' && next_token && next_token->value.size > 0 && next_token->value.str[0] == 'n')
      {
        u8* new_line = push_array(hephaestus.arena, u8, 1);
        new_line[0] = '\n';
        string8_list_push(hephaestus.arena, &result_list, (String8){1, new_line});
        string_index += 1;

        // push leftover after 'n' if present
        if (next_token->value.size > 1)
        {
          String8 leftover = { next_token->value.size - 1, next_token->value.str + 1 };
          string8_list_push(hephaestus.arena, &result_list, leftover);
          string_index += leftover.size;
        }

        // advance twice
        advance_iterator(iterator, false);
        advance_iterator(iterator, false);
      }
      else
      {
        string8_list_push(hephaestus.arena, &result_list, token->value);
        string_index += token->value.size;
        advance_iterator(iterator, false);
      }
    }
  }

  result.string = string8_list_join(hephaestus.arena, &result_list);
  advance_iterator(iterator, false); // skip closing backtick
  scratch_end(&scratch);
  return result;
}

function b32
is_token_whitespace(Token* token)
{
  b32 result = false;
  if (token->kind == Token_Space          ||
      token->kind == Token_Tab            ||
      token->kind == Token_Newline        ||
      token->kind == Token_CarriageReturn ||
      token->kind == Token_Null           ||
      token->kind == Token_Bell           ||
      token->kind == Token_Backspace      ||
      token->kind == Token_VerticalTab    ||
      token->kind == Token_FormFeed       ||
      token->kind == Token_Escape         ||
      token->kind == Token_Delete)
  {
    result = true;
  }
  return result;
}

function void
consume_whitespace(Token_Iterator* iterator)
{
  while (is_token_whitespace(iterator->current_token))
  {
    advance_iterator(iterator, false);
  }
}