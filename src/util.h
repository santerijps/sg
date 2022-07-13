#ifndef UTIL_H

#define UTIL_H

#ifndef _INC_STDLIB
  #include <stdlib.h>
#endif

#ifndef _INC_CTYPE
  #include <ctype.h>
#endif

#ifndef _INC_STRING
  #include <string.h>
#endif

// List of codes: https://i.stack.imgur.com/gpDPE.png
// TODO: Use different codes for Windows! These don't work in PowerShell!
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"
#define COLOR_INVERSE "\033[7m"
#define COLOR_TEXT_BLACK "\033[30m"

#define errorf(...) fprintf(stderr, __VA_ARGS__)
#define debugf(...) fprintf(stderr, "[%s:%d %s] ", __FILE__, __LINE__, __FUNCTION__); fprintf(stderr, __VA_ARGS__)

int IsAMarkdownFile(char *path)
{
  size_t l = strlen(path);
  return l > 3 && path[l - 3] == '.' && path[l - 2] == 'm' && path[l - 1] == 'd';
}

char* ToTitle(char *s)
{
  int capitalize_next_char = 1;
  size_t i, l = strlen(s);
  char *r = (char *) malloc(l + 1);
  for (i = 0; i < strlen(s); i++)
  {
    if (isalpha(s[i]))
    {
      if (capitalize_next_char)
      {
        r[i] = s[i] - 32;
        capitalize_next_char = 0;
        continue;
      }
    }
    else
    {
      capitalize_next_char = 1;
    }
    r[i] = s[i];
  }
  r[l] = '\0';
  return r;
}

char* RemoveExtension(char *s)
{
  size_t len = strlen(s);
  int should_write = 0;
  char *r = (char *) malloc(len + 1);
  memset(r, '\0', len + 1);
  while (len--)
  {
    if (should_write)
    {
      r[len] = s[len];
    }
    else
    {
      if (s[len] == '.')
      {
        should_write = 1;
      }
    }
  }
  return r;
}

char* ReplaceString(char *s, const char *search, const char *replace)
{
  size_t i, j;
  size_t s_len = strlen(s), search_len = strlen(search), replace_len = strlen(replace);
  short match;

  size_t ri, capacity = s_len;
  char *r = (char*) malloc(sizeof(char) * capacity);
  memset(r, 0, capacity);

  for (i = 0, ri = 0; i < s_len; i++, ri++)
  {
    match = 1;
    for (j = 0; j < search_len; j++)
    {
      if (s[i + j] != search[j])
      {
        match = 0;
        break;
      }
    }
    if (match == 1)
    {
      if ((ri + 1 + replace_len) >= capacity)
      {
        capacity *= 2;
        r = (char*) realloc(r, sizeof(char) * capacity);
      }
      strcat(r, replace);
      i += search_len - 1;
      ri += replace_len - 1;
    }
    else
    {
      r[ri] = s[i];
    }
  }

  for (i = ri; i < capacity; i++)
  {
    r[i] = '\0';
  }

  return r;
}

/**
 * Converts local links to .md files to .html files.
 * This is required to make links work after building.
*/
char* ConvertLocalLinks(char *s)
{
  // Look for "](anything.md)" and replace .md with .html
  return ReplaceString(s, ".md", ".html");
}

#endif