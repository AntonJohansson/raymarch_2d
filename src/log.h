#pragma once

extern const char* red;
extern const char* green;
extern const char* yellow;
extern const char* blue;
extern const char* magenta;
extern const char* cyan;

extern void log_bold();
extern void log_color(const char* color);
extern void log_reset();

extern void log_info(const char* fmt, ...);
extern void log_error(const char* fmt, ...);
extern void log_warning(const char* fmt, ...);
