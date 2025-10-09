# Hephaestus 

Hephaestus is a table driven text generator heavily inspired by [this article]([https://github.com/ryanfleury/metadesk](https://www.rfleury.com/p/table-driven-code-generation)). 

This document describes the structure and semantics of the Hephaestus DSL used to generate source code.  
The language is designed to define configuration, structured data tables, and generation templates for automatic code output.

---

## 1. Global Scopes

### `@config`
Defines global configuration parameters for the generation process.  
All configuration variables defined here can be referenced anywhere in the file.

**Example:**
```c
@config
{
  @output_file_name `test_file.c`
  @output_file_path `../src/generated/`
}
```
**Supported keys:**
- `@output_file_name` → Name of the file to generate.
- `@output_file_path` → Relative or absolute path where the file will be placed.

More configuration variables can be added in the future.

---

### `@table`
Defines a structured table of rows, each containing multiple columns.  
This is typically used to define a set of repeated data patterns that can later be iterated over during generation.

**Example:**
```c
@table // Defines stacks for different types
Stack_Table (name type default_value max)
{
  { node              UI_Node*             (UI_Node){0}                   32 }
  { top_left          Vec2f32              `vec2f32(0.0f, 0.0f)`          32 }
  { size_x            f32                   g_os_window.dimensions.x      32 }
  { size_y            f32                   g_os_window.dimensions.y      32 }
  { spacing_y         f32                   0.0f                          32 }
  { alignment_kind    UI_Alignment_Kind    UI_Alignment_Kind_Y            32 }
  { width_kind        UI_Width_Kind        UI_Width_Kind_Fill             32 }
  { height_kind       UI_Height_Kind       UI_Height_Kind_Fill            32 }
  { node_color_scheme UI_Node_Color_Scheme ui_context.color_scheme.window 32 }
  { border_color      Color                PURPLE(1)                      32 }
  { background_color  Color                PURPLE(1)                      32 }
}
```
- `Stack_Table` is the table name.
- `(name type default_value max)` are the column headers.
- Each `{ ... }` inside defines a row.

---

### `@generate`
Defines the output template. Can contain static text and dynamic interpolations.  
Supports looping through table rows and inserting variable values.

**Example:**
```c
@generate
{
  `// @Hephaestus generated code $(@time_now) \n\n`
  @foreach(Stack_Table) `// Row $(@index)\ntypedef struct $(name) $(name);\nstruct $(name)_stack\n{\n  $(type) data[$(max)];\n  u32 top_index;\n  $(type) bottom_val;\n};\n\n\n`
}
```

- Static text is enclosed in backticks.
- `@foreach(TableName)` repeats the inner string for each row of the table.
- Variable substitution uses `$()`.

---

## 2. Implicit Variables

Certain variables are always available and resolve automatically:

| Variable         | Description                               | Example Output                                 |
|-------------------|--------------------------------------------|-----------------------------------------------|
| `@index`          | Current index during a `foreach`          | `0`, `1`, `2`, ...                            |
| `@time_now`       | Current timestamp                         | `2025-October-08 14:27:56` |

Additional implicit variables can be added in the future without changing the language structure.

---

## 3. Variable Substitution and Operations

You can reference table columns or config values with `$(variable)` inside the `@generate` scope.  
You can apply numeric or string offsets with `+` or `-`.

### Numeric Expansion
If the variable expands to a number:
```
// Where $(max) expands to 32.

$(max+1) → 33
$(max-1) → 32
```

### String Expansion
If the variable expands to a string:
- `+N` skips N characters from the start.  
- `-N` truncates N characters from the end.

```
// Where $(name) expands to "Node"

$(name)+1 → "ode"
$(name)-1 → "Nod"
```

---

## 4. Generation Example

**Input DSL:**
```c
@generate
{
  `// @Hephaestus generated code $(@time_now) \n\n`
  @foreach(Stack_Table) `// Row $(@index)\ntypedef struct $(name) $(name);\nstruct $(name)_stack\n{\n  $(type) data[$(max)];\n  u32 top_index;\n  $(type) bottom_val;\n};\n\n\n`
}
```

**Generated Output:**
```c
// @Hephaestus generated code 2025-October-08 15:53:23 

// Row 0
typedef struct node node;
struct node_stack
{
  UI_Node* data[32];
  u32 top_index;
  UI_Node* bottom_val;
};

// Row 1
typedef struct top_left top_left;
struct top_left_stack
{
  Vec2f32 data[32];
  u32 top_index;
  Vec2f32 bottom_val;
};

// Row 2
typedef struct size_x size_x;
struct size_x_stack
{
  f32 data[32];
  u32 top_index;
  f32 bottom_val;
};

...
```
Each table row generates a new struct block.


