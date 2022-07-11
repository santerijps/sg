const char TMPLT_HTML_TOP[] =
  "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<meta charset=\"UTF-8\">\n"
  "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
  "<title>%s</title>\n"
;

const char TMPLT_HTML_MIDDLE[] =
  "</head>\n"
  "<body>\n"
;

const char TMPLT_HTML_BOTTOM[] =
  "</body>\n"
  "</html>\n"
;

const char TMPLT_CSS[] = "<link rel=\"stylesheet\" href=\"%s\"/>\n";
const char TMPLT_SCRIPT[] = "<script src=\"%s\"></script>\n";
const char TMPLT_CSS_INJECT[] = "<style>\n%s\n</style>\n";
const char TMPLT_SCRIPT_INJECT[] = "<script>\n%s\n</script>\n";

const char TMPLT_HLJS_CSS[] =
  "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.5.1/styles/%s.min.css\"/>\n"
;

const char TMPLT_HLJS_SCRIPTS[] =
  "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.5.1/highlight.min.js\"></script>\n"
  "<script>hljs.highlightAll();</script>\n"
;