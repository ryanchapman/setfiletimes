# setfiletimes

Sample program that shows how to set file times in macOS / Darwin.

It can set the following times:

- create time (CRTIME)
- access time (ACCTIME)
- modification time (MODTIME)
- backup time (BKUPTIME)


You _cannot_ programmatically change:

- change time (CHGTIME)

# Compiling

```
$ gcc settimes.c -o settimes
$
```

# Example

Let's assume you have a file at /tmp/test.txt, and you want to set access, backup, create, and modification time to 
New Year's Day of 2008.  You would run

```
$ ./settimes -abcm -s "01/01/08 00:00:00 GMT" -f /tmp/test.txt
$
```

afterwards, you can verify with the built-in command `stat`

```
$ TZ=GMT stat -x /tmp/test.txt
  File: "/tmp/test.txt"
  Size: 0            FileType: Regular File
  Mode: (0644/-rw-r--r--)         Uid: (  501/rchapman)  Gid: (    0/   wheel)
Device: 1,16   Inode: 11888037    Links: 1
Access: Tue Jan  1 00:00:00 2008
Modify: Tue Jan  1 00:00:00 2008
Change: Mon Jul 24 07:35:20 2023
 Birth: Tue Jan  1 00:00:00 2008
```


# Usage

```
$ ./settimes -h
usage: ./settimes [-abcm] [-s datetime] -f <path>

  -a             set access time (ACCTIME)
  -b             set backup time (BKUPTIME)
  -c             set create time (CRTIME)
  -f <path>      full path to file you want to set times on
  -m             set modification time (MODTIME)
  -s <datetime>  set datetime on file to <datetime>, which must be in the format
                 "%m/%d/%y %H:%M:%S %Z"
                 for example, "02/14/09 05:04:00 GMT"

example:
  ./settimes -abcm -s "02/14/09 05:04:00 GMT-8" -f /tmp/test.txt
  would set ACCTIME, BKUPTIME, CRTIME and MODTIME to 02/14/09 05:04 PDT on the file /tmp/test.txt
  timezones like EDT do not work because of ambiguitity.  For more info, run "man strptime"

notes:
  to view all times for a file, you can use "stat -x /tmp/test.txt"
```

# Related

## SetFile

There is a tool in the Xcode Command Line Tools called `SetFile` which can set the creation and modification dates on a file.  
It cannot set access, backup or change time.

For more info, install Xcode Command Line Tools and run `man SetFile`

## GetFileInfo

Also included in the Xcode Command Line Tools is a program called `GetFileInfo`.  It can display creation and modification dates on a file.
It cannot display access, backup or change time.

For more info, install Xcode Command Line Tools and run `man GetFileInfo`


Example run:

```
$ GetFileInfo /tmp/test.txt
file: "/private/tmp/test.txt"
type: "\0\0\0\0"
creator: "\0\0\0\0"
attributes: avbstclinmedz
created: 12/31/2007 10:00:00
modified: 12/31/2007 10:00:00
```

