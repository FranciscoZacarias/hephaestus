// @Hephaestus generated code 2025-October-09 13:23:32 

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


// Row 3
typedef struct size_y size_y;
struct size_y_stack
{
  f32 data[32];
  u32 top_index;
  f32 bottom_val;
};


// Row 4
typedef struct spacing_y spacing_y;
struct spacing_y_stack
{
  f32 data[32];
  u32 top_index;
  f32 bottom_val;
};


// Row 5
typedef struct alignment_kind alignment_kind;
struct alignment_kind_stack
{
  UI_Alignment_Kind data[32];
  u32 top_index;
  UI_Alignment_Kind bottom_val;
};


// Row 6
typedef struct width_kind width_kind;
struct width_kind_stack
{
  UI_Width_Kind data[32];
  u32 top_index;
  UI_Width_Kind bottom_val;
};


// Row 7
typedef struct height_kind height_kind;
struct height_kind_stack
{
  UI_Height_Kind data[32];
  u32 top_index;
  UI_Height_Kind bottom_val;
};


// Row 8
typedef struct node_color_scheme node_color_scheme;
struct node_color_scheme_stack
{
  UI_Node_Color_Scheme data[32];
  u32 top_index;
  UI_Node_Color_Scheme bottom_val;
};


// Row 9
typedef struct border_color border_color;
struct border_color_stack
{
  Color data[32];
  u32 top_index;
  Color bottom_val;
};


// Row 10
typedef struct background_color background_color;
struct background_color_stack
{
  Color data[32];
  u32 top_index;
  Color bottom_val;
};


