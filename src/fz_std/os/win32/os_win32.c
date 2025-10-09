///////////////////////////////////////////////////////
// @Section: System info
///////////////////////////////////////////////////////
// @Section: Process Info
///////////////////////////////////////////////////////
// @Section: Process Launch Parameters
///////////////////////////////////////////////////////
// @Section: Memory
function OS_Memory_Stats
os_memory_stats()
{
  OS_Memory_Stats result = {0};

  MEMORYSTATUSEX status = {0};
  status.dwLength = sizeof(status);

  if(GlobalMemoryStatusEx(&status))
  {
    result.total_physical     = (u64)status.ullTotalPhys;
    result.available_physical = (u64)status.ullAvailPhys;
    result.total_virtual      = (u64)status.ullTotalPageFile;
    result.used_virtual       = (u64)(status.ullTotalPageFile - status.ullAvailPageFile);
  }

  return result;
}

function void*
os_memory_reserve(u64 size)
{
  void *result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
  return result;
}

function b32
os_memory_commit(void *ptr, u64 size)
{
  b32 result = (VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != 0);
  return result;
}

function void
os_memory_decommit(void *ptr, u64 size)
{
  VirtualFree(ptr, size, MEM_DECOMMIT);
}

function void
os_memory_release(void *ptr, u64 size)
{
  // NOTE(fz): Size not needed for windows. Though required for other OS's.
  VirtualFree(ptr, 0, MEM_RELEASE);
}

function u64
os_memory_get_page_size()
{
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  u64 result = (u64)info.dwPageSize;
  return result;
}

function void
os_message_box(String8 title, String8 text)
{
  Scratch scratch = scratch_begin(0, 0);
  MessageBoxA(NULL, cstring_from_string8(scratch.arena, text), cstring_from_string8(scratch.arena, title), MB_OK);
  scratch_end(&scratch);
}

///////////////////////////////////////////////////////
// @Section: Console
function b32
os_console_init()
{
  b32 result = false;

  if (GetConsoleWindow() != NULL)
  {
    // Already attached to a console; no need to allocate a new one.
    result = false;
  }
  else if (AllocConsole())
  {
    // No console attached; allocate a new one.
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    result = true;
  }

  // Try to enable color
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (handle != INVALID_HANDLE_VALUE)
  {
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode))
    {
      if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == 0)
      {
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
      }
    }
  }

  return result;
}

function void
os_console_write(String8 string)
{
  printf(S_FMT, (s32)string.size, string.str);
}

function void
os_console_write_line(String8 string)
{

}

function void
os_console_writef(char* fmt, ...)
{

}

function void
os_console_set_text_color(u8 r, u8 g, u8 b)
{

}

function void
os_console_reset_text_color()
{

}

function String8
os_console_read_line(Arena* arena)
{

}

function b32
os_console_has_input()
{

}

///////////////////////////////////////////////////////
// @Section: Files
function b32
os_file_create(String8 path)
{
  Scratch scratch = scratch_begin(0,0);
  u8* cpath = cstring_from_string8(scratch.arena, path);
  HANDLE file = CreateFileA(cpath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  b32 result = (file != INVALID_HANDLE_VALUE);
  if(result)
  {
    CloseHandle(file);
  }
  scratch_end(&scratch);
  return result;
}

function b32
os_file_delete(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  b32 result = DeleteFileA(cpath);
  scratch_end(&scratch);
  return result;
}

function b32
os_file_exists(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  DWORD attr = GetFileAttributesA(cpath);
  scratch_end(&scratch);
  b32 result = (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
  return result;
}

function u32
os_file_overwrite(String8 path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  HANDLE file = CreateFileA(cpath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);

  if(file == INVALID_HANDLE_VALUE)
  {
    return 0;
  }

  DWORD written = 0;
  WriteFile(file, data, (DWORD)data_size, &written, 0);
  CloseHandle(file);
  return (u32)written;
}

u32
os_file_append(String8 path, u8* data, u64 data_size)
{
  Scratch scratch = scratch_begin(0, 0);

  u8 *cpath = cstring_from_string8(scratch.arena, path);

  HANDLE file = CreateFileA(
    (char*)cpath,
    FILE_APPEND_DATA,
    FILE_SHARE_READ,
    0,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    0);

  if (file == INVALID_HANDLE_VALUE)
  {
    DWORD err = GetLastError();

    LPSTR msg_buf = NULL;
    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS;

    FormatMessageA(flags,
                   NULL,
                   err,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&msg_buf,
                   0,
                   NULL);

    if (msg_buf)
    {
      // trim trailing CR/LF
      size_t mlen = strlen(msg_buf);
      while (mlen > 0 && (msg_buf[mlen - 1] == '\n' || msg_buf[mlen - 1] == '\r'))
      {
        msg_buf[--mlen] = 0;
      }
    }

    emit_error(Sf(scratch.arena,
                  "CreateFileA failed for '%s' (err=%u). msg: %s",
                  (char*)cpath,
                  (unsigned)err,
                  (msg_buf ? msg_buf : "Unknown error")));

    if (msg_buf) LocalFree(msg_buf);
    scratch_end(&scratch);
    return 0;
  }


  // Move to end before append. Use SetFilePointerEx for 64-bit safety.
  LARGE_INTEGER zero = {0};
  if (!SetFilePointerEx(file, zero, NULL, FILE_END))
  {
    DWORD err = GetLastError();

    LPSTR msg_buf = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   err,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&msg_buf,
                   0,
                   NULL);

    if (msg_buf)
    {
      size_t mlen = strlen(msg_buf);
      while (mlen > 0 && (msg_buf[mlen - 1] == '\n' || msg_buf[mlen - 1] == '\r'))
      {
        msg_buf[--mlen] = 0;
      }
    }

    emit_warn(Sf(scratch.arena,
                 "SetFilePointerEx failed for '%s' (err=%u). msg: %s. Will attempt WriteFile anyway.",
                 (char*)cpath,
                 (unsigned)err,
                 (msg_buf ? msg_buf : "Unknown error")));

    if (msg_buf) LocalFree(msg_buf);
  }

  DWORD written = 0;
  BOOL ok = WriteFile(file, data, (DWORD)data_size, &written, NULL);
  if (!ok)
  {
    DWORD err = GetLastError();

    LPSTR msg_buf = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   err,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&msg_buf,
                   0,
                   NULL);

    if (msg_buf)
    {
      size_t mlen = strlen(msg_buf);
      while (mlen > 0 && (msg_buf[mlen - 1] == '\n' || msg_buf[mlen - 1] == '\r'))
      {
        msg_buf[--mlen] = 0;
      }
    }

    emit_error(Sf(scratch.arena,
                  "WriteFile failed for '%s' (err=%u). msg: %s",
                  (char*)cpath,
                  (unsigned)err,
                  (msg_buf ? msg_buf : "Unknown error")));

    if (msg_buf) LocalFree(msg_buf);
    CloseHandle(file);
    scratch_end(&scratch);
    return 0;
  }

  if ((u64)written != data_size)
  {
    emit_warn(Sf(scratch.arena,
                 "WriteFile wrote %u of %llu bytes for '%s'",
                 (unsigned)written,
                 (unsigned long long)data_size,
                 (char*)cpath));
  }

  CloseHandle(file);
  scratch_end(&scratch);
  return (u32)written;
}

function b32
os_file_wipe(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  HANDLE file = CreateFileA(cpath, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);

  if(file == INVALID_HANDLE_VALUE)
  {
    return 0;
  }

  SetFilePointer(file, 0, 0, FILE_BEGIN);
  b32 result = SetEndOfFile(file);
  CloseHandle(file);
  return result;
}

function u32
os_file_size(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  WIN32_FILE_ATTRIBUTE_DATA attr;
  b32 ok = GetFileAttributesExA(cpath, GetFileExInfoStandard, &attr);
  scratch_end(&scratch);

  if(!ok)
  {
    return 0;
  }

  ULARGE_INTEGER size;
  size.LowPart = attr.nFileSizeLow;
  size.HighPart = attr.nFileSizeHigh;
  return (u32)size.QuadPart;
}

function File_Data
os_file_load(Arena* arena, String8 path)
{
  File_Data result = {0};

  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  HANDLE file = CreateFileA(cpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);

  if(file == INVALID_HANDLE_VALUE)
  {
    return result;
  }

  DWORD file_size = GetFileSize(file, 0);
  u8 *buffer = push_array(arena, u8, file_size);
  DWORD read = 0;
  if(ReadFile(file, buffer, file_size, &read, 0) && read == file_size)
  {
    result.path = path;
    result.data.str = buffer;
    result.data.size = file_size;
  }

  CloseHandle(file);
  return result;
}

function u64
os_file_get_last_modified_time(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  HANDLE file = CreateFileA(cpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  scratch_end(&scratch);

  if(file == INVALID_HANDLE_VALUE)
  {
    return 0;
  }

  FILETIME ft;
  b32 success = GetFileTime(file, 0, 0, &ft);
  CloseHandle(file);

  if(!success)
  {
    return 0;
  }

  ULARGE_INTEGER time;
  time.LowPart  = ft.dwLowDateTime;
  time.HighPart = ft.dwHighDateTime;
  return time.QuadPart;
}

///////////////////////////////////////////////////////
// @Section: Directories

function b32
os_directory_create(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  b32 result = CreateDirectoryA(cpath, 0);
  b32 exists = (GetLastError() == ERROR_ALREADY_EXISTS);
  scratch_end(&scratch);
  return result || exists;
}

function b32
os_directory_delete(String8 path)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *cpath = cstring_from_string8(scratch.arena, path);
  b32 result = RemoveDirectoryA(cpath);
  scratch_end(&scratch);
  return result;
}

function b32
os_directory_move(String8 path, String8 destination)
{
  Scratch scratch = scratch_begin(0, 0);
  u8 *csrc = cstring_from_string8(scratch.arena, path);
  u8 *cdst = cstring_from_string8(scratch.arena, destination);
  b32 result = MoveFileA(csrc, cdst);
  scratch_end(&scratch);
  return result;
}

function String8  
os_directory_pop(String8 path)
{
  String8 result = S("");
  for(u64 i = path.size; i > 0; i -= 1)
  {
    if(path.str[i - 1] == '/' || path.str[i - 1] == '\\')
    {
      result = string8_slice(path, 0, i - 1);
      break;
    }
  }
  return result;
}

function String8  
os_directory_push(String8 path, String8 directory)
{
  Scratch scratch = scratch_begin(0, 0);
  b32 needs_slash = (path.size > 0 &&
                    path.str[path.size - 1] != '/' &&
                    path.str[path.size - 1] != '\\');

  String8 slash = S("/");
  String8 result;

  if(needs_slash)
  {
    String8 temp = string8_concat(scratch.arena, path, slash);
    result = string8_concat(scratch.arena, temp, directory);
  }
  else
  {
    result = string8_concat(scratch.arena, path, directory);
  }

  scratch_end(&scratch);
  return result;
}

///////////////////////////////////////////////////////
// @Section: Misc
function String8
os_executable_path(Arena* arena)
{
  String8 result = {0};

  u8 temp_path[MAX_PATH];
  DWORD length = GetModuleFileNameA(NULL, (char*)temp_path, MAX_PATH);

  if (length > 0 && length < MAX_PATH)
  {
    result.size = (u64)length;
    result.str = push_array(arena, u8, result.size);
    MemoryCopy(result.str, temp_path, result.size);
  }

  return result;
}

function b32
os_path_exists(String8 path)
{
  DWORD attr = GetFileAttributesA((u8*)path.str);
  b32 result = (attr != INVALID_FILE_ATTRIBUTES);
  return result;
}

function b32
os_path_is_absolute(String8 path)
{
  b32 result = false;

  if (path.size != 0 && 
      path.str[0] == '/' || path.str[0] == '\\' && 
      path.size > 2 && ((path.str[0] >= 'A' && path.str[0] <= 'Z') ||
                        (path.str[0] >= 'a' && path.str[0] <= 'z')) &&
      path.str[1] == ':')
  {
    result = true;
  }

  return false;
}

function String8
os_absolute_path_from_relative_path(Arena* arena, String8 relative_path)
{
  if (os_path_is_absolute(relative_path))
  {
    // Still allocate for consistency
    String8 result = string8_copy(arena, relative_path);
    return result;
  }

  String8 exe_path = os_executable_path(arena);

  // Find last slash to isolate directory
  u64 last_slash = 0;
  for (u64 i = 0; i < exe_path.size; i++)
  {
    if (exe_path.str[i] == '/' || exe_path.str[i] == '\\')
    {
      last_slash = i;
    }
  }

  String8 base_dir = {
    .size = last_slash,
    .str  = exe_path.str,
  };

  // Combine base dir + "/" + relative
  u64 tmp_size = base_dir.size + 1 + relative_path.size;
  u8 *tmp_str  = push_array(arena, u8, tmp_size + 1);
  memcpy(tmp_str, base_dir.str, base_dir.size);
  u64 offset = base_dir.size;

  if (offset > 0 && base_dir.str[offset-1] != '/')
  {
    tmp_str[offset++] = '/';
  }

  memcpy(tmp_str + offset, relative_path.str, relative_path.size);
  u64 combined_size = offset + relative_path.size;
  tmp_str[combined_size] = 0;

  // Normalize '\' -> '/'
  for (u64 i = 0; i < combined_size; i++)
  {
    if (tmp_str[i] == '\\') tmp_str[i] = '/';
  }

  // Tokenize and resolve "." and ".."
  u8 **parts = push_array(arena, u8*, combined_size/2 + 1);
  u64 parts_count = 0;

  u64 start = 0;
  for (u64 i = 0; i <= combined_size; i++)
  {
    if (i == combined_size || tmp_str[i] == '/')
    {
      u64 len = i - start;
      if (len > 0)
      {
        u8 *seg = tmp_str + start;

        if (len == 1 && seg[0] == '.')
        {
          // skip "."
        }
        else if (len == 2 && seg[0] == '.' && seg[1] == '.')
        {
          // go up one directory
          if (parts_count > 0) parts_count -= 1;
        }
        else
        {
          parts[parts_count++] = seg;
          seg[len] = 0; // null terminate segment
        }
      }
      start = i + 1;
    }
  }

  // Rebuild canonical path
  u64 final_size = 0;
  for (u64 i = 0; i < parts_count; i++)
  {
    final_size += strlen((char*)parts[i]) + 1; // +1 for '/'
  }

  String8 abs_path;
  abs_path.size = final_size;
  abs_path.str  = push_array(arena, u8, final_size + 1);

  u64 pos = 0;
  for (u64 i = 0; i < parts_count; i++)
  {
    u64 len = strlen((char*)parts[i]);
    memcpy(abs_path.str + pos, parts[i], len);
    pos += len;
    if (i+1 != parts_count)
    {
      abs_path.str[pos++] = '/';
    }
  }

  abs_path.size = pos;
  abs_path.str[pos] = 0;

  return abs_path;
}

function String8
os_get_appdata_dir(Arena* arena, String8 project_name)
{
  String8 result = {0};

  DWORD len = GetEnvironmentVariableA("APPDATA", 0, 0);
  if (len == 0) return result;

  u8* temp = push_array(arena, u8, len + 1);
  GetEnvironmentVariableA("APPDATA", (LPSTR)temp, len + 1);
  String8 appdata = string8_from_cstring(temp);

  // Build full path: %APPDATA%\project_name
  String8 full_path = string8_from_format(arena, "%S\\%S", appdata, project_name);

  // Create the directory if it doesn't exist
  os_directory_create(full_path);

  result = full_path;
  return result;
}


function void
os_exit_process(u32 code)
{
  ExitProcess(code);
}

///////////////////////////////////////////////////////
// @Section: OS Global State
///////////////////////////////////////////////////////
// @Section: Entry point (Windows specific functions behind entry_point)
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
  SetUnhandledExceptionFilter(&win32_exception_filter);
  _g_hInstance = hInstance;

  // Dynamically load windows functions which are not guaranteed in all SDKs
  {
    HMODULE module = LoadLibraryA("kernel32.dll");
    g_win32_set_thread_description_func = (W32_SetThreadDescription_Type *)GetProcAddress(module, "SetThreadDescription");
    FreeLibrary(module);
  }

  local_persist Thread_Context thread_context;
  thread_context_init_and_attach(&thread_context);

  main_thread_base_entry_point(__argc, __argv);
  return _g_application_return;
}

///////////////////////////////////////////////////////
// @Section: Error handling

function LONG WINAPI
win32_exception_filter(EXCEPTION_POINTERS* exception_ptrs)
{
  // TODO(fz): Implement
  os_exit_process(1);
  return 1;
}

function void
_win32_output_last_error(DWORD err)
{
  LPWSTR messageBuffer = NULL;
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

  if (messageBuffer)
  {
    // Convert wide string to narrow string (UTF-8 or ANSI) for OutputDebugStringA
    int size_needed = WideCharToMultiByte(CP_ACP, 0, messageBuffer, -1, NULL, 0, NULL, NULL);
    if (size_needed > 0)
    {
      char* messageA = (char*)HeapAlloc(GetProcessHeap(), 0, size_needed);
      if (messageA)
      {
        WideCharToMultiByte(CP_ACP, 0, messageBuffer, -1, messageA, size_needed, NULL, NULL);
        printf(": ");
        printf("%s\n", messageA);
        printf("\n");
        HeapFree(GetProcessHeap(), 0, messageA);
      }
    }
    LocalFree(messageBuffer);
  }
}

///////////////////////////////////////////////////////
// @Section: Timer

function void
os_time_init()
{
  QueryPerformanceFrequency(&g_win32_performance_frequency);
}

function u64
os_time_microseconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000000) / g_win32_performance_frequency.QuadPart;
}

function u64
os_time_milliseconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000) / g_win32_performance_frequency.QuadPart;
}

function f64
os_time_seconds()
{
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (f64)counter.QuadPart / (f64)g_win32_performance_frequency.QuadPart;
}

function u64
os_get_epoch_microseconds()
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  ULARGE_INTEGER uli = {ft.dwLowDateTime, ft.dwHighDateTime};
  return (uli.QuadPart - 116444736000000000ULL) / 10;
}

function OS_Date_Time
os_datetime_now()
{
  SYSTEMTIME st;
  GetLocalTime(&st);
  OS_Date_Time result =
  {
    st.wYear, st.wMonth, st.wDay,
    st.wHour, st.wMinute, st.wSecond,
    st.wMilliseconds
  };
  return result;
}

function String8
os_datetime_to_string8(Arena *arena, OS_Date_Time dt, b32 include_ms)
{
  local_persist u8* months[] = 
  {
    "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
  };
 
  String8 month_name = (dt.month >= 1 && dt.month <= 12) ? string8_from_cstring(months[dt.month - 1]) : S("Invalid");
  
  String8 result;
  if (include_ms)
  {
    result = string8_from_format(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u.%03u",
      dt.year, month_name.size, month_name.str, dt.day,dt.hour, dt.minute, dt.second, dt.millisecond);
  }
  else
  {
    result = string8_from_format(arena, "%04u-" S_FMT "-%02u %02u:%02u:%02u",
      dt.year, month_name.size, month_name.str, dt.day,dt.hour, dt.minute, dt.second);
  }
  
  return result;
}

function OS_Timer
os_timer_start()
{
  OS_Timer timer;
  QueryPerformanceCounter((LARGE_INTEGER*)&timer.opaque[0]);
  return timer;
}

function u64
os_timer_microseconds(OS_Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000000) / g_win32_performance_frequency.QuadPart;
}

function u64
os_timer_milliseconds(OS_Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return ((now.QuadPart - start) * 1000) / g_win32_performance_frequency.QuadPart;
}

function f64
os_timer_seconds(OS_Timer *timer)
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  u64 start = timer->opaque[0];
  return (f64)(now.QuadPart - start) / (f64)g_win32_performance_frequency.QuadPart;
}

function void
os_timer_reset(OS_Timer *timer)
{
  QueryPerformanceCounter((LARGE_INTEGER*)&timer->opaque[0]);
}

///////////////////////////////////////////////////////
// @Section: Cursor

function Vec2s32
os_cursor_window_get()
{
  Vec2s32 result;
  POINT point;
  GetCursorPos(&point);
  ScreenToClient(g_os_window.window_handle, &point);
  result.x = point.x;
  result.y = point.y;
  return result;
}

void os_cursor_set(Cursor_Type cursor)
{
  HCURSOR hCursor = NULL;

  _g_current_cursor = cursor;

  switch (cursor)
  {
    case Cursor_Arrow:        hCursor = LoadCursor(NULL, IDC_ARROW); break;
    case Cursor_Ibeam:        hCursor = LoadCursor(NULL, IDC_IBEAM); break;
    case Cursor_Wait:         hCursor = LoadCursor(NULL, IDC_WAIT); break;
    case Cursor_Crosshair:    hCursor = LoadCursor(NULL, IDC_CROSS); break;
    case Cursor_UpArrow:      hCursor = LoadCursor(NULL, IDC_UPARROW); break;
    case Cursor_SizeNWSE:     hCursor = LoadCursor(NULL, IDC_SIZENWSE); break;
    case Cursor_SizeNESW:     hCursor = LoadCursor(NULL, IDC_SIZENESW); break;
    case Cursor_SizeWE:       hCursor = LoadCursor(NULL, IDC_SIZEWE); break;
    case Cursor_SizeNS:       hCursor = LoadCursor(NULL, IDC_SIZENS); break;
    case Cursor_SizeALL:      hCursor = LoadCursor(NULL, IDC_SIZEALL); break;
    case Cursor_No:           hCursor = LoadCursor(NULL, IDC_NO); break;
    case Cursor_Hand:         hCursor = LoadCursor(NULL, IDC_HAND); break;
    case Cursor_AppStarting:  hCursor = LoadCursor(NULL, IDC_APPSTARTING); break;
    case Cursor_Help:         hCursor = LoadCursor(NULL, IDC_HELP); break;
    case Cursor_Pin:          hCursor = LoadCursor(NULL, IDC_PIN); break;
    case Cursor_Person:       hCursor = LoadCursor(NULL, IDC_PERSON); break;
    default:                  hCursor = LoadCursor(NULL, IDC_ARROW); break;
  }

  if (hCursor)
  {
    SetCursor(hCursor);
  }
}


function void
os_cursor_set_position(s32 x, s32 y)
{
  SetCursorPos(x, y);
}

function void
os_cursor_lock(Input_State* input, b32 lock)
{
  if (lock)
  {
    RECT rect;
    GetClientRect(g_os_window.window_handle, &rect);
    POINT center = {(rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2};
    ClientToScreen(g_os_window.window_handle, &center);
    SetCursorPos(center.x, center.y);

    input->_g_ignore_next_mouse_move = true;
    input->_g_is_cursor_locked       = true;

    // Reset deltas to avoid cursor jump
    input->mouse_current.delta.x  = 0.0f;
    input->mouse_current.delta.y  = 0.0f;
    input->mouse_previous.delta.x = 0.0f;
    input->mouse_previous.delta.y = 0.0f;
    MemoryCopyStruct(&(input->mouse_previous), &(input->mouse_current));
  }
  else
  {
    input->_g_is_cursor_locked = false;
  }
}

function void
os_cursor_hide(b32 hide)
{
  // Win32 quirk. It has an internal counter required to show the cursor.
  // The while loops just make sure it exhausts the counter and applies immediately.
  while (ShowCursor(hide ? FALSE : TRUE) >= 0 &&  hide);
  while (ShowCursor(hide ? FALSE : TRUE) < 0  && !hide);
}

///////////////////////////////////////////////////////
// @Section: Input-Keyboard

function u32 
_native_key_from_os_key(Keyboard_Key key)
{
  return _win32_key_table[key];
}

function Keyboard_Key 
_os_key_from_native_key(u32 native_key)
{
  for(u32 i = 0; i < Keyboard_Key_Count; ++i)
  {
    if(_win32_key_table[i] == native_key)
    {
      return (Keyboard_Key)i;
    }
  }
  return Keyboard_Key_Count; // invalid
}

///////////////////////////////////////////////////////
// @Section: Window Lifecycle
function b32
os_window_init(s32 width, s32 height, String8 title, Input_State* out_input)
{
  MemoryZeroStruct(&g_os_window);
  b32 result = true;

  g_os_window.window_handle = _win32_window_create(_g_hInstance, width, height, title);
  if (!IsWindow(g_os_window.window_handle))
  {
    win32_check_error();
    emit_error(S("Failed to get window handle\n"));
  }

  // Store a pointer to the input in the window, such that it is updated every message
  SetWindowLongPtr(g_os_window.window_handle, GWLP_USERDATA, (LONG_PTR)out_input);
  
  g_os_window.device_context = GetDC(g_os_window.window_handle);
  if (!g_os_window.device_context)
  {
    win32_check_error();
    emit_error(S("Failed to get device context"));
  }

  g_os_window.dimensions = (Vec2s32){width, height};
  g_os_window.title      = S("FZ_Window_Title");

  _input_init(out_input);
  g_os_resize_callback = _win32_window_resize_callback;

  return result;
}

function void
os_window_open()
{
  ShowWindow(g_os_window.window_handle, SW_SHOW);
  UpdateWindow(g_os_window.window_handle);
}

function void     
os_window_close()
{
  ShowWindow(g_os_window.window_handle, SW_HIDE);
  UpdateWindow(g_os_window.window_handle);
}

function b32
os_is_application_running(Input_State* input)
{
  b32 result = true;

  MSG msg = {0};
  if (g_os_window.window_handle != NULL)
  {
    _input_update(input);

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT) 
      {
        _g_application_return = (s32)msg.wParam;
        return false;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return result;
}

function OS_Window
os_window_get()
{
  return g_os_window;
}

function Vec2s32
os_window_get_client_dimensions()
{
  RECT rect;
  GetClientRect(g_os_window.window_handle, &rect);
  Vec2s32 result = vec2s32((rect.right - rect.left), (rect.bottom - rect.top));
  return result;
}

function Vec2s32
os_window_client_to_screen(Vec2s32 client_point)
{
  POINT point = { client_point.x, client_point.y };
  ClientToScreen(g_os_window.window_handle, &point);
  Vec2s32 result = vec2s32(point.x, point.y);
  return result;
}

///////////////////////////////////////////////////////
// @Section: Window Flags

function b32
os_window_is_fullscreen()
{
  RECT rect;
  GetWindowRect(g_os_window.window_handle, &rect);

  MONITORINFO mi = { sizeof(mi) };
  GetMonitorInfo(MonitorFromWindow(g_os_window.window_handle, MONITOR_DEFAULTTONEAREST), &mi);

  b32 result = (rect.left   == mi.rcMonitor.left &&
                rect.top    == mi.rcMonitor.top &&
                rect.right  == mi.rcMonitor.right &&
                rect.bottom == mi.rcMonitor.bottom);

  return result;
}

function void
os_window_set_fullscreen(b32 set)
{
  static WINDOWPLACEMENT prev = { sizeof(prev) };
  if (set)
  {
    GetWindowPlacement(g_os_window.window_handle, &prev);

    MONITORINFO mi = { sizeof(mi) };
    GetMonitorInfo(MonitorFromWindow(g_os_window.window_handle, MONITOR_DEFAULTTONEAREST), &mi);

    SetWindowLong(g_os_window.window_handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(g_os_window.window_handle, HWND_TOP,
                 mi.rcMonitor.left, mi.rcMonitor.top,
                 mi.rcMonitor.right - mi.rcMonitor.left,
                 mi.rcMonitor.bottom - mi.rcMonitor.top,
                 SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
  }
  else
  {
    SetWindowLong(g_os_window.window_handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    SetWindowPlacement(g_os_window.window_handle, &prev);
    SetWindowPos(g_os_window.window_handle, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                 SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
  }
}

function b32
os_window_is_maximized()
{
  b32 result = IsZoomed(g_os_window.window_handle);
  return result;
}

function void
os_window_set_maximized(b32 set)
{
  ShowWindow(g_os_window.window_handle, set ? SW_MAXIMIZE : SW_RESTORE);
}

function b32
os_window_is_minimized()
{
  b32 result = IsIconic(g_os_window.window_handle);
  return result;
}

function void
os_window_set_minimized(b32 set)
{
  ShowWindow(g_os_window.window_handle, set ? SW_MINIMIZE : SW_RESTORE);
}

function void
os_swap_buffers()
{
  SwapBuffers(g_os_window.device_context);
}

///////////////////////////////////////////////////////
// @Section: Window Appearance

function void
os_window_set_visible(b32 visible)
{
  ShowWindow(g_os_window.window_handle, visible ? SW_SHOW : SW_HIDE);
}

function b32
os_window_set_title(String8 title)
{
  Scratch scratch = scratch_begin(0, 0);
  char* ctitle = cstring_from_string8(scratch.arena, title);
  b32 result = SetWindowTextA(g_os_window.window_handle, ctitle);
  win32_check_error();
  // TODO(fz): Add title to g_os_window
  scratch_end(&scratch);
  return result;
}

function void
os_window_clear_custom_border_data()
{

}

function void
os_window_push_custom_title_bar(f32 thickness)
{

}

function void
os_window_push_custom_edges(f32 thickness)
{

}

function void
os_window_push_custom_title_bar_client_area()
{

}

function void
os_window_set_position(Vec2f32 pos)
{
  SetWindowPos(g_os_window.window_handle, 0,
               (int)pos.x, (int)pos.y, 0, 0,
               SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

function void
os_window_set_size(s32 width, s32 height)
{
  SetWindowPos(g_os_window.window_handle, 0,
               0, 0, width, height,
               SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
}

///////////////////////////////////////////////////////
// @Section: Win32

LRESULT CALLBACK 
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  Input_State* input = (Input_State*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  switch (message) 
  {
    case WM_SETCURSOR:
    {
      if (LOWORD(lParam) == HTCLIENT) 
      {
        os_cursor_set(_g_current_cursor);
        return true;
      }
    }
    break;

    case WM_SIZE: 
    {
      g_os_resize_callback(LOWORD(lParam), HIWORD(lParam));
      return 0;
    }
    break;

    // Keyboard keys
    case WM_KEYDOWN: 
    {
      Keyboard_Key key = _os_key_from_native_key((u32)wParam);
      if (key < Keyboard_Key_Count) 
      {
        _input_process_keyboard_key(input, (Keyboard_Key)key, true);
      }
      else
      {
        // TODO(fz): Handle error
      }
      return 0;
    }
    break;
    case WM_KEYUP: 
    {
      Keyboard_Key key = _os_key_from_native_key((u32)wParam);
      if (key < Keyboard_Key_Count) 
      {
        _input_process_keyboard_key(input, (Keyboard_Key)key, false);
      }
      else
      {
        // TODO(fz): Handle error
      }
      return 0;
    }
    break;

    // Mouse Cursor
    case WM_MOUSEMOVE: 
    {
      if (input->_g_ignore_next_mouse_move) 
      {
        input->_g_ignore_next_mouse_move = false;
        return 0;
      }
      s32 x = LOWORD(lParam);
      s32 y = HIWORD(lParam);
      _input_process_mouse_cursor(input, (f32)x, (f32)y);
      return 0;
    }
    break;
    
    // Mouse Buttons
    case WM_LBUTTONDOWN: 
    {
      _input_process_mouse_button(input, Mouse_Button_Left, true);
      return 0;
    }
    break;
    case WM_LBUTTONUP: 
    {
      _input_process_mouse_button(input, Mouse_Button_Left, false);
      return 0;
    }
    break;
    case WM_RBUTTONDOWN: 
    {
      _input_process_mouse_button(input, Mouse_Button_Right, true);
      return 0;
    }
    break;
    case WM_RBUTTONUP: 
    {
      _input_process_mouse_button(input, Mouse_Button_Right, false);
      return 0;
    }
    break;
    case WM_MBUTTONDOWN: 
    {
      _input_process_mouse_button(input, Mouse_Button_Middle, true);
      return 0;
    }
    break;
    case WM_MBUTTONUP: 
    {
      _input_process_mouse_button(input, Mouse_Button_Middle, false);
      return 0;
    }
    break;

    case WM_CLOSE:
    {
      DestroyWindow(hWnd);
      return 0;
    }
    break;

    case WM_DESTROY: 
    {
      ReleaseDC(hWnd, g_os_window.device_context);
      PostQuitMessage(0);
      return 0;
    }
    break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

function HWND
_win32_window_create(HINSTANCE hInstance, s32 width, s32 height, String8 title)
{
  WNDCLASSEXA wc = 
  {
    .cbSize        = sizeof(wc),
    .lpfnWndProc   = WndProc,
    .hInstance     = hInstance,
    .hIcon         = LoadIconA(NULL, MAKEINTRESOURCEA(IDI_APPLICATION)),
    .hCursor       = LoadCursorA(NULL, MAKEINTRESOURCEA(IDC_ARROW)),
    .lpszClassName = "FZ_Window_Class",
  };

  ATOM atom = RegisterClassExA(&wc);
  Assert(atom && "Failed to register window class");
    
  DWORD exstyle = WS_EX_APPWINDOW;
  DWORD style   = WS_OVERLAPPEDWINDOW;

  Scratch scratch = scratch_begin(0, 0);
  HWND result = CreateWindowExA(exstyle, wc.lpszClassName, cstring_from_string8(scratch.arena, title), style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, wc.hInstance, NULL);
  if (!result)
  {
    win32_check_error();
    emit_fatal(S("Error creating Win32 window."));
  }

  scratch_end(&scratch);
  return result;
}

function void
_win32_window_resize_callback(s32 width, s32 height)
{
  if (height == 0) height = 1;
  if (width == 0)  width  = 1;
  g_os_window.dimensions.x = width;
  g_os_window.dimensions.y = height;
  glViewport(0, 0, width, height);
}