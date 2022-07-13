# sg (name still in progress)

`sg` is a static site generator implemented in the C programming language.

## Usage

```txt

sg: A static site generator implemented in the C programming language.

Basic usage: sg [options...] file... [dir...]

sg can be used to convert single Markdown files or entire directories into HTML.
Custom styles and scripts can be added to design the documents to your liking.

Common flags:

  -h, --help                 Show helpful information.
  -v, --version              Show current app version.

Compilation options:

  --out-dir        <path>    The path where to save the output.
                             If the path does not exist, it will be created.

  --css            <href>    Include a remote CSS file in the document.
  --css-inject     <path>    Read the contents of a CSS file and inject the contents to the document.
                             A <link rel="stylesheet"> tag will be created for the JavaScript in the <head> of the document.

  --script         <src>     Include a remote JavaScript file in the document.
  --script-inject  <path>    Read the contents of a JavaScript file and inject the contents to the document.
                             A <script> tag will be created for the JavaScript in the <head> of the document.

  --enable-hljs              Enable highlight.js to enable code highlighting.
                             This will add the needed scripts in the HTML document.

  --hljs-style     <style>   The highlight.js style to use in code highlighting.
                             E.g. github-dark, base16/darcula etc. (without the .css file extension!)
                             Refer to https://github.com/highlightjs/highlight.js/tree/main/src/styles for list of styles.
```

## Setting up

```txt
git clone https://github.com/santerijps/sg
cd sg
git submodule --init
make
```
