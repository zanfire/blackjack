#ifndef CONSTANTS_HPP__
#define CONSTANTS_HPP__

// Escape sequences.
constexpr char underline[] = {0x1b,'[','4',';','3','9','m',0};
constexpr char bold_green[] = {0x1b,'[','1',';','3','2','m',0};
constexpr char bold_red[] = {0x1b,'[','1',';','3','1','m',0};
constexpr char bold_yellow[] = {0x1b,'[','1',';','3','3','m',0};
constexpr char normal[] = {0x1b,'[','0',';','3','9','m',0};
// Unicodes.
constexpr char action_mark[] = "\xE2\x8C\x9B";

#endif