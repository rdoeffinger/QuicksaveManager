# Quicksave Manager

## Compiling

Make sure you have Qt5 and its development libraries installed.
Run "qmake" and then "make".
You can also use QtCreator to compile it.

## Disclaimer

Note: This is an early version, it might have bugs, and
due to the way it works (it simply takes a copy of the files
one by one) the backups might occasionally be corrupted
if the save files are modified in quick succession for example.

## Usage

Click "browse..." button under "backup path" to set the path
in which the screenshots and backups will be saved.

Click "browse..." button under "savegame location" and set the
path which contains the files you want to create automatic
snapshots/backups of.

Under "save pattern" enter the pattern of filenames that
should be watched and backed up. Wildcards like \* can be
used. Examples are \* for all files, quicksave\* for all files
starting with "quicksave".

Click "start" to start monitoring the specified files for changes.

Snapshots along with the screenshots captured will appear on the
list on the right side.

Double-click a screenshot to view it larger in a separate window.

Click "restore" to restore a selected backup.
