# prioriter

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9843c695746145b98569eaecd8bcd560)](https://app.codacy.com/gh/EmilyBourne/prioriter?utm_source=github.com&utm_medium=referral&utm_content=EmilyBourne/prioriter&utm_campaign=Badge_Grade_Settings)

System-tray based program to keep track of jobs and prioritise based on importance and urgence.

![Basic Prioriter Icon](/ExampleImages/PrioriterEmpty.png)

The icon will change depending upon the importance of the jobs within:

![Prioriter Icon when there are high priority/urgent jobs](/ExampleImages/PrioriterBar.png)

and will go red when a job becomes very very urgent:

![Prioriter Icon when jobs should definitely not be ignored](/ExampleImages/redPrioriter.png)

Jobs can also be edited from a window:

![The Edit Window](/ExampleImages/PrioriterWindow.png)

The program should be built with QT4 on linux (support for QT5 in development), QT5 has sufficient functionality on windows and mac (which should improve when the QT5 branch is merged).

The program has been tested on linux (QT4), windows (QT5) and mac (QT5). The build commands should be similar on all platforms, they are provided here for linux:

### Linux

```
mkdir build;

cd build;

qmake ../src/Prioriter.pro;

make
```

### Adding the program to the start menu (linux)

On linux the program can be added to the start menu by creating a document named "prioriter.desktop" in the folder ~/.local/share/applications . The contents should be:

```
[Desktop Entry]
Name=Prioriter
Comment=System tray based app to keep track of jobs and prioritise based on importance and urgence.
Exec= XXX/build/prioriter
Icon= XXX/images/128x128.ico
Terminal=false
Type=Application
Categories=Accessories;
```

be sure to replace XXX with the folder where prioriter was downloaded. The file should have permission to be executed. This can be done by modifying the properties or in the terminal with the command

```
chmod +x prioriter.desktop
```

### Starting the program on start-up (linux)

The program can be set to run on start-up by adding another .desktop file to the folder ~/.config/autostart . In this case it is advised to add an additional parameter to the exec line "-start_hidden" so that the edit window does not also open on startup. The desktop file should therefore look as follows:

```
[Desktop Entry]
Name=Prioriter
Comment=System tray based app to keep track of jobs and prioritise based on importance and urgence.
Exec= XXX/build/prioriter -start_hidden
Icon= XXX/images/128x128.ico
Terminal=false
Type=Application
Categories=Accessories;
```
