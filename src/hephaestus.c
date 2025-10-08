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
              case Template_String_Variable_Unknown: { /* Don't do anything */ } break;
              case Template_String_Variable_Time_Now:
              {
                String8 time_now = os_datetime_to_string8(hephaestus.arena, os_datetime_now(), false);
                string = string8_replace_first(hephaestus.arena, string, S("$(@time_now)"), time_now);
              } break;
              case Template_String_Variable_Replace:
              {
              
              } break;
              default:
              {
                hph_fatal(S("Argument kind not found or not allowed inside a string command."));
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
                  string = string8_replace_first(hephaestus.arena, string, S("$(@time_now)"), time_now);
                } break;

                // Place row index
                case Template_String_Variable_Index:
                {
                  string = string8_replace_first(hephaestus.arena, string, S("$(@index)"), Sf(hephaestus.arena, "%d", row_index));
                } break;

                // Replace with respective table row/col value
                case Template_String_Variable_Replace:
                {
                  String8 arg_name = arg->name;
                  u64 split_index = arg_name.size;
                  s32 operand = 0;

                  for (u64 i = 0; i < arg_name.size; i += 1)
                  {
                    u8 c = arg_name.str[i];
                    if (c == '+' || c == '-')
                    {
                      split_index = i;
                      String8 num_str = {arg_name.size - i, arg_name.str + i};
                      operand = atoi(cstring_from_string8(hephaestus.arena, num_str));
                      break;
                    }
                  }
                  String8 base_name = {split_index, arg_name.str};

                  u32 index = U32_MAX;
                  for (u32 column_idx = 0; column_idx < command->table->columns_count; column_idx += 1)
                  {
                    if (string8_match(base_name, command->table->columns[column_idx], true))
                    {
                      index = column_idx;
                    }
                  }
                  if (index == U32_MAX)
                  {
                    hph_fatal(Sf(hephaestus.arena, "Unable to find header "S_FMT" in table "S_FMT"", S_ARG(base_name), S_ARG(command->table->name)));
                  }

                  Table_Entry entry = row->entries[index];

                  // Handle operands, if they exist. Truncate string or add to number
                  if (operand != 0)
                  {
                    s32 value = F32_MIN;
                    if (s32_from_string8(entry.value, &value))
                    {
                      entry.value = Sf(hephaestus.arena, "%d", operand + value);
                    }
                    else
                    {
                      // If its not a number, try to truncate
                      if (operand > 0)
                      {
                        entry.value.str  += operand;
                        entry.value.size -= (u64)operand;
                      }
                      else if (operand < 0)
                      {
                        entry.value.size -= (u64)(-operand);
                      }
                    }
                  }

                  string = string8_replace_first(hephaestus.arena, string, Sf(hephaestus.arena, "$("S_FMT")", S_ARG(arg->name)), entry.value);
                } break;
                default:
                {
                  hph_fatal(S("Argument kind not found or not allowed inside a string command."));
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
    hph_fatal(S("Tried to advance token iterator beyond max."));
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
  advance_iterator(iterator, true);

  // We handle the template string per byte, so we'll just skip the iterator to the end of the string.
  String8_List list = string8_list_empty();
  while (iterator->current_token->kind != Token_Backtick)
  {
    if (iterator->current_token->kind == Token_EOF)
    {
     hph_fatal(S("Unexpected EOF while parsing backtick string."));
    }

    string8_list_push(hephaestus.arena, &list, iterator->current_token->value);
    advance_iterator(iterator, false);
  }
  advance_iterator(iterator, false);
  String8 full_string = string8_list_join(hephaestus.arena, &list);

  String8 search = S("\\n");
  String8 replacement;
  replacement.size = 1;
  replacement.str  = push_array(hephaestus.arena, u8, 1);
  replacement.str[0] = '\n';
  full_string = string8_replace_all(hephaestus.arena, full_string, search, replacement);

  Template_String8 result = {0};
  result.args   = push_array(hephaestus.arena, Template_String8_Arg, MAX_TEMPLATE_STRING_ARGS);
  result.string = full_string;

#define safe_advance(idx) if ((idx)+1 >= result.string.size) {hph_fatal(S("Incomplete variable in template string"));} else {(idx) += 1; c = result.string.str[(idx)]; }
  for (u32 i = 0; i < result.string.size; i += 1)
  {
    u8 c = result.string.str[i];

    if (c == '$')
    {
      Template_String8_Arg* arg = &result.args[result.args_count];
      result.args_count += 1;

      safe_advance(i);
      if (c != '(')
      {
        hph_fatal(S("Expected '(' after $ in template string."));
      }
      safe_advance(i);
      u8* start = &result.string.str[i];
      u32 start_index = i;
      u32 paren_level = 1;
      while (paren_level > 0)
      {
        safe_advance(i);
        if (c == '(')
        {
          paren_level += 1;
        }
        else if (c == ')')
        {
          paren_level -= 1;
        }
      }
      arg->name    = string8_range(start, &result.string.str[i]);
      arg->start_index = start_index;
      safe_advance(i);
      if (arg->name.size == 0)
      {
        hph_fatal(S("Unsupported empty args. Remove $() or add an argument."));
      }

      if (string8_match(arg->name, S("@time_now"), true))
      {
        arg->kind = Template_String_Variable_Time_Now;
      }
      else if (string8_match(arg->name, S("@index"), true))
      {
        arg->kind = Template_String_Variable_Index;
      }
      else
      {
        arg->kind = Template_String_Variable_Replace;
      }
    }
  }
#undef safe_advance

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