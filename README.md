#Introduction

A basic command shell to run executables in PATH with basic connector functionality. This project was created as an assignment for Mike Izbitchy's Spring 2015 CS100 course at UC Riverside.

See <https://github.com/mikeizbicki/ucr-cs100/tree/2015spring/assignments/hw/hw0-rshell> for further details.

#Installation

````
$ git clone https://github.com/cdixi001/rshell.git
$ cd rshell
$ git checkout hw0
$ make
$ bin/rshell
````

#Usage

Just like a normal terminal, commands are of the following form:

```
cmd 	= executable [ argumentList ] [ connector cmd ]
connector = || or && or ;
```
Exit with the command 'exit'

For example the following runs ls -a and pwd, does not run ps, and exits.

```
username§host: ls -a && pwd || ps ; exit
```

Extra spacing has no effect on executing commands. Host name is limited to 32 characters to save space.

##Connectors

Valid connectors are `||` (or), `&&` (and), and `;` (new command).

Any other symbols will be read as part of a command. Errors resulting from such symbols will be part of regular command error outputs.

Connectors are read from left to right. A blank or empty expression is evaluated to true;

###Examples

```
$ ls -a || pwd
$ asdfasdf || ps
```
All files are listed. An error message for asdfasdf (an invalid executable) is shown and all current processes are listed.

```
$ ls -a && pwd
$ adsfsdf && ps
& ps && asdfasdf
```

All files are listed and currently directory is displayed. An error message for adsfsdf is shown. Current processes are displayed.

```
$ adfsdf ; pwd
```
An error message is shown and the current working directory is displayed.

Same as
```
$ adfsdf
$ pwd
```

Multiple Connectors

```
$ && adsfsdf || ls && pwd || izbickisucks ; ps
```

An error message is shown. The contents of the current directory are displayed. The current working directory is displayed. The current processes are displayed.

#Known Issues

None currently. If you find any, let me know. 

#Works Cited(ish)

Mike Izbitchy <https://github.com/mikeizbicki>

Peter "Donut Boy" Hung <https://github.com/phung001>

<http://stackoverflow.com/questions/4034750>

<http://cplusplus.com/reference/string/string/c_str>

<http://github.com/mikeizbicki/ucr-cs100/blob/2015spring/textbook/assignment-help/syscalls/exec.md>
