// @Hephaestus generated code 2025-October-10 14:07:24

// Forward declare UI structures
typedef struct UI_Node_Color_Scheme UI_Node_Color_Scheme;
typedef struct UI_Color_Scheme UI_Color_Scheme;
typedef struct UI_Node UI_Node;
typedef struct UI_Node_Cache UI_Node_Cache;
typedef struct UI_Signal UI_Signal;
typedef struct UI_Context UI_Context;

// Forward declare UI enums
typedef enum UI_Alignment_Kind UI_Alignment_Kind
typedef enum UI_Width_Kind UI_Width_Kind
typedef enum UI_Height_Kind UI_Height_Kind

typedef u64 UI_Node_Flags;
enum
{
  UI_Node_Flags_Mouse_Clickable = (1 << 0), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Display_Text = (1 << 1), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Draggable = (1 << 2), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Hoverable = (1 << 3), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Center_Text_Horizontally = (1 << 4), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Center_Text_Vertically = (1 << 5), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Dimensions_Wrap_Text = (1 << 6), /* TODO(fz): Add documentation */ 
};

typedef u64 UI_Node_Flags;
enum
{
  UI_Signal_Flags_Left_Down = (1 << 0), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Middle_Down = (1 << 1), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Right_Down = (1 << 2), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Left_Clicked = (1 << 3), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Middle_Clicked = (1 << 4), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Right_Clicked = (1 << 5), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Mouse_Hovered = (1 << 6), /* TODO(fz): Add documentation */
};

