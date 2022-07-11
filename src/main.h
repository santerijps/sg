#pragma once

// TODO: Add include guards!
#include <stdio.h>
#include <libgen.h>

#include "util.h"
#include "template.h"
#include "modules/cfs/cfs.h"
#include "modules/clargs/clargs2.h"
#include "modules/md4c/src/md4c.h"
#include "modules/md4c/src/md4c-html.h"
#include "modules/md4c/src/entity.h"

typedef struct FlagParser FlagParser;
typedef struct Flag * FlagRef;
typedef struct OtherArgs Args;
typedef char * String;
typedef unsigned int uint;
typedef void * any;

const String APP_NAME = "sg";
const String APP_DESCRIPTION = "A static site generator implemented in the C programming language.";
const String APP_VERSION = "0.1.0";

FlagParser fp;
FlagRef f_help;
FlagRef f_version;
FlagRef f_out_dir;
FlagRef f_css;
FlagRef f_script;
FlagRef f_css_inject;
FlagRef f_script_inject;
FlagRef f_enable_hljs;
FlagRef f_hljs_style;
Args f_args;

#define MD_PARSER_FLAGS MD_FLAG_TABLES | MD_FLAG_TASKLISTS | MD_FLAG_PERMISSIVEURLAUTOLINKS | MD_FLAG_PERMISSIVEEMAILAUTOLINKS | MD_FLAG_PERMISSIVEWWWAUTOLINKS | MD_FLAG_LATEXMATHSPANS | MD_FLAG_STRIKETHROUGH | MD_DIALECT_GITHUB

void ParseFlags(int argc, String argv[]);
void PrintAppUsage();
void PrintAppVersion();

void ConvertMarkdownToHTML();
void ConvertFile(const String input_file_path, const String out_dir_path);
void ConvertDirectory(const String input_dir_path, const String out_dir_path);
void OnConvertChunk();
