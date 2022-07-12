#include "main.h"

// TODO: How to handle document titles?

int main(int argc, String argv[])
{
  ParseFlags(argc, argv);

  if (FlagIsSet(f_help))
  {
    PrintAppUsage();
  }
  else if (FlagIsSet(f_version))
  {
    PrintAppVersion();
  }
  else if (f_args.count > 0)
  {
    ConvertMarkdownToHTML();
  }
  else
  {
    errorf("%s error: no input file or directory provided. Type '%s --help' for app usage.", APP_NAME, APP_NAME);
    return 1;
  }
  return 0;
}

void ParseFlags(int argc, String argv[])
{
  fp = InitFlagParser();
  f_help = NewBoolFlag(&fp, "help", 'h', "Show helpful instructions.");
  f_version = NewBoolFlag(&fp, "version", 'v', "Show the current app version.");
  f_out_dir = NewStrFlag(&fp, "out-dir", '\0', "Output directory path.", 0);
  f_css = NewStrFlag(&fp, "css", '\0', "Add CSS file to the head of the document.", 1);
  f_script = NewStrFlag(&fp, "script", '\0', "Add script file to the head of the document.", 1);
  f_css_inject = NewStrFlag(&fp, "css-inject", '\0', "Inject the contents of a CSS file into the document directly.", 1);
  f_script_inject = NewStrFlag(&fp, "script-inject", '\0', "Inject the contents of a JS file into the document directly.", 1);
  f_enable_hljs = NewBoolFlag(&fp, "enable-hljs", '\0', "Include highlight.js in the head of the document.");
  f_hljs_style = NewStrFlag(&fp, "hljs-style", '\0',  "Which highlight.js style to use.", 0);
  f_args = ParseArgs(&fp, argc, argv);
}

void PrintAppUsage()
{
  printf(
    "\n"
    "%s: %s\n"
    "\n"
    "Basic usage: %s [options...] file... [dir...]\n"
    "\n"
    "%s can be used to convert single Markdown files or entire directories into HTML.\n"
    "Custom styles and scripts can be added to design the documents to your liking.\n"
    "\n"
    "Common flags:\n"
    "\n"
    "  -h, --help                 Show helpful information.\n"
    "  -v, --version              Show current app version.\n"
    "\n"
    "Compilation options:\n"
    "\n"
    "  --out-dir        <path>    The path where to save the output.\n"
    "                             If the path does not exist, it will be created.\n"
    "\n"
    "  --css            <href>    Include a remote CSS file in the document.\n"
    "  --css-inject     <path>    Read the contents of a CSS file and inject the contents to the document.\n"
    "                             A <link rel=\"stylesheet\"> tag will be created for the JavaScript in the <head> of the document.\n"
    "\n"
    "  --script         <src>     Include a remote JavaScript file in the document.\n"
    "  --script-inject  <path>    Read the contents of a JavaScript file and inject the contents to the document.\n"
    "                             A <script> tag will be created for the JavaScript in the <head> of the document.\n"
    "\n"
    "  --enable-hljs              Enable highlight.js to enable code highlighting.\n"
    "                             This will add the needed scripts in the HTML document.\n"
    "\n"
    "  --hljs-style     <style>   The highlight.js style to use in code highlighting.\n"
    "                             E.g. github-dark, base16/darcula etc. (without the .css file extension!)\n"
    "                             Refer to https://github.com/highlightjs/highlight.js/tree/main/src/styles for a list of styles.\n"
    "\n"
    , APP_NAME, APP_DESCRIPTION
    , APP_NAME, APP_NAME
  );
}

void PrintAppVersion()
{
  printf("%s\n", APP_VERSION);
}

void ConvertMarkdownToHTML()
{
  String out_dir_path;
  if (FlagIsSet(f_out_dir))
  {
    out_dir_path = FlagStrVal(f_out_dir, 0);
  }
  else
  {
    out_dir_path = "out";
  }
  if (!fs_path_exists(out_dir_path))
  {
    // TODO: The entire directory path should be created
    // in case of a path that contains more than 1 non-existant directory.
    if (!fs_create_dir(out_dir_path))
    {
      debugf("%s requires multiple directories to be created. This is not supported (yet).\n", out_dir_path);
      return;
    }
  }
  for (int i = 0; i < f_args.count; i++)
  {
    if (fs_is_file(f_args.values[i]))
    {
      ConvertFile(f_args.values[i], out_dir_path);
    }
    else
    {
      ConvertDirectory(f_args.values[i], out_dir_path);
    }
  }
}

void ConvertFile(const String input_file_path, const String out_dir_path)
{
  FILE *output_file;
  String output_file_basename, output_file_path;
  String input_file_basename, input_file_data;
  int result, i, len;

  // Resolve the output file path.
  input_file_basename = fs_basename(input_file_path);
  len = strlen(input_file_basename);
  output_file_basename = malloc(sizeof(char) * (len + 3));
  memset(output_file_basename, '\0', len + 3);
  strcpy(output_file_basename, input_file_basename);
  output_file_basename[len - 2] = 'h';
  output_file_basename[len - 1] = 't';
  output_file_basename[len] = 'm';
  output_file_basename[len + 1] = 'l';
  output_file_path = fs_path_join(2, out_dir_path, output_file_basename);

  output_file = fopen(output_file_path, "w");
  if (output_file == NULL)
  {
    errorf("Could not open output file!\n");
    return;
  }

  // Generate a prettier document title
  String without_extension = RemoveExtension(input_file_basename);
  String title = ToTitle(without_extension);
  fprintf(output_file, TMPLT_HTML_TOP, title);
  free(without_extension);
  free(title);

  // Add remote CSS
  if (FlagIsSet(f_css)) for (i = 0; i < f_css->value_count; i++)
  {
    fprintf(output_file, TMPLT_CSS, FlagStrVal(f_css, i));
  }

  // Add remote JavaScript
  if (FlagIsSet(f_script)) for (i = 0; i < f_script->value_count; i++)
  {
    fprintf(output_file, TMPLT_SCRIPT, FlagStrVal(f_script, i));
  }

  // Add highlight.js scripts and and a style (default style if none provided)
  if (FlagIsSet(f_enable_hljs))
  {
    fwrite(TMPLT_HLJS_SCRIPTS, strlen(TMPLT_HLJS_SCRIPTS), 1, output_file);
    if (FlagIsSet(f_hljs_style))
    {
      fprintf(output_file, TMPLT_HLJS_CSS, FlagStrVal(f_hljs_style, 0));
    }
    else
    {
      fprintf(output_file, TMPLT_HLJS_CSS, "default");
    }
  }

  // Inject CSS
  if (FlagIsSet(f_css_inject)) for (i = 0; i < f_css_inject->value_count; i++)
  {
    String path = FlagStrVal(f_css_inject, i);
    if (!fs_path_exists(path))
    {
      errorf("File does not exist: %s\n", path);
      continue;
    }
    String data = fs_read_file(path);
    if (data == NULL)
    {
      errorf("Failed to read file: %s\n", path);
      continue;
    }
    fprintf(output_file, TMPLT_CSS_INJECT, data);
    free(data);
  }

  // Inject JavaScript
  if (FlagIsSet(f_script_inject)) for (i = 0; i < f_script_inject->value_count; i++)
  {
    String path = FlagStrVal(f_script_inject, i);
    if (!fs_path_exists(path))
    {
      errorf("File does not exist: %s\n", path);
      continue;
    }
    String data = fs_read_file(path);
    if (data == NULL)
    {
      errorf("Failed to read file: %s\n", path);
      continue;
    }
    fprintf(output_file, TMPLT_SCRIPT_INJECT, data);
    free(data);
  }

  fprintf(output_file, TMPLT_HTML_MIDDLE);
  input_file_data = fs_read_file(input_file_path);
  result = md_html(input_file_data, strlen(input_file_data), OnConvertChunk, output_file, MD_PARSER_FLAGS, 0);
  if (result != 0)
  {
    errorf("Failed to convert MD to HTML!\n");
    return;
  }
  fwrite(TMPLT_HTML_BOTTOM, strlen(TMPLT_HTML_BOTTOM), 1, output_file);
  fclose(output_file);
  free(input_file_data);
  free(output_file_path);
}

void ConvertDirectory(String input_dir_path, const String out_dir_path)
{
  String path, out_dir;
  DIR *dir;
  struct dirent *dirent;

  out_dir = fs_path_join(2, out_dir_path, fs_basename(input_dir_path));
  if (!fs_path_exists(out_dir))
  {
    if (!fs_create_dir(out_dir))
    {
      errorf("ConvertDirectory: Could not create directory!\n");
    }
  }

  if ((dir = opendir(input_dir_path)) == NULL)
  {
    errorf("ConvertDirectory: Could not open directory!\n");
    return;
  }

  while ((dirent = readdir(dir)) != NULL)
  {
    // Ignore directories that start with '.'
    if (dirent->d_name[0] == '.') continue;
    path = fs_join(input_dir_path, dirent->d_name);
    if (fs_is_dir(path))
    {
      ConvertDirectory(path, out_dir);
    }
    else if (IsAMarkdownFile(path))
    {
      ConvertFile(path, out_dir);
    }
    else
    {
      // TODO: Copy the file to the new destination
      debugf("%s should be copied over to %s. This is not yet supported.", path, out_dir);
    }
  }

  closedir(dir);
}

void OnConvertChunk(const String chunk, uint size, any user_data)
{
  FILE *output_file = (FILE *) user_data;
  fwrite(chunk, size, sizeof(char), output_file);
}