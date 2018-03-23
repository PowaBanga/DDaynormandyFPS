
Version: q2admin1.17.44-tsmod-2

This is Q2Admin with R1CH's security patches applied, and a couple tweaks
to prevent kicking of players for false positives (Q2Admin would think a
player was a bot when they were merely lagging badly.)

Original Q2Admin readme.txt follows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*******************************************************
*                                                     *
* Q2Admin v1.17 for Quake2                            *
* (c) Shane Powell 1999-2001                          *
*                                                     *
* Date: 18th August 2001                              *
*                                                     *
* Win32 / Linux                                       *
*                                                     *
* http://www.planetquake.com/q2admin/                 *
*                                                     *
*******************************************************


Q2Admin is a transparent proxy mod that adds many admin
functions plus ZBot/Ratbot detection (including hacked 
bots) to Quake2.  It works with all Quake2 mods transparently 
by filtering communication between the server and the primary
mod it is running on top of.



Contents:

1. Install.

1.1. Windows Install.

1.2. Linux Install.

1.3. Alpha Linux Install.

1.4. Solaris Install.

2. Admin Commands and Setup.

2.1. Admin Commands.

2.2. q2admin.txt setup.

2.3. Quake2 client admin.

2.4. Quake2 Server admin.

2.5. Admin command reference.

2.5.1 "[LIKE/RE/CL] name" explained.

2.6. Impulses explained.

2.7. Advanced admin commands.

2.7.1. Max Rate / Max fps.

2.7.2. IP / Nick Banning.

2.7.3. Limited rcon.

2.7.4. Chat Flood Protection and Muting.

2.8. play_ and say_ commands.

2.8.1 play_all.

2.8.2 play_team.

2.8.3 play_person.

2.8.4 say_person.

2.8.5 say_group.

2.8.6 extended say.

2.9. Message of the Day (MOTD).

2.10 disable spawning items.

2.11 command voting.

2.12 disable client commands.

2.13 cl_pitchspeed and cl_anglespeedkey (Action Quake2 M4 Scripts)

2.14 client side variable checking

3. String preprocessing.

4. Logging explained.

5. Regular expressions.

6. Trouble Shooting.

7. Credits.

8. Copyright and Distribution Permissions.

9. Release History.





1. Install
----------

1.1. Windows Install
--------------------

Installation for servers running Quake2 v3.20

1. Shut down the Quake2 Server.

2. Create a directory off the Quake2 root directory called 
'release'

3. Copy the gamex86.dll into the 'release' directory.

4. Copy the following files into the Quake2 root directory 
(wherever Quake2.exe is). 
    q2admin.txt
    q2adminban.txt
    q2adminlog.txt
    q2adminlrcon.txt
    q2adminflood.txt
    q2adminspawn.txt
    q2adminvote.txt
    q2admindisable.txt
    q2admincheckvar.txt

5. Start the server as usual.


The  Q2Admin  prints it's version number out just before 
the standard "==== InitGame ====" when the server starts.

Q2Admin also prints out warning messages if the config 
files can't be found.

To enable overflow detection you must set logfile to 2 or
3 on the server.  This is best done by adding it to your
server.cfg file.




Installation for servers running Quake2 up to v3.19

1. Shut down the Quake2 Server.

2. Copy the gamex86.dll and q2admin.txt into the Quake2 root
directory (wherever Quake2.exe is). You shouldn't 
be overwriting any mod files unless you already have a 
seamless dll installed for another mod (I believe there is 
a camera mod that is seamless).  You can only run one 
seamless dll at a time.

3. Start the server as usual.


The  Q2Admin  prints it's version number out just before 
the standard "==== InitGame ====" when the server starts.

Q2Admin also prints out warning messages if the config 
files can't be found.

To enable overflow detection you must set logfile to 2 or
3 on the server.  This is best done by adding it to your
server.cfg file.




1.2. Linux Install
------------------


PLEASE NOTE THAT Q2ADMIN ONLY RUNS WITH THE GLIBC VERSION
OF QUAKE2 FOR LINUX.  It will run with the non-glibc 
version but allot of features seam to be broken when
running it with that version.  Use at your own risk.
                                                    


1. Shut down the Quake2 Server.

2. Uncompress q2admin1.tar.gz into the Quake2 root
directory.  e.g. "tar zxf q2admin1.tar.gz"

3. For each mod that you want to protect, copy the 
following files into the mod directory.
    install
    q2admin.so
    q2admin.txt*
    q2adminban.txt*
    q2adminlog.txt*
    q2adminlrcon.txt*
    q2adminflood.txt*
    q2adminspawn.txt*
    q2adminvote.txt*
    q2admindisable.txt*
    q2admincheckvar.txt*

*Optional. Use only if you want to customithe Q2Admin
config for this particular mod rather than use the global
Q2Admin config.

4. Run "install" once. (e.g. ". ./install")

5. Start the server as usual.

The Q2Admin prints it's version number out just before 
the standard "==== InitGame ====" when the server starts.

Q2Admin also prints out warning messages if the config 
files can't be found.


The "install" script moves the old gamei386.so to 
gamei386.real.so and moves q2admin.so gamei386.so.  
If the install script is run again the files are moved 
back to there original names.  This can be used to 
uninstall or enable / disable the detect bot code.

To enable overflow detection you must set logfile to 2 or
3 on the server.  This is best done by adding it to your
server.cfg file (v3.20 only).




1.3. Alpha Linux Install
------------------------


1. Shut down the Quake2 Server.

2. Uncompress q2adminaxp1.tar.gz into the Quake2 root
directory.  e.g. "tar zxf q2adminaxp1.tar.gz"

3. For each mod that you want to protect, copy the 
following files into the mod directory.
    install
    q2admin.so
    q2admin.txt*
    q2adminban.txt*
    q2adminlog.txt*
    q2adminlrcon.txt*
    q2adminflood.txt*
    q2adminspawn.txt*
    q2adminvote.txt*
    q2admindisable.txt*
    q2admincheckvar.txt*

*Optional. Use only if you want to customize the Q2Admin
config for this particular mod rather than use the global
Q2Admin config.

4. Run "install" once. (e.g. ". ./install")

5. Start the server as usual.

The Q2Admin prints it's version number out just before 
the standard "==== InitGame ====" when the server starts.

Q2Admin also prints out warning messages if the config 
files can't be found.


The "install" script moves the old gameaxp.so to 
gameaxp.real.so and moves q2admin.so gameaxp.so.  
If the install script is run again the files are moved 
back to there original names.  This can be used to 
uninstall or enable / disable the detect bot code.

To enable overflow detection you must set logfile to 2 or
3 on the server.  This is best done by adding it to your
server.cfg file (v3.20 only).




1.4. Solaris Install
--------------------

The Solaris port is done by the SolarEclipse.  Go check 
them out for ports of other Quake2 mods to Solaris OS.

http://www.planetquake.com/eclipse


Intel Solaris OS:
----------------

1. Shut down the Quake2 Server.

2. Uncompress q2admin1_solx86.tar.gz into the Quake2 root
directory.  e.g. "tar zxf q2admin1_solx86.tar.gz"

3. For each mod that you want to protect, copy the 
following files into the mod directory.
    install.ksh
    q2admin.solx86.so
    q2admin.txt*
    q2adminban.txt*
    q2adminlog.txt*
    q2adminlrcon.txt*
    q2adminflood.txt*
    q2adminspawn.txt*
    q2adminvote.txt*
    q2admindisable.txt*
    q2admincheckvar.txt*

*Optional. Use only if you want to customize the Q2Admin
config for this particular mod rather than use the global
Q2Admin config.

4. Run "install" once. (e.g. ". ./install")

5. Start the server as usual.

The Q2Admin prints it's version number out just before 
the standard "==== InitGame ====" when the server starts.

Q2Admin also prints out warning messages if the config 
files can't be found.


The "install" script moves the old gamei386.so to 
gamei386.real.so and moves q2admin.solx86.so gamei386.so.  
If the install script is run again the files are moved 
back to there original names.  This can be used to 
uninstall or enable / disable the detect bot code.

To enable overflow detection you must set logfile to 2 or
3 on the server.  This is best done by adding it to your
server.cfg file (v3.20 only).



Sparc Solaris OS:
----------------

1. Shut down the Quake2 Server.

2. Uncompress q2admin1_sparc.tar.gz into the Quake2 root
directory.  e.g. "tar zxf q2admin1_sparc.tar.gz"

3. For each mod that you want to protect, copy the 
following files into the mod directory.
    install.ksh
    q2admin.sparc.so
    q2admin.txt*
    q2adminban.txt*
    q2adminlog.txt*
    q2adminlrcon.txt*
    q2adminflood.txt*
    q2adminspawn.txt*
    q2adminvote.txt*
    q2admindisable.txt*
    q2admincheckvar.txt*

*Optional. Use only if you want to customize the Q2Admin
config for this particular mod rather than use the global
Q2Admin config.

4. Run "install" once. (e.g. ". ./install")

5. Start the server as usual.

The Q2Admin prints it's version number out just before 
the standard "==== InitGame ====" when the server starts.

Q2Admin also prints out warning messages if the config 
files can't be found.


The "install" script moves the old gamesparc.so to 
gamesparc.real.so and moves q2admin.sparc.so gamesparc.so.  
If the install script is run again the files are moved 
back to there original names.  This can be used to 
uninstall or enable / disable the detect bot code.


To enable overflow detection you must set logfile to 2 or
3 on the server.  This is best done by adding it to your
server.cfg file (v3.20 only).




2. Admin commands and Setup
---------------------------


2.1. Admin commands
-------------------

There are 3 ways to use admin commands.

1. In the q2admin.txt for initial configuration.
2. In a Quake2 client connected to a server running Q2Admin.
3. On the server console.



2.2. q2admin.txt setup
-------------------------

The configuration file is automatically read from two places.
It first reads q2admin.txt from the Quake2 directory then
from the mod directory.

Anyone wanting to make a configuration specific to a mod 
can copy the q2admin.txt into the mod directory and 
modify it for the requirements of that mod.

If you run multiple servers from one mod directory you can
use the command "+set q2admintxt <q2admin.txt-filename>"
when starting a quake2 server. The q2amin.txt-file needs to
be in the mod directory for this to work. For example:
quake2 +set dedicated 1 +set q2admintxt q2admin_port27920.txt +exec ctfserver.cfg


The format for the .txt file is as follows:

Lines beginning with a ';' are comment lines and are 
ignored. e.g.

; this is a comment line.


The standard layout for a config command is as follows:

<command> "<value>"

Note the value must always be in quotes.  e.g.

adminpassword "secret"


For a list of configuration commands see section 2.5.




2.3. Quake2 client admin
------------------------

By default the Quake2 client admin is disabled. To enable it
you must provide a password. Edit the q2admin.txt file 
and add a default password.

All remote admin commands must be prefixed with a '!'.  

Before you can use any of the remote admin commands you
must login.  Logging in puts the user in admin mode where 
he/she can run admin commands. Admin mode lasts until
the level changes, the server is shut down or the player
disconnects from the server.  There can be multiple 
admins logged in at the same time.


To log into admin mode, from the Quake2 client use the 
command '!setadmin' with the admin password that was 
set in q2admin.txt file.

e.g.

!setadmin secret

The server will print the 'q2admin Admin mode active'.

There is also a automatically way to get into admin on 
client connect.

You can put your password into a cvar 'q2adminpassword' 
and this cvar is checked on client connect to see if it's 
got the correct password set.  If it has it will print 
'q2admin Admin mode active' message not long after connect.

e.g.
set q2adminpassword secret

then connect to the server.

Note, the cvar is ONLY checked at client connect.



Once set you can begin to use admin commands. If you try to
setadmin again the server will tell you that you are already
in admin mode.

For string values there are some special commands.  To have 
a double quote in a value, you must use '\q'.  To use a 
new line in a string you use '\n'.  To use a '$' in a string
you use a '\d'. To use a '\' in a string you use '\\'.  
e.g.

!customservercmd "error \qGet off my server, ZBot/RatBot user!\q"

or

!ban + name re ^VK-.*\d password duck


When typing in a command on the client console you
don't have to type the whole command in most cases, you just 
need to type in enough of the command to make it unique.
e.g. 
!customs "error \qGet off my server, ZBot/RatBot user!\q"

For a list of admin commands see section 2.5.




2.4. Quake2 Server admin
------------------------

All admin commands run from the server console must be
prefixed with 'sv !'.

e.g.
'sv !ban name blank'


For string values there are some special commands.  To have 
a double quote in a value, you must use '\q'.  To use a 
newline in a string you use '\n'.  To use a '$' in a string
you use a '\d'. To use a '\' in a string you use '\\'.  
e.g.

sv !customservercmd "error \qGet off my server, ZBot/RatBot user!\q"

or

sv !ban + name re ^VK-.*\d password duck


When typing in a command on the server console you
don't have to type the whole command in most cases, you just 
need to type in enough of the command to make it unique.
e.g. 
sv !customs "error \qstuff off ZBot/RatBot user\q"

For a list of server admin commands see section 2.5.




2.5. Admin command reference
----------------------------

Here is a list of all the admin commands broken down into sections:

Admin Setup:
  adminpassword                   - client console password (blank = disable)
  setadmin                        - sets a client into admin mode
  maxfps                          - controls the max clients cl_maxfps value
  minfps                          - controls the min clinets cl_maxfps value
  maxrate                         - controls a clients max rate value
  minrate                         - controls a clients min rate value
  gamemaptomap                    - converts the mod gamemap to map commands
  quake2dirsupport                - makes the win32 dll setup like the linux
  serverinfoenable                - enable/disable 'set Q2Admin "1.15" S'
  q2adminrunmode                  - q2admin run level
  maxclientsperframe              - max clients processed per frame
  framesperprocess                - messages per x frames.

Banning:
  ban                             - adds bans 
  banonconnect                    - disallow banned clients at the connect 
  chatban                         - adds chat bans
  chatbanning_enable              - enable chat bans
  defaultbanmsg                   - default ban message
  defaultchatbanmsg               - default chat ban message
  defaultreconnectmessage         - default reconnect message
  delban                          - deletes a ban from memory
  delchatban                      - deletes a chat ban from memory
  hackuserdisplay                 - message to show on detection of a hacked quake2.exe
  kickonnamechange                - kick client when changing to banned name
  listbans                        - lists all bans in memory
  listchatbans                    - lists all chat bans in memory
  nickbanning_enable              - enable nick banning
  ipbanning_enable                - enable ip banning
  reloadbanfile                   - clear and reload all bans
  lock                            - disable/enable access to the server
  lockoutmsg                      - message when trying to connect to a locked server
  reconnect_address               - enable forced reconnecting
  reconnect_time                  - forced reconnecting timeout in seconds
  reconnect_checklevel            - check level for the reconnect feature
  skincrashmsg                    - message to show on detection of a too large skin
  checkclientipaddress            - check if connecting clients has a valid ip address

Chat Flood Protection:
  chatfloodprotect                - chat flood protection setup (like QW)
  clientchatfloodprotect          - client chat flood protection setup 
  chatfloodprotectmsg             - chat flood message displayed
  mute                            - mutes a single client for x seconds
  reloadfloodfile                 - reloads the q2adminflood.txt file.
  listfloods                      - lists the commands that are flood protected
  floodcmd                        - adds a command to the flood protected list
  flooddel                        - dels a command from the flood list

Logging:
  clearlogfile                    - deletes the log file
  displaylogfile                  - displays the while log file to the console
  logevent                        - view / modify log events
  logfile                         - view / add / del log file setups

ZBot/RatBot/ZorBot/BW-Proxy/Nitro2(Xania)/Timescale Detection:
  clientsidetimeout               - internal development, don't touch.
  customclientcmd                 - run on the zbot/ratbot client when detected
  customservercmd                 - run on the server when zbot/ratbot detected
  disconnectuser                  - kicks zbot/ratbot user on detect
  disconnectuserimpulse           - kicks client for using impulses
  displayimpulses                 - displays message when client uses impulses
  displayzbotuser                 - display message when a client is detected as a zbot
  dopversion                      - determines if a p_version is sent to each player
  impulsestokickon                - which impulses to kick client on
  numofdisplays                   - number of times zbot/ratbot message is displayed
  randomwaitreporttime            - random wait time between detect & action
  zbotdetect                      - enable/disable zbot/ratbot detection
  zbotdetectactivetimeout         - wait time between detect & action taken
  zbotuserdisplay                 - message that is displayed on zbot detect
  zbc_enable                      - WhiteFang's ZbotCheck code enable
  zbc_jittermax                   - WhiteFang's ZbotCheck setting
  zbc_jittermove                  - WhiteFang's ZbotCheck setting
  zbc_jittertime                  - WhiteFang's ZbotCheck setting
  proxy_bwproxy                   - detection of BW-Proxy setup
  proxy_nitro2                    - detection of Nitro2/Xania setup
  timescaledetect                 - detect timescale cheaters.
  timescaleuserdisplay            - message to display for a timescale cheater
  
Client Connecting:
  customclientcmdconnect          - client command run on client connect
  customservercmdconnect          - server command run on client connect

Name Changing:
  displaynamechange               - displays message when user changes names
  namechangefloodprotect          - name change flood protection setup (like QW)
  namechangefloodprotectmsg       - name change flood protection message

Skin Changing:
  skinchangefloodprotect          - skin change flood protection setup (like QW)
  skinchangefloodprotectmsg       - skin change flood protection message

Lrcons:
  listlrcons                      - list all lrcons loaded into memory
  lrcon                           - add lrcon into memory.
  lrcondel                        - delete lrcon from memory.
  reloadlrconfile                 - clear and reload all lrcon's.
  rcon_random_password            - used random rcon password for the lrcon command.
  lrcon_timeout                   - lrcon timeout for a random set rcon

play_ commands control:
  play_all_enable                 - enable the play_all client command
  play_team_enable                - enable the play_team client command
  play_person_enable              - enable the play_person client command
  printmessageonplaycmds          - enable the message display of play_ commands

say_ commands control:
  extendedsay_enable              - enable extended say/play commands
  say_group_enable                - enable the say_group client command
  say_person_enable               - enable the say_person client command

Server console chatting:
  say_group                       - server console messaging to a group
  say_person                      - server console messaging to a client

MOTD:
  setmotd                         - set the MOTD file.

Client Control:
  stuff                           - force client to run a client command
  ip                              - displays a clients ip address
  kick                            - allows you to kick one or more clients

Spawning Entities Control:
  spawnentities_enable            - enable/disable the spawn entities feature
  listspawns                      - lists all the disabled entity types
  reloadspawnfile                 - reloads the spawn entity list from file
  spawncmd                        - add spawn entity type to the list
  spawndel                        - delete a spawn entity type
  spawnentities_internal_enable   - enable the internal disable feature

Command Voting:
  vote_enable                     - enable / disable command voting
  clientvotecommand               - client vote command to use
  clientvotetimeout               - vote timeout
  votecountnovotes                - count non-voters in percent
  votepasspercent                 - pass percent
  voteminclients                  - minimum clients needed to start a vote
  clientremindtimeout             - remind clients to vote every x seconds
  listvotes                       - lists all the allowed vote commands
  reloadvotefile                  - reload vote command list
  votecmd                         - add vote command
  votedel                         - delete vote command
  voteclientmaxvotes              - max votes a player can cast in x seconds
  voteclientmaxvotetimeout        - time limit that the player can cast x votes
  
Disable Client Commands Control:
  disablecmds_enable              - enable/disable the disable client commands feature
  listdisable                     - lists all the disabled commands
  reloaddisablefile               - reloads the disable commands list from file
  disablecmd                      - add disable command type to the list
  disabledel                      - delete a disable command type

Misc:
  version                         - prints the current version of q2admin out
  cvarset                         - more powerful version of server console set
  entity_classname_offset         - sets the classname offset, needed for some features
  filternonprintabletext          - filters out nonprintable characters.
  swap_attack_use                 - swaps +attack and +use.
  mapcfgexec                      - exec the map cfg files.

cl_pitchspeed:
  cl_pitchspeed_enable            - Enable/Disable cl_pitchspeed change detect.
  cl_pitchspeed_display           - Display when a user changes cl_pitchspeed.
  cl_pitchspeed_kick              - Kick user for changing cl_pitchspeed.
  cl_pitchspeed_kickmsg           - Kick message.

cl_anglespeedkey:
  cl_anglespeedkey_enable         - Enable/Disable cl_anglespeedkey change detect.
  cl_anglespeedkey_display        - Display when a user changes cl_anglespeedkey.
  cl_anglespeedkey_kick           - Kick user for changing cl_anglespeedkey.
  cl_anglespeedkey_kickmsg        - Kick message.

Client Side Variable checking:
  checkvarcmds_enable             - Enable/Disable client side variable checking
  listcheckvar                    - lists all the checked variables commands
  reloadcheckvarfile              - reloads the checked variables commands list from file
  checkvarcmd                     - add checked variable command type to the list
  checkvardel                     - delete a checked variable command type
  checkvar_poll_time              - poll timeout in second between each variable check




Command:  "adminpassword"
Value:    String
Where Allowed:  q2admin.txt, server console.

  Sets the password used in activating admin mode 
  for a Quake2 client.


Command:  "ban"
Where Allowed:  client console, server console.

  See section 2.7.2.

Command:  "banonconnect"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Ban on client connect (YES) or ban on client begin (NO).  
  When banning on client connect the banned player will not 
  use up a client position at all BUT no ban message is 
  displayed.  Ban on client begin will use a player position 
  for the length of the client map load then be kicked.  But 
  client will get a BAN message before getting kicked.
  See section 2.7.2.


Command:  "chatban"
Where Allowed:  client console, server console.

  See section 2.7.2.


Command:  "chatbanning_enable"
Where Allowed:  client console, server console.

  Enable chat banning.
  See section 2.7.2.


Command:  "chatfloodprotect"
Value:    <number of messages> <in x seconds> <silence in seconds>
Where Allowed:  q2admin.txt, client console, server console.

  Chat flood protection so that a chat macro 
  does not flood the server and other clients.

  x number of messgaes in y number of seconds will set will
  mute the player for z seconds.

  if <silence in seconds> is 0 then the person is kicked.
  
  if <silence in seconds> is -1 then the person is permanently 
  muted.
  
  The console format for the command is:
  
  [sv] !chatfloodprotect <number of messages> <in x seconds> <silence in seconds>
  
  to disable change to:
  [sv] !chatfloodprotect disable

  See section 2.7.4.


Command:  "chatfloodprotectmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Chat flood protection message.
  See section 2.7.4.


Command:  "checkvar_poll_time"
Value:    Number (seconds)
Where Allowed:  q2admin.txt, client console, server console.

  The number of seconds between before each variable for each client is checked when 
  check client side variables is enabled.
  e.g. if there are 5 variables listed in q2admincheckvar.txt, the first variables is 
  checked first, x seconds later the next line is checked and so on until there are no
  more variables to check.  It then loops back to checking the first variable again.
  So if checkvar_poll_time is 60 seconds and there are 5 variables listed, each 
  variable will be checked one every 5 minutes.  Reducing this variable will increase 
  the traffic between the client and the server.


Command:  "checkvarcmd"
Where Allowed:  client console, server console.

  [sv] checkvarcmd CT variable value
  [sv] checkvarcmd RG variable lower upper
  
  Used to add client side variable command to the list.  See section 2.14.


Command:  "checkvarcmds_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enables/Disables the client side variable checking commands list.  See section 2.14.


Command:  "checkvardel"
Where Allowed:  client console, server console.

  [sv] disabledel disablenum

  Used to delete a client side variable command from the checked variable commands list. 
  See section 2.14.


Command:  "clearlogfile <lognum>"
Value:    none
Where Allowed:  client console, server console.

  Where lognum is 1 to 32.  
  Deletes the Q2Admin lognum log file if it is setup.


Command:  "clientchatfloodprotect"
Where Allowed:  client console, server console.

  Client chat flood protection so that a chat macro 
  does not flood the server and other clients.  This 
  commands sets the clients local flood protection 
  setup which overrides the global one if enabled.
  
  x number of messgaes in y number of seconds will set will
  mute the player for z seconds.

  if <silence in seconds> is 0 then the person is kicked.
  
  if <silence in seconds> is -1 then the person is permanently 
  muted.
  
  The console command format is:
  
  [sv] !clientchatfloodprotect [LIKE/RE/CL] name [xxx(num) xxx(sec) xxx(silence) / disable]

  See section 2.7.4.
   


Command:  "clientsidetimeout"
Value:    Number (seconds)
Where Allowed:  q2admin.txt, client console, server console.

  Mainly used for internal development testing. Don't touch.


Command:  "clientvotecommand"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Client console vote command.  See section 2.11.


Command:  "clientvotetimeout"
Value:    Seconds
Where Allowed:  q2admin.txt, client console, server console.

  Sets the clients voting timeout is seconds.  See section 2.11.


Command:  "cl_anglespeedkey_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Enables/disables the cl_anglespeedkey change detect. See section 2.13.


Command:  "cl_anglespeedkey_display"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Display when a cl_anglespeedkey change is detected. See section 2.13.


Command:  "cl_anglespeedkey_kick"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Kick the user when a cl_anglespeedkey change is detected. See section 2.13.


Command:  "cl_anglespeedkey_kickmsg"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Kick string to display when the user is kicked.


Command:  "cl_pitchspeed_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Enables/disables the cl_pitchspeed change detect. See section 2.13.


Command:  "cl_pitchspeed_display"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Display when a cl_pitchspeed change is detected. See section 2.13.


Command:  "cl_pitchspeed_kick"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Kick the user when a cl_pitchspeed change is detected. See section 2.13.


Command:  "cl_pitchspeed_kickmsg"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.
  
  Kick string to display when the user is kicked.


Command:  "customclientcmd"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Custom client console command to run on ZBot/RatBot bot detect.
  (run after the client messages and before the disconnect)


Command:  "customclientcmdconnect"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Custom client console command that runs when a client connects.


Command:  "customservercmd"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Custom server console command to run on ZBot/RatBot bot detect.
  (run after the log file update)

  There is a special variable you can pass to this command.  
  %c will get translated to the clients connection number.


Command:  "customservercmdconnect"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Custom server console command that is run when clients 
  connect.  '%c's are replaced by the client number.
  e.g. "sv !stuff cl %c file clientconnect.txt"


Command:  "cvarset"
Value:    None
Where Allowed:  client console, server console.

  Allows the settings of server console cvar's.  This is a little 
  more powerful than the 'set' command as it accepts the string
  process features of q2admin.  See section 2.2 for more details.
  Setting a value to 'none' will also delete the cvar (the same
  as setting it to blank. This is useful for remote control of 
  features like RA2 password cvar where you can set it remotely 
  but you can't blank it remotely.  You can with cvarset 
  e.g. "rcon xxx sv !cvarset password none"
  

Command:  "defaultbanmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  The default ban message for logging and displaying when someone 
  is banned.


Command:  "defaultchatbanmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  The default chat ban message for logging and displaying when someone 
  has typed in a banned word.


Command:  "defaultreconnectmessage"
Value:    String
Where Allowed:  q2admin.txt, client console, server console

  Message to display while a player is reconnecting to the Quake2 server.


Command:  "delban"
Value:    Ban Number
Where Allowed:  client console, server console.

  Deletes a ban from memory (not from the q2adminban.txt file though).
  See section 2.7.2.


Command:  "delchatban"
Value:    Ban Number
Where Allowed:  client console, server console.

  Deletes a chat ban from memory (not from the q2adminban.txt file though).
  See section 2.7.2.


Command:  "disablecmd"
Where Allowed:  client console, server console.

  [sv] disablecmd [SW/EX/RE] command
  
  Used to add disabled command to the list.  See section 2.12.


Command:  "disablecmds_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enables/Disables the disable client commands list.  See section 2.12.


Command:  "disabledel"
Where Allowed:  client console, server console.

  [sv] disabledel disablenum

  Used to delete a disabled command from the disabled commands list. 
  See section 2.12.


Command:  "disconnectuser"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Disconnect a bot user when detected.


Command:  "disconnectuserimpulse"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Disconnect user if generating an impulse.


Command:  "displayimpulses"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Display when impulses are generated / used 
  This is a future-proof thing that will display messages 
  when impulses are used.  The reason is if the ZBot 
  person has a ZBot hack to get around the "normal" detect 
  this will display when he is sending commands.  Sort of 
  like a back-up method of detecting ZBot users.  The only 
  problem is that normal clients can generate impulses as 
  well and I don't know if this is for commanding the ZBot
  or something that a normal q2 mod uses.  Only 1 q2 mod 
  that I know off uses impulses though so chances are that 
  it's a ZBot user.
  
  ZBot impulses:
  - Impulse 169 : toggle menu
  - Impulse 170 : move up (menu)
  - Impulse 171 : move down (menu)
  - Impulse 172 : increase item (menu)
  - Impulse 173 : decrease item (menu)
  - Impulse 174 : toggles bot on/off 
  - Impulse 175 : toggles scanner display
 
  Note: I display a message when ANY impulse is used just 
  to be safe...


Command:  "displaylogfile <lognum>"
Value:    none
Where Allowed:  client console

  Where lognum is 1 to 32.
  Displays the contents of lognum logfile to the console.


Command:  "displaynamechange"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Display name changes so everybody knows someone's changed their name...


Command:  "displayzbotuser"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Display user is a ZBot/RatBot to the rest of the server.


Command:  "dopversion"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Determines whether Q2Admin will send a "p_version"-string to a player
  on connect. If the player is using a proxy it will respond with its
  name and version number.


Command:  "entity_classname_offset"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  The byte offset to the classname pointer in the entity structure. 
  This is required for the internal disable spawn entities feature.
 
  If this figure is wrong q2admin may crash and the internal disable 
  entities feature will not work or the entity create/delete logging 
  feature to work.
 
  Email killerbee to check what the offset is for the mod that you are 
  running.


Command:  "extendedsay_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enables the say_group and say_person client commands to be run
  from the say and messagemode commands in a special syntax.  
  See section 2.8.6


Command:  "filternonprintabletext"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Filters out any non-printable characters from any text being said/printed.
  It only allows characters in the range of 0x20 to 0x7E.
  This may upset some mods printing.


Command:  "floodcmd"
Where Allowed:  client console, server console.

  Adds a client console command to the flood protected list in memory.

  See section 2.7.4.


Command:  "flooddel"
Where Allowed:  client console, server console.

  Deletes a client console command from the flood protected list in memory.

  See section 2.7.4.


Command:  "framesperprocess"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  q2admin processes messages every x frames.  This is a internal
  testing value and it is not a good idea to change it.


Command:  "gamemaptomap"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Converts the mod gamemap commands to map commands for when it changes 
  levels.  This forces the mod dll to unload / reload.


Command:  "impulsestokickon"
Value:    List of numbers
Where Allowed:  q2admin.txt, client console, server console.

  What impulses do we want to kick people on??  disconnectuserimpulse 
  must be set to 'Yes' for this to take action.  Commenting this 
  command out of the q2admin.txt will select all impulses.

  The format for running this command in the server console or the 
  client console is:

  [sv] !impulsestokickon [add/reset] [impulse] [impulse] ...

  [add] will add the following impulses to the current list.
  [reset] will clear the current list (effectively selecting all 
  impulses)


Command:  "ip"
Where Allowed:  client console, server console.

  Displays the ip address of a client.
  
  [sv] !ip [like/re/cl] name


Command:  "ipbanning_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable ip banning.  For bans that use both IP and Name, it then just 
  uses the name.
  See section 2.7.2.


Command:  "hackuserdisplay"
Value:    String
Where Allowed:  q2admin.txt, client console, server console

  Message to display when a player gets kicked for using a hacked
  quake2.exe to get around (some of) the protection methods of Q2Admin.


Command:  "kick"
Where Allowed:  client console, server console.

  Kick one or more clients.
  
  [sv] !kick [like/re/cl] name

  e.g.
  !kick re ^VK-.*\d
  will kick any client which starts with 'VK-'

  !kick cl 1+4+5+7
  will kick clients 1, 4, 5, 7.


Command:  "kickonnamechange"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Kick uses for changing to banned names.  If No the name is not allowed 
  to change.
  See section 2.7.2.


Command:  "listbans"
Where Allowed:  client console, server console.

  Lists all the bans loaded into memory.
  See section 2.7.2.


Command:  "listchatbans"
Where Allowed:  client console, server console.

  Lists all the chat bans loaded into memory.
  See section 2.7.2.


Command:  "listcheckvar"
Where Allowed:  client console, server console.

  Lists all the checked variable commands loaded into memory.
  See section 2.14


Command:  "listdisable"
Where Allowed:  client console, server console.

  Lists all the disabled commands loaded into memory.
  See section 2.12


Command:  "listfloods"
Where Allowed:  client console, server console.

  Lists all the flood protected commands  loaded into memory.
  See section 2.7.4.


Command:  "listlrcons"
Where Allowed:  client console, server console.

  Lists all the lrcons loaded into memory.
  See section 2.7.3.


Command:  "listspawns"
Where Allowed:  client console, server console.

  Lists all the spawns loaded into memory.
  See section 2.10


Command:  "listvotes"
Where Allowed:  client console, server console.

  Lists all the vote commands loaded into memory.
  See section 2.11


Command:  "lock"
Value:    Yes/No
Where Allowed:  client console, server console.

  When set to yes it locks down the server so 
  that noone else can connect.


Command:  "lockoutmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  The server lockdown message that connecting users see.


Command:  "logevent"
Where Allowed:  client console, server console.

  Lists / edits the log event list.
  See section 4.


Command:  "logfile"
Where Allowed:  client console, server console.

  Lists / edits the log file list.
  See section 4.


Command:  "lrcon_timeout"
Value:    Number (seconds)
Where Allowed:  q2admin.txt, client console, server console.

  Timeout, when rcon_random_password is set to "Yes", for how long lrcon command is
  timed out before the rcon password is set back to the orginal value.
  See section 2.7.3.


Command:  "lrcon"
Where Allowed:  client console, server console.

  Adds lrcon's to the internal lrcon list.
  See section 2.7.3.


Command:  "lrcondel"
Where Allowed:  client console, server console.

  Deletes lrcon's from the internal lrcon list.
  See section 2.7.3.


Command:  "mapcfgexec"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enables the feature to exec the mapcfg/<mapname>-pre.cfg, 
  mapcfg/<mapname>-post.cfg and mapcfg/<mapname>-end.cfg files.


Command:  "maxclientsperframe"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Maximum clients that q2admin can process per frame.
  This is a debugging/internal  value that should not be 
  changed unless told to.


Command:  "maxfps"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Sets the maximum cl_maxfps that a client may have.  
  See section 2.7.1.
  To disable set to 0.


Command:  "maxrate"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Sets the maximum rate that a client may have.
  See section 2.7.1.
  To disable set to 0.



Command:  "maximpulses"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Max number of impulses to detect before kicking.


Command:  "minfps"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Sets the minimum cl_maxfps that a client may have.  
  To disable set to 0.


Command:  "minrate"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Sets the minimum rate that a client may have.
  See section 2.7.1.
  To disable set to 0.

Command:  "mute"
Where Allowed:  client console, server console.

  [sv] !mute [like/re/cl] name [time(seconds) / PERM]
  This command will stop the client from messaging for time 
  seconds.  
  
  A mute time of 0 will unmute a client.

  [PERM] option will permanently mute a client.

  One caveat for this command is that is works a lot better if 
  the underlying mod's flood protection is turned off and 
  q2admin's flood protection is used.  For most code based on 
  the v3.20 code you use the 'flood_msgs 0' to turn off flood 
  protection.

  See section 2.7.4.


Command:  "namechangefloodprotect"
Value:    <number of messages> <in x seconds> <silence in seconds>
Where Allowed:  q2admin.txt, client console, server console.

   Name changing flood protection so that a name change macro 
   does not flood the server and other clients.
   [sv] !namechangefloodprotect <number of messages> <in x seconds> <silence in seconds>
   if <silence in seconds> is 0 then the person is kicked.
   to disable change to:
   [sv] !namechangefloodprotect disable


Command:  "namechangefloodprotectmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Name change flood protection message.


Command:  "nickbanning_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable nick banning.  For bans that use both IP and Name, it 
  then just uses the IP.
  See section 2.7.2.


Command:  "numofdisplays"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Message to display that the using is using a bot.
  NOTE: must have 1 and only 1 '%s' in the line
  %s will print the users name


Command:  "play_all_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable play_all command.
  Plays a wav file for each person in the game.  
  See section 2.8


Command:  "play_team_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable play_team command (may not work %100 with each mod, 
  depends on how the mod teams are implemented).
  Plays a wav file on each person that is in your team.
  See section 2.8


Command:  "play_person_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable play_person command.
  Plays a wav file for 1 person in the game.
  See section 2.8


Command:  "printmessageonplaycmds"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Print messages when clients use the play sound commands.  It tells who is playing the sound and to whom. 
  (all/team/private)
 

Command:  "proxy_bwproxy"
Value:    Number (0-2)
Where Allowed:  q2admin.txt, client console, server console.

  Controls the detection of BW-Proxy proxy.
  
  0 - No Detection.  This means they will be detected as zbots.
  1 - Normal Detection.  This is the most secure method but with some mods 
      this may not work.
  2 - Insecure Detection.  This will allow the proxies to connect but
      it may also allow some types of modified zbot to connect as well.
      Also settings either proxy_bwproxy or proxy_nitro2 to '2' will 
      automatically set the other one to '2' as well.


Command:  "proxy_nitro2"
Value:    Number (0-2)
Where Allowed:  q2admin.txt, client console, server console.

  Controls the detection of Nitro2 proxy.
  
  0 - No Detection.  This means they will be detected as zbots.
  1 - Normal Detection.  This is the most secure method but with some mods 
      this may not work.
  2 - Insecure Detection.  This will allow the proxies to connect but
      it may also allow some types of modified zbot to connect as well.
      Also settings either proxy_bwproxy or proxy_nitro2 to '2' will 
      automatically set the other one to '2' as well.



Command:  "quake2dirsupport"
Value:    Yes/No
Where Allowed:  q2admin.txt.

  Only for use in windows.
  This tells the Q2Admin dll that it is running in the Quake2 
  directory (or the release directory for v3.20+).  When set to 
  'No' the dll is being loaded from the mod directory like the 
  linux / solaris versions. For win32 the old mod dll must be 
  called 'gamex86.real.dll' for this to work.


Command:  "randomwaitreporttime"
Value:    Number (seconds)
Where Allowed:  q2admin.txt, client console, server console.

  When zbotdetectactivetimeout is -1 this value is used to 
  generate a random time between 5 seconds to 5 seconds + 
  randomwaitreporttime seconds.
  e.g. 55 means 5 seconds to 60 seconds


Command:  "rcon_random_password"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Changes the rcon password to some random string for the lrcon command.  This prevents
  people from discovering the real rcon password through dumpping packets.
  See section 2.7.3.


Command:  "reconnect_address"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Force client to reconnect to inital connection, bepasses any 
  proxies that may be runnning.  Please put in the ip 
  address/name and port (if needed) of the server.
 
  e.g. reconnect_address "quake2.games.org.nz:27920"
  or
  e.g. reconnect_address "192.168.1.10"
 
  No address means the feature is disabled.


Command:  "reconnect_checklevel"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  This controls how strict the reconnect feature is.
  0 - very strict (most secure but is the most likely to cause multiple reconnections)
  1 - less strict (not so secure, this should allow NAT router / internet software proxy
                   to connect more easily) 


Command:  "reconnect_time"
Value:    Number (seconds)
Where Allowed:  q2admin.txt, client console, server console.

  Client must reconnect in X time from the inital connect.  


Command:  "reloadbanfile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current ban's and reloads the banfile.


Command:  "reloadcheckvarfile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current checked variable command list and 
  reloads the checked variable command list file. 
  See section 2.14


Command:  "reloaddisablefile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current disbled command list and reloads the 
  disabled command list file. See section 2.12


Command:  "reloadfloodfile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current flood protected commands from memory and
  reloads them from the q2adminflood.txt file.
  See Section 2.7.4.


Command:  "reloadlrconfile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current lrcon's and reloads the lrcon command 
  list file.


Command:  "reloadspawnfile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current spawn list and reloads the spawn  
  list file. See section 2.10


Command:  "reloadvotefile"
Value:    None
Where Allowed:  client console, server console.

  Deletes all the current vote command list and reloads the 
  vote command list file. See section 2.11


Command:  "say_group"
Where Allowed:  server console.

  Sends a message to 1 or more clients.
  [sv] !say_group [LIKE/RE/CL] <playername> <message>

  See section 2.8


Command:  "say_group_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable say_person command.
  Send a message to a group of people in the game.
  See section 2.8


Command:  "say_person"
Where Allowed:  server console.

  Sends a message to 1 client.
  [sv] !say_person [LIKE/RE/CL] <playername> <message>

  See section 2.8


Command:  "say_person_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable say_person command.
  Send a message to 1 person in the game.
  See section 2.8


Command:  "serverinfoenable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Sets Q2Admin's version number in a cvar on the server. This way
  programs like Gamespy will show Q2Admin's version number as part
  of the server info. If you're getting "Info string length exceeded"
  on the server console, change this value to "No".


Command:  "setadmin"
Value:    String
Where Allowed:  client console.

  Sets the admin mode for Quake2 clients.


Command:  "setmotd"
Value:    String (filename)
Where Allowed:  q2admin.txt, client console, server console.

  MOTD file to display at client connect (and also happens on 
  level changed as well) 
  For the console versions of the command, if you don't supply
  a filename, the MOTD is cleared. If you do the MOTD is loaded.
  NOTE: by default the MOTD is read from the Quake2 directory.
  If you want to use a mod motd you must supply the directory 
  name. e.g. sv !setmotd "ctf/mymotd.txt"
  Also a any qauke2 client connected to the server may redisplay
  the motd by typing 'motd' into the console.


Command:  "skinchangefloodprotect"
Value:    <number of skin changes> <in x seconds> <silence in seconds>
Where Allowed:  q2admin.txt, client console, server console.

   Skin changing flood protection so that a skin change macro 
   does not flood the server and other clients.
   [sv] !skinchangefloodprotect <number of skin changes> <in x seconds> <silence in seconds>
   if <silence in seconds> is 0 then the person is kicked.
   to disable change to:
   [sv] !skinchangefloodprotect disable


Command:  "skinchangefloodprotectmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Skin change flood protection message.


Command:  "skincrashmsg"
Value:    String
Where Allowed:  q2admin.txt, client console, server console

  Message to display when a player gets kicked for trying to change
  his skin to a very large value which would crash the server.


Command:  "soloadlazy"
Value:    Yes/No
Where Allowed:  q2admin.txt.

  This switch is only used in the LINUX version.  It determines 
  the way that the so file is loaded.  All so files should load 
  either way... so files that only load with the lazy switch set to 
  'yes' means that you are either missing a library to run the mod so 
  or that there is a problem with the so file that should be reported 
  to the author of the mod.  Running so files that requires the lazy 
  flag set to 'yes' means that the mod may crash at some stage.  This 
  switch is only provided so that if you require such mods to run you 
  can still run them with q2admin.  
  Note: q2 loads so files in lazy mod, which is why they work under 
  q2 and why they may crash as well.



Command:  "spawncmd"
Where Allowed:  client console, server console.

  [sv] spawncmd [SW/EX/RE] command
  
  Used to add spawn entities to the list.  See section 2.10.


Command:  "spawndel"
Where Allowed:  client console, server console.

  [sv] spawndel spawnnum

  Used to delete spawn enities from the spawn list. See section 2.10.


Command:  "spawnentities_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enables/Disables the spawn list from disabling the spawn entities
  at map load.  See section 2.10.


Command:  "spawnentities_internal_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enables/Disables the internal spawn list feature from disabling the 
  spawn entities.  spawnentities_enable must be enabled for this to
  work.  See section 2.10.


Command:  "stuff"
Where Allowed:  client console, server console.

  [sv] !stuff [LIKE/RE/CL] name [client commands / FILE <filename>]

  Stuffs the command or all the lines in the file to the client.
  The client will run the command(s) just like the person typed 
  them into there quake2 client console themselves.  Combine
  this command with the customservercmdconnect command to force
  default settings for all clients that connect.

  To do multiple client commands in the one stuff command can work but 
  can be a little tricky. You must surround the command with double 
  quotes '"' and any double quotes you use in the command you must use 
  the '\q' syntax. e.g.
  sv !stuff cl 0 "say \qxxx\q;say \qyyy\q;say \qzzz\q"


Command:  "swap_attack_use"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Swaps the use of +attack and +use.  This can be useful as if the players have 
  to use +use to fire instead of +attack then it renders the auto-aim proxy 
  unsable. 

  Side Note Proz (aka Black Monk) sent me:

    BTW, for Macintosh users, !swap_attack_use is painful.  They need to use 
    their Q2 mouse controls to set the fire button to be, "ALT24".  I think.  
    They can't just "bind mouse1 +use" like PC users can, the mice act 
    differently due to InputSprockets.  And if a Mac user is using, say, USB 
    OverDrive to get the Razor Boomslang mouse to work, popular for Mac gamers 
    but not a supported Mac product, then they won't be able to bind to ALT24 at 
    all meaning they are screwed or they have to bind a keyboard key to get it 
    working.  Just an FYI in case you update the docs.


Command:  "timescaledetect"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Detect timescale cheaters.


Command:  "timescaleuserdisplay"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Message to display when a timescale cheater has been detected.


Command:  "version"
Value:    none
Where Allowed:  client console, server console.

  Displays q2admin version number.  Also works in the Quake2 
  console if your not in admin mode.


Command:  "votecmd"
Where Allowed:  client console, server console.

  [sv] votecmd [SW/EX/RE] command
  
  Used to add a vote command to the vote command list.  See section 2.11.


Command:  "voteclientmaxvotes"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  Maximum number of votes allowed in <voteclientmaxvotetimeout> time for one player.
 
  0 = disabled, i.e. unlimited.

  e.g. if you would like 5 votes every ten min's for a client you will have:
  voteclientmaxvotes "5"
  voteclientmaxvotetimeout "600"

  if you would like only 1 vote allowed every min.
  voteclientmaxvotes "1"
  voteclientmaxvotetimeout "60"

  if you would like unlimited voting
  voteclientmaxvotes "0"

  see voteclientmaxvotetimeout
  see section 2.11.



Command:  "voteclientmaxvotetimeout"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  
  Time in seconds allowed for <voteclientmaxvotes> to be cast for one player.

  0 = for whole level.


  e.g. if you would like 2 votes every 30 seconds:
  voteclientmaxvotes "2"
  voteclientmaxvotetimeout "30"

  if you would like 10 votes for every level:
  voteclientmaxvotes "10"
  voteclientmaxvotetimeout "0"

  see voteclientmaxvotes
  see section 2.11.


Command:  "votecountnovotes"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable/disable counting of clients that didn't vote as a no vote 
  in the percent calculation.


Command:  "votedel"
Where Allowed:  client console, server console.

  [sv] votecmd votenum
  
  Used to delete a vote command from the vote command list.  See section 2.11.


Command:  "voteminclients"
Value:    Number
Where Allowed:  q2admin.txt, client console, server console.

  The minimum numnber of clients that need to be connected before a vote
  can be proposed.  See section 2.11.



Command:  "votepasspercent"
Value:    Number (1-100)
Where Allowed:  q2admin.txt, client console, server console.

  The percent of 'yes' votes required to pass a vote command.
  See section 2.11.


Command:  "vote_enable"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable/disable voting.



Command:  "zbotdetect"
Value:    Yes/No
Where Allowed:  q2admin.txt, client console, server console.

  Enable/disable main ZBot/RatBot detection.


Command:  "zbotdetectactivetimeout"
Value:    Number (seconds)
Where Allowed:  q2admin.txt, client console, server console.

  Once a ZBot/RatBot is detected, how long to wait before actioning it.
  -1 will generate a random timeout between 5 seconds to 5 
  seconds + randomwaitreporttime seconds.


Command:  "zbotuserdisplay"
Value:    String
Where Allowed:  q2admin.txt, client console, server console.

  Message to display that the using is using a ZBot/RatBot.
  NOTE: must have 1 and only 1 '%s' in the line
  %s will print the users name






2.5.1. "[LIKE/RE/CL] name" explained
------------------------------------

Many of the q2admin server console / client console use the 
"[LIKE/RE/CL] name" format to find one or many clients to
apply the command to.

There are 3 ways that q2admin commands allow to select a
client.  

"LIKE name"  The name part must be somewhere in the 
             client name.

"RE name"    Uses regular expressions syntax to find the 
             client name. 
"CL cnum"    This allows you to give the client number.
             You can use the server console command 'status'
             to find out the client number.  
             For command that allow multiple clients to be 
             selected you can use multiple client numbers
             with a '+' between each client number.
             e.g. "CL 0+2+3+4"

For commands that allow only 1 client and more than 1 client 
matches then the command erros out.





2.6. Impulses explained
-----------------------

The ZBot has two types of commands for the client to communicate 
with the ZBot proxy.  One is special Quake2 console commands
and the other is the Quake2 impulse command.  The Quake2 impulse 
command is basically the same as the Quake impulse command and is
not used in Quake2. Because Quake2 doesn't use the impulse 
command it just ignores it.  What the detector does it monitor the
impulse commands that are generated by a Quake2 client and 
log / display / kick (if you wish) when an impulse is generated.
The problem with detecting impulses is that ANY Quake2 client can
generate an impulse.  This can lead to false detections if you kick
people generating impulses.  I leave it up to the server admin to
decide how to handle impulses.

Impulse detection is included as a backup for of detection in case
the main ZBot detection fails.  Most admins can ignore 
impulses unless they are being regularly generated. If they are
then there is a chance that that person is using a hacked or
modified ZBot that can gets around the main detection.

Reasons for clients generating impulses:

1. They are using a hacked ZBot.
2. They have used a ZBot before and have exceed zbot.cfg that
   comes with the ZBot and the binded keys have been saved to 
   there config.cfg file. Then they press the keys by accident
   during play.
3. The mod uses impulses (I don't know of any other mod that
   uses them though).
4. They think they are in Quake for some reason!!





2.7. Advanced admin commands
----------------------------


2.7.1. Max Rate / Max fps
-------------------------

RATE:
The Quake2 server sends one packet per server frame (10 per second)
regardless of the client's cl_maxfps setting. The client's rate setting,
which can be set by the server to provide rate-capping, sets the maximum
number of bytes per second (not bytes per frame) that the client wishes to
receive. If the rate is exceeded, the server will drop packets to avoid
overflowing the connection. (Note: The rate is expressed in bytes of game
data, so it should be lower than the available bandwidth to allow for
protocol overhead.)


The maxrate command clips a players rate so that it can't be 
changed to greater than the maxrate setting.  Setting maxrate to
0 turns it off.

The minrate command clips a players rate so that it can't be 
changed to a lesser than the minrate setting.  Setting minrate to
0 turns it off.



CL_MAXFPS:
The Quake2 client sends a packet to the server once per client frame. The
client framerate varies from machine to machine, so a fast machine with
limited bandwidth runs the risk of overflowing its connection.
Unfortunately, the only way to limit the upstream traffic is to limit the
client framerate via cl_maxfps.

The maxfps command clips a players cl_maxfps value so that it
can't be changed to greater than the maxfps settings. Setting
maxfps to 0 turns it off.  Setting the maxfps doesn't have a 
noticeable effect unless the player is used to having a fast 
computer on a show connection.  If you want to set cl_maxfps 
I would suggest ranges from 31 to 41.  The other problem with 
this is there is a chance that it will not work with all mods.  
You can tell it's not working correctly when player names start 
to get truncated.




This was the taken from the UQSA forum's:

Whatz Up With "Rate" 
After all the hoopla concerning the "rate" setting and how it 
affects the server and client, Brett Jacobs decided to email 
Zoid and ask him just "whatz up with dat rate stuff?" Below 
is his reply to Brett's question. Brett's main reason for 
emailing him was because a server kicked Brett for having his 
"rate" set to 15,000.

Hi Zoid,
It seems the server fad now is to disallow too high of a "rate" 
setting for clients. Can you explain to me how a client's high 
"rate" setting can affect the server and also what affect will 
it have on other players on that server. Also does my clients 
"rate" setting affect other players differently depending on 
whether they have a high or low bandwidth connection?


Zoid's Reply:

Rate simply controls the amount of data a server will send to 
you. It has no effect on other players, since every player has 
his own independent data stream. What is sent to the player is 
calculated individually, so a player with rate 25000 does not 
affect a player with rate 2500. It seems to be a common 
misunderstanding to this.

It is not rate that beats the crap out of the servers, but the 
client's framerate. The client sends a packet every frame. Even 
if you had a rate of 2500, you could still be sending up to 72 
packets/second, which is why low bandwidth connections should 
lower their cl_maxfps, since at 72 packets/second, they could 
lag themselves with the outgoing bandwidth.

The only way I could see rate affecting players is if the 
server _itself_ was running out of bandwidth. This could happen 
if the server was running on a low bandwidth line (cable modem) 
and there were lots of players. The majority of servers run on a 
decent connections though (such as T1), so this is probably not an 
issue.

Also note that rate is simply an indicator of the _maximum_ the 
server will send. If you are standing a room with one other player, 
there isn't that much to send--you wouldn't hit the 25000 limit. 
The average byte/second on a player on a server is about 2k to 3k. 
Of course, busy rooms bump that up (such as CTF--you get a lot of 
fights in the bases with lots of players).

I think what is happening is players get in a busy room (such as a 
heavy attack on a CTF base) and low bandwidth guys start starving, 
since their rate setting starts clipping pretty heavily to make 
sure they don't start falling behind by being overrun with data. 
This would happen regardless as to the rate of the other players. 
A busy room is a busy room and higher bandwidth players (not 
necessary lower ping, in this case, it's bandwidth that matters) 
will have an advantage since they won't get as much data clipped 
out by rate.

Back in the days when we didn't have rate settings, I imagine many 
people remember being on e1m7 on a NetQuake server and finding 
their ping had gone to like 3000 or so--this is because they 
were in essentially one room with lots of other players and the 
server was sending them more data then they could handle. This is 
why rate was added in the first place, to cut down on the amount 
of data being sent and keep you from lagging behind. Now, Quake2 
is MUCH more efficient than NetQuake (it uses almost 2/3rds less 
bandwidth than NetQuake did), it still sends out a lot of data in 
a very busy room.

-----

/// Zoid.





2.7.2. IP / Nick Banning
------------------------

Banning can be enabled and disabled by the 'nickbanning_enable', 
'ipbanning_enable' and 'chatbanning_enable' commands.  

They can be enabled/disabled in the q2admin.txt by changing the
there values to "Yes" or "No".

e.g.
nickbanning_enable "Yes"
ipbanning_enable "Yes"
chatbanning_enable "Yes"

They can also be enabled/disabled from the server console / client admin 
console as well.  See section 2.3 and 2.4 for how to do this.

The permanent ban list, q2adminban.txt, is loaded from the Quake2 directory and
then the mod directory in that order.

The format for q2adminban.txt is a simple text file.  

In q2adminban.txt there are 3 types of lines:
 
1. comments which are ignored.
2. nick / ip ban's which the line begins with 'BAN:'
3. chat bans which the line begins with 'CHATBAN


1. Comments:
Lines beginning with ';' are comments.


2. IP and Name Banning:

The format for a ban is:

BAN: [+/-(-)] [ALL/[NAME [LIKE/RE] "name"/BLANK/ALL(ALL)] [IP xxx[.xxx(0)[.xxx(0)[.xxx(0)]]][/yy(32)]] [PASSWORD "xxx"] [MAX 0-xxx(0)] [FLOOD xxx(num) xxx(sec) xxx(silence)] [MSG "xxx"]

[+/-(-)] 

Exclude or include ban.
- (default) refuses access ban.
+ allows access ban.

The first ban that fits the connecting player profile is acted on. 
Ban's are checked from the last added to the first added.  So the 
first bans to be checked are bans added manually at the console 
then the mod ban list (last to first) then the Quake2 ban list 
(last to first).



[NAME [LIKE/RE] "name"/BLANK/ALL(ALL)]

This is the format for a name ban. 

LIKE means that the only part of a players name must match the 
banned name for the ban rule to apply.  Like only applies to 
specified names, not BLANK or ALL.

RE means regular expression.  If you don't know what regular
expressions are leave it alone. If you wish to use '\' commands
in names you MUST do a '\\' for every '\' you want passed to 
regular expression.  e.g. a '\' in the search you be '\\\\'.
This is because of the '\' commands in names that I preprocess
for.  See section 5 for more details on Regular Expressions.

"name" is the nick or part nick to ban.

BLANK will pickup any name that is blank i.e. all spaces.

ALL is the default, meaning all names.  This is default so that 
when you specify a IP without a name it's applies to all names.


Example Name Bans:

BAN: - NAME BLANK
BAN: NAME LIKE "fuck"



[IP xxx[.xxx(0)[.xxx(0)[.xxx(0)]]/yy(32)]]

This is the format for a IP ban.

The /yy value says how many bits are fixed on the IP, i.e. a IP mask value.  
Basically the first x number of bits to compare to the connecting players 
IP.

0 = apply to all IPs. 
(0.0.0.0 mask)

8 = compare first number only. e.g. IP 192.168.1.10/8 would 
only compare  the '192' part of the IP. 
(255.0.0.0 mask)

16 = compare first two numbers only. e.g. 192.168.1.10/16 
would only compare the '192.168' part of the IP. 
(255.255.0.0 mask)

24 = compare first three numbers only e.g. 192.168.1.10/24
would only compare the '192.168.1' part of the IP.
(255.255.255.0 mask)

32(default) = compare the whole IP.
(255.255.255.255 mask)

If you don't specify an IP then the default IP of 'IP 0.0.0.0/0'
is used.


Examples of IP Bans:

BAN: - IP 192.168.1.10
BAN: + IP 192.10.1.1
BAN: - IP 192.168.1.10/24


[ALL]

Because the default BAN command allows for a default banning 
of everybody, I've disallowed banning all using the NAME / 
IP combo.  To ban everybody you must use the ALL command e.g.

BAN: ALL MSG "Server down for maintenance"
BAN: + ALL MAX 10


[PASSWORD "xxx"]

This applies only to allow access bans (+) and only allows users 
that have the correct password set in the client.

The password must be set on the client in the following manner before 
connecting or renaming:
set pw xxx u
where xxx is the password.  The password can't be bigger than 20 
characters and is case sensitive.

e.g.
say you had a ban:
ban + name re "^VK-.*$" password "duck" msg "Name reserved for Clan VK"

the client must have the password set like:
set pw duck u
to connect or rename to a name beginning with "VK-". e.g.
name "VK-Test"

NOTE:  because of the way Quake2 works in the area of servers seeing
client side variables, the password feature may NOT work with some mods.
If it does not you will start to see values being truncated. e.g. the 
clients name.  You can try to reduce the size of the password (within 
reason).


[MAX 0-xxx(0)] 

This applies only to allow access bans (+).  This allows up to xxx 
connections before refusing (banning) connections.  A MAX of 0 means 
unlimited connections and is the default.

e.g.
BAN: + IP 192.168/16 MAX 10

Will allow up to only 10 players to connect with ip of 192.168.x.x

e.g.
BAN: + ALL MAX 15
BAN: + IP 192.168/16 MAX 30

Will allow up to 30 from 192.168.* but only 15 with any other IP.


[FLOOD xxx(num) xxx(sec) xxx(silence]

Adding a flood option sets the clients chat flood protection (this 
overrides any global chat flood protection that are set).  The
options are the same as the standard "clientchatfloodprotect" command.

See section 2.7.4.


[MSG "xxx"]

This is the message to display in the log and displayed to the user.
Messages are only displayed to the user when a user changes their
name. Players ejected on connect can't be displayed a message.

e.g.
BAN: NAME BLANK MSG "No blank names allowed."


You can combine name and ip bans.  Say you want to only allow 1 
player with the name duck from a known IP.

e.g.
BAN: NAME "duck"
BAN: + NAME "duck" IP 234.231.34.154 MAX 2

(NOTE: the reason for MAX 2 is allow for crashes and reconnect so that 
duck doesn't have to wait for the Q2 server to kick the old player off)


Console Bans:

The ban format is the same as banfile format with some added extras.

[sv] !BAN [+/-(-)] [ALL/[NAME [LIKE/RE] name/%p x/BLANK/ALL(ALL)] [IP [xxx[.xxx(0)[.xxx(0)[.xxx(0)]]]/%p x][/yy(32)]] [PASSWORD xxx] [MAX 0-xxx(0)]] [FLOOD xxx(num) xxx(sec) xxx(silence] [MSG xxx] [TIME 1-xxx(mins)] [SAVE [MOD]] [NOCHECK]


The ban will only last until a level change or the server is shut down 
unless you use the save option.

Running it from the quake console 'sv !ban xxx' or from the 
client console in admin mode '!ban xxx'.

New options:

NAME [LIKE/RE] %p x

Where x is a client connection number.  It uses the name of the 
client as the ban name.

e.g.

sv !ban name %p 1


IP %p x[/yy]

Where x is a client connection number.  It uses the ip of the client 
as the ban IP.

e.g.

sv !ban ip %p 1


TIME 1-xxx(mins)

The ban will only last for x minutes, until the level changes or 
the server is shutdown.



SAVE [MOD]

"SAVE" by itself will cause the ban to be added last to the q2adminban.txt
in the Quake2 directory.  

e.g.

sv !ban name blank save


"SAVE MOD" will cause the ban to be added last to the q2adminban.txt in
the mod directory.

e.g.

sv !ban name blank save mod



NOCHECK

When a ban is added in either the client or the server console, all
connected clients are checked against all current bans.
If you supply 'NOCHECK' then the ban checking is not executed.



Banning detected ZBot/RatBot users:


This is how you would ban a ZBot/RatBot player on detect for 5 minutes. In the 
q2admin.txt file change the setting for customservercmd to something 
like:

customservercmd "sv !ban ip %p %c msg \"ZBot/RatBot user ban\" time 5"

The above command applies an IP ban on the players ip for 5 minutes.



From the server or client console you can list all the loaded bans
by using the 'listbans' command.
e.g.
sv !listbans
Start Ban List:
 (2, Temporary) BAN: NAME LIKE "test" TIME 9.831666
 (1, Temporary) BAN: + NAME RE "^VK-.*$" PASSWORD "duck"
 (0, Permanent) BAN: NAME BLANK
End Ban List



From the server or client console you can delete a ban from 
memory.  NOTE: this only deletes the ban from memory not from
the q2adminban.txt file.

The command is 'delban' and you have to know the ban number. 
To get the ban number do a 'listbans' command and it will
display the ban number.  e.g. the ban number of 
BAN: + NAME RE "^VK-.*$" PASSWORD "duck"
is 1.

To delete this ban from memory you run delban like this:

sv !delban 1



Name changing:
The way name changing is handled when a player changes to a banned 
name depends on the 'kickonnamechange' setting.  If it is set to
"No" then the name change is disallowed and the ban msg (default
or ban message) is displayed.  If it is set to "Yes" then the user
is displayed the ban msg and is kicked.




3. Chat Banning:

The q2adminban.txt format is:

CHATBAN: [LIKE/RE(LIKE)] "xxx" [MSG "xxx"]

This is the format for a name ban. 


[LIKE/RE(LIKE)] "xxx"

LIKE means that the only part of a message text must match the 
banned text for the ban rule to apply.  

RE means regular expression.  If you don't know what regular
expressions are leave it alone. If you wish to use '\' commands
in names you MUST do a '\\' for every '\' you want passed to 
regular expression.  e.g. a '\' in the search you be '\\\\'.
This is because of the '\' commands in names that I preprocess
for. See section 5 for more details on Regular Expressions.

"xxx" is the chat text to ban.


[MSG "xxx"]

Not working for the moment.


The chat banner is written in such a way that it will pick up
any chat message going to a client and not print them.  It
doesn't matter what special message features a mod may have 
it will work.  The problem with this method is that I don't know 
who generated the message.  



Reloading q2adminban.txt:

From the server or client console you can delete all the bans
in memory and reload the q2adminban.txt file using the
'reloadbanfile' command. e.g.

sv !reloadbanfile





2.7.3. Limited rcon
-------------------

Limited rcon is for admins that need to give out rcon to clans for 
competitions.

Limited rcon only allows specified commands to be executed.

The file q2adminlrcon.txt is read from the Quake2 directory and then the mod 
directory.  This allows server admins to set up global and 
mod specific lrcon commands.

The passwords are case sensitive but the command compare is not.

*** Warning ***
Using lrcon without the rcon_random_password set to yes means that people 
that know how to dump packets leaving your machine can see the rcon password.

rcon_random_password generates a random string for the rcon password for the 
one lrcon command.  The downside of this is that only 1 person can use an lrcon 
at any one time AND while a lrcon is in affect the rcon password is some random
string meaning the admin has no control over server at that point.

lrcon_timeout is used to control how only the rcon password is set to a random 
value when a lrcon is being used.
                                 
The format for q2adminlrcon.txt is as follows:

Lines beginning with a ';' are comments.

lrcon command format:
<lrcontype><password> <command to allow>

 <lrcontype> can be one of three values:
 'SW:'  Starts with match. 
    The start of the command must match: e.g. 'SW:lrcon test map' allows 
    lrcon test map q2dm1
    lrcon test map q2dm2
    lrcon test map 

 'EX:'  Exact match.
    The whole command must match: e.g. 'EX:lrcon test map q2dm1' allows
    lrcon test map q2dm1
    That will disallow:
    lrcon test map q2dm10
    lrcon test map q2dm2
    
 'RE:'  Regular Expression match. 
    It is suggested that you use the start of line command '^' to make sure it 
    matches the commands you want.  See section 5 for more details on Regular 
    Expressions.

e.g.
SW:secret map
SW:secret timelimit
SW:secret fraglimit
EX:top fraglimit 100
EX:top fraglimit 50
RE:tip ^map q2dm[1258]$

You can even specify exact commands that are allowed e.g. say you want 
lrcon password 'duck' only to be able to change maps to q2dm1, q2dm2 and
q2dm8.  The q2adminlrcon.txt would look like:

EX:duck map q2dm1
EX:duck map q2dm2
EX:duck map q2dm8

or use a regular expression like this:
RE:duck ^map q2dm[128]$


To run lrcon from the client console is the same as running rcon.
e.g.

lrcon duck map q2dm1

or

lrcon secret timelimit 40


There is a limit of 1024 lrcon password commands.

The only prerequisite for running lrcon is that the Quake2 server 
rcon_password must be set to *SOMETHING*. It doesn't matter what, 
it just has to be set.

Also, if the client has rcon_password set, lrcon will not work.
Note: there is no lrcon_password, so the password must be typed every 
the time.




There are 5 server console commands to control lrcon's.


[sv] !listlrcons

List all the lrcons loaded into memory.
e.g.
sv !listlrcons
Start lrcon's List:
   1 SW:"bad" "map"
   2 SW:"bad" "timelimit"
   3 SW:"bad" "fraglimit"
   4 EX:"test" "map q2dm1"
   5 RE:"tip" "^map q2dm[128]$"
End lrcon's List




[sv] !reloadlrconfile

Deletes all the lrcons in memory and reloads the q2adminlrcon.txt file.
e.g.

sv !reloadlrconfile



[sv] !lrcon [SW/EX/RE] "password" "command"

Adds a lrcon to the list in memory.

e.g.

sv !lrcon sw "test" "map"
sv !lrcon ex "duck" "map q2dm1"
sv !lrcon re "tip" "^map q2dm[128]\d"



[sv] !lrcondel lrconnum

Delete one lrcon out of memory.  The lrconnum is the number displayed leftmost
when running the lrconlist server console command.  NOTE: after the delete the
lrcon numbers higher than the delete lrcon will be reduced by one.

e.g.

sv !lrcondel 2








2.7.4. Chat Flood Protection and Muting
---------------------------------------

The q2admin chat flood protection and muting will work best if any mod flood 
protection is disabled.  

Q2admin does chat flood protection on 2 levels, a global level and a client
level.  The client level overrides the global level if it is enabled.  The
global level is controlled by the chatfloodprotect variable.  The client
level is controlled by the clientchatfloodprotect variable.  


The format for the chatfloodprotect is:

[sv] !chatfloodprotect [<number of messages> <in x seconds> <silence in seconds> / disable]


This variable can also be set in the q2admin.txt file.

A silence value of 0 will kick the client.  
A silence value of -1 will permanently mute the client.



The format for the clientchatfloodprotect

[sv] !clientchatfloodprotect [LIKE/RE/CL] name [<number of messages> <in x seconds> <silence in seconds> / disable]

This variable can also be set with include bans.  See section 2.7.2.

A silence value of 0 will kick the client.  
A silence value of -1 will permanently mute the client.


The flood message that is displayed if the client floods the server is controlled
by the chatfloodprotectmsg which can be set in q2admin.txt or the server/admin 
client console.



Flood protection automatically monitors the say, say_team, say_person, 
say_group, console messages and any mod command that generates print messages.


You can also enable monitoring of non-message command flooding. e.g. q2admin's
play_ commands, quake2 wave command, any mod added command.

To add commands you wish to flood monitor to the q2adminflood.txt file.

The file q2adminflood.txt is read from the quake2 directory and the mod 
directory.  This allows server admin's to setup lrcon's as global and 
mod specific as well.

The format for q2adminlrcon.txt is as follows:

Lines beginning with a ';' are comments.

flood command format:
<floodtype><command to check>

 <floodtype> can be one of three values:
 'EX:'  Exact match.
 'SW:'  Starts with match.
 'RE:'  Regular Expression match. 

e.g.
SW:play_
EX:wfplay
RE:^say.*

The matchs are only made against the first word or the command 
e.g. in the command "play_person cl 0 xxx" only "play_person" is 
checked against the flood command list

NOTE: DO NOT PUT THE FOLLOWING COMMANDS INTO THIS FILE:
say, say_team, say_group, say_person or any added mod command
that generates normal message text.

You can monitor q2admin own commands like:
play_person, play_team

Also note that internal commands to quake2 can't be monitored as
mod's don't see as client commands. e.g. name, cl_maxfps, rate, etc 

There is a limit of 1024 custom flood commands that can be setup.


The server / admin client console command 'reloadfloodfile' can
be used to delete all flood commands and reload the q2adminflood.txt
file.

[sv] !reloadfloodfile


The server / admin client console command 'listfloods' is used to 
list all the flood commands loaded.

[sv] !listfloods


The server / admin client console command 'floodcmd' is used to add a
flood command to the flood list loaded into memory.  

[sv] !floodcmd [SW/EX/RE] "command"

e.g.

sv !floodcmd SW "play_"


The server / admin client console command 'flooddel' is used to delete 
a flood command from the flood list loaded into memory.  

[sv] !flooddel floodnum

e.g.

sv !flooddel 1

You can use the listfloods command to find out with the flood number is.


The list of flood controlled commands are reloaded every level change.


With the mute server / client admin command you can stop a clients message 
commands and all the commands in the flood protected list from doing 
anything, the flood protection code does a automatic mute in effect.  The 
mute command can be used to timelimit mute / permanently mute  and unmute 
clients.

The format for the mute command is:


[sv] !mute [like/re/cl] name [time(seconds) / PERM]
This command will stop the client from messaging for time 
seconds.  

A mute time of 0 will unmute a client.

[PERM] option will permanently mute a client.






2.8. play_ and say_ commands
----------------------------

play_all, play_team and play_person commands are not server admin 
commands as such. They have added functionality for the clients to 
play wave files. printmessageonplaycmds enables the displaying of
a message when a client using any of the play_ commands. 


2.8.1 play_all
--------------

play_all <wavefile>
play_all will play the wave file on all the clients machines (if they
have the wave file).

play_all can be enabled / disabled with the play_all_enable admin 
command.



2.8.2 play_team
---------------

play_team <wavefile>
play_team will play the wave file on all the team client machines.  
This command will work the same as the say_team command.  This command
may not work for all team mods though.  It all depends on how the teams 
are programmed in the mod.  Try it, if it works, it works.

play_team can be enabled / disabled with the play_team_enable admin 
command.



2.8.3 play_person
-----------------

play_person [LIKE/RE/CL] <playername> <wavefile>
play_person will play the wave file on a specific players client.
play_person will only play the wave for the specified player.  
If more than 1 player name matches then no wave is played.

The play_team, play_all and play_person works the same as the play command 
so you can play wave files that are in pak files.

play_person can be enabled / disabled with the play_person_enable admin
command.



2.8.4 say_person
----------------

say_person [LIKE/RE/CL] <playername> <message>
This command allows a player to send a private message to another player.  The 
message is also sent the server console as well.  The say_person command will 
only send a message to one player.  If more than 1 player name matches then no 
message is sent.

[LIKE] The playername must be somewhere in the actual players name.
[RE]   Regular Expression match of a players name.  See section 5 for more 
       details on Regular Expressions.
[CL]   The <playername> must be a client number. This can only really be used
       by admin's that can run the rcon status command to find out the client
       number. The CL option is mainly for the !say_ server console commands.

say_person can be enabled / disabled with the say_person_enable admin
command.

say_person can also be used on the server console as well.  The console version
is always enabled.

[sv] !say_person [LIKE/RE/CL] <playername> <message>





2.8.5 say_group
----------------

say_group [LIKE/RE/CL] <playername> <message>
This command allows a player to send a private message to 1 or more other players.  
The message is also sent the server console as well.  To send to more than 1 player
with completely different nick's you will have to use Regular Expressions and the \|
command. e.g. say_group re "fred\\|john\\|ted" yo yo, what's up guys?

[LIKE] The playername must be somewhere in the actual players name.
[RE]   Regular Expression match of a players name.  See section 5 for more 
       details on Regular Expressions.
[CL]   The <playername> must be a client number. This can only really be used
       by admin's that can run the rcon status command to find out the client
       number.  Multiple clients can be selected by using the '+' between the 
       client numbers. 
       e.g. CL 0+2+14+3
       Note there are NO spaces between the numbers and '+'s.
       The CL option is mainly for the !say_ server console commands.

say_group can be enabled / disabled with the say_group_enable admin
command.


say_group can also be used on the server console as well.  The console version
is always enabled.

[sv] !say_group [LIKE/RE/CL] <playername> <message>







2.8.6 extended say
------------------

Extened say allow you to run the say_person, say_group, play_all, play_team and 
play_person from the messagemode or the messagemode2 prompts.  To enabled 
extended say checking you set the extendedsay_enable variable to yes in the 
q2admin.txt or in the server / client admin console.

To use any of the supported commands you must also enable the command as well. 
e.g. to use say_person you must enable extendedsay_enable and say_person_enable.


say_person format:

!p [LIKE/RE/CL] <playername> <message>


e.g. at the messagemode prompt you type:
!p like tom hi tom


say_group format:

!g [LIKE/RE/CL] <playername> <message>


e.g. at the messagemode prompt you type:
!g re ^VK-.*-$ meeting tonight... normal place, normal time..


play_all format:

!a wavfile


e.g. at the messagemode prompt you type:
!a hiall


play_team format:

!t wavfile

e.g. at the messagemode prompt you type:
!t attack


play_person format:

!w [LIKE/RE/CL] name wavfile

e.g. at the messagemode prompt you type:
!w like tom hi





2.9. Message of the Day (MOTD)
------------------------------

The MOTD feature in the Q2Admin is very, very simple.  There isn't 
much to it and is meant to do two things:

1. Provide a simple MOTD features for mods that don't have a MOTD.
2. Provide a global MOTD for admin's that want a global MOTD.

The setmotd command is used to set name of the MOTD file.  This file is
loaded from the Quake2 directory. If you wish to have a unique MOTD per 
mod then I suggest you add a following line into you server startup 
config and leave the setmotd in the q2admin.txt file blank.

sv !setmotd "xxx\motd.txt"
where xxx is the mod directory.

The MOTD file is reloaded with every level change.
The MOTD file can be reloaded with the above command as well.

Clients can re-view the MOTD at any time by using the "motd"
command at the client console.

The length of time the message stays displayed is controlled by a client 
console variable 'scr_centertime'.  The default is 2.5 seconds.






2.10. disable spawning items
----------------------------

To enable the spawn disable feature you must set the 'spawnentities_enable' 
variable in q2admin.txt to 'yes'.

Entity types can be disabled from loading in a map.  It stops the entity from 
loading *IF* is loaded from the map.  If it's loaded from another source it 
can't.  If the mod supports ent files or some other method of entity 
loading then it can't stop that.  You will have to use there method to stop 
it loading.  e.g. if it uses ent files then you will have to edit the ent file.

There is also a feature for disabling entities as they are created in memory.  
This feature has some risks involved as "entity_classname_offset" must be set 
correctly.  This variable MAY be different for every mod that exists.  Most 
likely tho 99% of mods will have the same value.  There is no way to calculate 
or figure out this variable.  The easiest way to see work it out is to send 
killerbee@planetquake.com a email stating the mod and asking what the correct
number is. Also disabling some entities may cause the mod to do strange things.
To enable this feature you must have 'spawnentities_enable' and 
'spawnentities_internal_enable' set to 'Yes'.  

To help work out what entity classnames may be there is a logging feature that 
will log the classnames as they are created.  

The file q2adminspawn.txt contains a list of entity classnames that are 
disabled.  You just need to edit the file and follow the instructions.

In q2adminspawn.txt is listed all the entity classnames for quake2 that ID
provide.  For other mod's you will have to look up the mod documentation for 
the classname list.  

The q2adminspawn.txt is treated a little differently from the other txt config 
files.  The file is only loaded once at server startup or when the map command 
is used.

You can also load a disable spawn entity list for a specific maps.  To create a 
spawn disable list for a specific maps you must create a "q2adminmaps" directory 
off the mod directory.  In this directory you need to create a spawn file with the
name of "<mapname>.q2aspawn".  The format for this file is exactly the same as 
q2adminspawn.txt. e.g. If the mod is the default quake2 dm and you wanted to 
disable the grenade launcher on q2dm1 only then you would:
Create the directory "baseq2/q2adminmaps".
In that directory you would create the file "baseq2/q2adminmaps/q2dm1.q2spawn".
In the file "baseq2/q2adminmaps/q2dm1.q2spawn" you would have the line
"EX:weapon_grenadelauncher".


Disabled spawn classnames can be managed with the 'spawncmd', 'spawndel', 
'listspawns' and 'reloadspawnfile' commands. 

spawncmd can be used to add a spawn classname to the disabled list.  Note that
the entity type is NOT disabled right then, it will only be disabled on the 
next map onwards.

spawndel can be used to delete a spawn classname from the list.  Note that the
entity type will appear from the next map onwards.

reloadspawnfile is used to delete and reload the spawn list.

listspawns is used to list all the spawns classnames loaded.




2.11. command voting
--------------------

To enable the voting feature set 'vote_enable' in the q2admin.txt file to
'Yes'.

Any command you wish people to allow to vote on can be added to the 
q2adminvote.txt file.  You can add any command that can be run on the
server.  There are three types of command setups that you can use.

Extract match type commands.  e.g. 'EX:map q2dm1' meaning that a client 
can only vote for 'map q2dm1' and nothing else.

Starts with type commands.  e.g. 'SW:map' meaning that a client 
can vote for any command start with map.  e.g. 'map q2dm1', 'map q2dm3'

Regular expressions commands.  e.g. 'RE:^map q2dm[123456]$'  The most powerful
way to restrict commands.

The client command used to do vote is set by the 'clientvotecommand' and
is 'vote' by default.  

The voting is setup with a number of variables.
'clientvotetimeout' is the length of time that the vote will last for until
it will voting ends.  It can be shorter then this if everyone votes.

votepasspercent sets the pass percent required to pass a vote.
e.g. 60 is 60% of the counted clients need to vote yes.

'votecountnovotes' says if clients that didn't vote are counted as no votes 
or not counted at all.
e.g. if there are 10 clients and the passvotepercent is set to %50.  If 4 
people vote 'yes' and 2 people vote 'no' and the rest didn't vote.
If 'votecountnovotes' is set to 'Yes' (default) the calculation goes like so:
4 / 10 == 40% < %60 == Vote Failed.
If 'votecountnovotes' is set to 'No' the calculation goes like so:
4 / (10 - 4) == 66% > %60 == Vote Passed.

'voteminclients' is the minimum number of clients that have to be connected 
before a vote can be proposed.

'clientremindtimeout' is frequency at which clients are reminded to vote. 
Every 'clientremindtimeout' seconds since the vote has started until the 
vote is complete.

The vote commands can be managed with the 'votecmd', 'votedel', 'listvotes'
and 'reloadvotefile'.

'votecmd' is used to add vote commands.
'votedel' is used to delete a vote command.
'listvotes' can be used to list all the vote commands available.
'reloadvotefile' deletes and reloads all the vote commands.

q2adminvote.txt is loaded can each level change.
 

On the client side the voting works as follows.  Note the clientvotecommand
is assumed to be set to 'vote'.

A vote can be proposed by typing in a vote followed by a valid vote command.

e.g.

vote map q2dm1

All the clients are informed of that they need to vote, how to vote, and
the current vote stats.

The person the proposed the vote is automatically voted 'yes'.

Other clients can vote by typing in the console:
vote yes
or 
vote no

They can change there vote at any stage before the vote ends.

Every 'clientremindtimeout' the reminder message, vote proposed and the 
current vote stats is displayed to clients that haven't voted and only the 
vote proposed and the vote stats are displayed.

When everyone has voted or the clientvotetimeout is up the vote is passed or
failed.  The pass / fail and the vote stats and displayed to all clients.

If the vote has passed, there is a 5 second gap before the vote command is 
run on the server.

The length of time the vote messages stays displayed is controlled by a 
client console variable 'scr_centertime'.  The default is 2.5 seconds.



2.12. disable client commands
-----------------------------

To enable the disable client commands feature you must set the 'disablecmds_enable' 
variable in q2admin.txt to 'yes'.

Disabled client commands are filtered out so that they do not reach the mod.  This is
good for removing any mod command that you do not wish the client to use.  Anyone
using a disabled command will see nothing.  The use of a disabled command is logged
to the server console and the can also be logged to the q2admin log.  See section 4
on logging for further details on the q2admin log setup.

The file q2admindisable.txt contains a list of disabled commands that are 
disabled.  You just need to edit the file and follow the instructions.

Disabled client commands can be managed with the 'disablecmd', 'disabledel', 
'listdisable' and 'reloaddisablefile' commands. 

disablecmd can be used to add a disabled command to the disabled command list.  

disabledel can be used to delete a disabled command from the list.  

reloaddisablefile is used to delete and reload the disabled command list.

listdisable is used to list all the disabled commands loaded.



2.13 cl_pitchspeed and cl_anglespeedkey (Action Quake2 M4 Scripts)
------------------------------------------------------------------

There is a Action Quake2 cheat that allows M4 Not to kick up when used.

This script changes the clients cl_pitchspeed or cl_anglespeedkey.  To enable 
the detection of changing cl_pitchspeed you need to edit the q2admin.txt 
file and change "cl_pitchspeed_enable" variable to "Yes".
To enable the detection of changing cl_anglespeedkey you need to edit the q2admin.txt 
file and change "cl_anglespeedkey_enable" variable to "Yes".

"cl_pitchspeed_display" will display when someone changes the value.
"cl_pitchspeed_kick" if the person is kicked when they change the value.
"cl_pitchspeed_kickmsg" what the kick message is.

"cl_anglespeedkey_display" will display when someone changes the value.
"cl_anglespeedkey_kick" if the person is kicked when they change the value.
"cl_anglespeedkey_kickmsg" what the kick message is.

The log will log them as a zbot use with a value of '-7'.

Changing the cl_pitchspeed value to "150" (the normal value for cl_pitchspeed)
is ignored (this is for technical reasons).

Changing the cl_anglespeedkey value to "1.5" (the normal value for cl_anglespeedkey)
is ignored (this is for technical reasons).



2.14 client side variable checking
----------------------------------

To enable client side variable commands feature you must set the 
'checkvarcmds_enable' variable in q2admin.txt to 'yes'.

This feature allows you to force any client side variable to be a constant value
or a between a range for numeric variables.  You can setup which variables you 
want to check in the q2admincheckvar.txt file.  Q2admin implements this feature 
by polling the variables every XX seconds.  This means that q2admin isn't using
up userinfo space.  You can set the polling frequency using the 'checkvar_poll_time'
variable in q2admin.txt.

If there are 5 variables listed in q2admincheckvar.txt, the first variables is 
checked first, x seconds later the next line is checked and so on until there are no
more variables to check.  It then loops back to checking the first variable again.
So if checkvar_poll_time is 60 seconds and there are 5 variables listed, each 
variable will be checked one every 5 minutes.  Reducing this variable will increase 
the traffic between the client and the server.

The file q2admincheckvar.txt contains a list of client side variables to check.
You just need to edit the file and follow the instructions in the default 
q2admincheckvar.txt.

Client side checked variables can be managed with the 'checkvarcmd', 'checkvardel', 
'listcheckvar' and 'reloadcheckvarfile' commands. 

checkvarcmd can be used to add a client side variable to the variable list.  

checkvardel can be used to delete a client side variable from the list.  

reloadcheckvarfile is used to delete and reload the client side variable list.

listcheckvar is used to list all the client side variables loaded.




3. String preprocessing.
------------------------

What values to pass can be a little confusing. Just about all the strings 
passed into q2admin are preprocessed. There are many reasons for q2admin 
to do this.  The major ones are to allow values to be passed though from 
the quake2 console that normally aren't allowed.  To allow special values
to be passed that would not normally be known until runtime.

All the values in a string beginning with '\' are replaced. The current 
replacement list is:

'\\' replaced with '\'.
'\q' replaced with '"'.
'\"' replaced with '"'. (This can't be used in the quake2 console)
'\n' replaced with a newline.
'\s' replaced with a space (' ').
'\m' replaced with the current mod directory.

Some examples of uses:

in the q2admin.txt file:
customservercmd "error \"Get off my server, ZBot/RatBot user!\""

in the quake2 client q2admin admin console:
!customservercmd "error \qGet off my server, ZBot/RatBot user!\q"

in the server console:
sv !cvarset test \s\s\s

in q2adminban.txt file:
INCLUDE: "\m\\includebanlist.txt"

in the server console:
sv !stuff cl 0 "say \qyo yo\q\nsay \qhi hi\q"





4. Logging explained
--------------------

Logging is all controlled by one file, "q2adminlog.txt".  
There can be up to 32 log files and one log event can go to
any of the 32 log files (including one/some/all of them).

The layout for the file is split into two parts.  One is
setting of the log files.  This must go at the start of 
the file before any of the logevents can be setup.

The format for log file setup is:
LOGFILE: LogNum [MOD] "LogFileName"

LogNum: is a number between 1 to 32.
MOD: means the log file is saved into the mod directory 
     instead of the Quake2 directory.
"LogFileName": is the log file name.  This can include 
     directories. Also you can use '%p' in the file name. 
     This is replaced by the server port number to give a 
     unique log for each server within the same mod 
     that is run.

e.g.

LOGFILE: 1 "q2admin.log"
LOGFILE: 2 MOD "q2admin%p.log"
LOGFILE: 5 MOD "entity.log"



The second part is setting up the log events layout themselves.  
Each log event can only be set up once and log events not set up 
are assumed to be not logged.

The format for a log event is:

[logtype]: YES/NO lognum [+ lognum [+ lognum ...]] "format"

logtype:  is the specific log event that we are logging.
YES/NO:   log this event or not.
lognum:   is a number between 1 to 32.  Says which log file(s) to 
          log this event to.  Multiple log files can be assigned by
          using the '+' sign. e.g. 1 + 2 + 3 + 4
"format": the string to print to the log file.  This can have a number
          of replacement fields.  


Log types:

Here is the list of log types that can be used:

ZBOT

  Logged when a ZBot or RatBot user is detected.

ZBOTIMPULSES

  Logged when someone uses ZBot impulses.

IMPULSES

  Logged when someone uses impulses other than ZBot impulses.

NAMECHANGE

  Logged when someone changes there name.

SKINCHANGE

  Logged when someone changes there skin.

CHATBAN

  Logged when a message is banned.

CLIENTCONNECT

  Logged on client connect.

CLIENTBEGIN

  Logged on client begin.

CLIENTDISCONNECT

  Logged on client disconnect.

CLIENTKICK

  Logged when the Q2Admin dll kicks someone for any reason.

CLIENTCMDS

  Logged when a client sends a command to the server.

CLIENTLRCON

  Logs when a client runs a lrcon command.

BAN

  Logged when a client is banned (ip or name)

CHAT

  Logged when chat messages are used.

SERVERSTART

  Logged when the server is started.

SERVERINIT

  Logged when the server is inited.

SERVEREND

  Logged when the server is shutdown.

INTERNALWARN

  Logged when the Q2Admin dll generates certain types of 
  internal warning.  Used for internal testing of problems.

PERFORMANCEMONITOR

  Logged for all the major quake2 -> mod function calls. 
  Should only be used for small periods of time. It is meant 
  to collect / pin-point performance problems with q2admin 
  and the underlying mod.

  This option will create very large log files in a very short 
  period of time.

DISABLECMD
  
  When someone tries to run a disabled client command.

ENTITYCREATE
  
  When a entity is created.  "entity_classname_offset" must be 
  correct for the classname to be correct.

ENTITYDELETE

  When a entity is deleted.  "entity_classname_offset" must be 
  correct for the classname to be correct.



"format" can include the following replacements:

#n = Client Name
#p = Client Ping
#i = Client IP
#r = Client Rate
#s = Client Skin
#t = Date / Time
#m = Impulse Message (ZBot impulse only)
#e = Impulse Number (impulse and ZBot impulse only)
#e = Internal ZBot/Ratbot detect number 
     (50 to -2 is a zbot detect, -3 or -4 is a ratbot detect, -5 is a Nirto2 / BW-Proxy / Xania)
#e = Internal warning number (internal warning only)
#f = Time in seconds to complete function (performance monitor only)
#m = Internal warning message (internal warning only)
#m = Old Name (name change only)
#m = Old Skin (skin change only)
#m = Kick Reason (kick only)
#m = Banned chat (chat ban only)
#m = Client command (Client commands only)
#m = lrcon command sent (lrcon only)
#m = ban message (ban only)
#m = chat message (chat only)
#m = performance monitor function (performance monitor only)
#m = command that was tried to run (disabled command only)
#m = entity classname (entity create or entity delete command only)

Examples of event log setups are:

ZBOT: YES 1 "Proxy User: Time \"#t\" name \"#n\" Ping \"#p\" IP \"#i\" I(#e)"
ZBOTIMPULSES: YES 1 "ZBOT Impulse: Time \"#t\" Impulse \"#n generated a impulse #m\" Ping \"#p\" IP \"#i\""
IMPULSES: YES 1 "Impulse: Time \"#t\" Impulse \"#n generated a impulse #e\"  Ping \"#p\" IP \"#i\""
NAMECHANGE: YES 1 "Name: Time \"#t\" \"#m -->> #n\"  Ping \"#p\" IP \"#i\""
SKINCHANGE: YES 1 "Skin: Time \"#t\" Name \"#n\" \"#m -->> #s\"  Ping \"#p\" IP \"#i\""
CLIENTCONNECT: YES 1 "Connect: Time \"#t\" Name \"#n\"  Ping \"#p\" IP \"#i\" SKIN \"#s\""
CLIENTBEGIN: YES 1 "Player Begin: Time \"#t\" Name \"#n\"  Ping \"#p\" IP \"#i\""
CLIENTDISCONNECT: YES 1 "Disconnect: Time \"#t\" Name \"#n\"  Ping \"#p\" IP \"#i\""
INTERNALWARN: YES 1 "Internal Warning(#e): Time \"#t\" Name \"#n\"  Ping \"#p\" IP \"#i\" #m"
CHATBAN: YES 1 "chat ban :#n  \"#m\""
CLIENTKICK: YES 1 "kick #n"
CLIENTCMDS: YES 1 "cmd #n \"#m\""
BAN: YES 1 "ban #n \"#m\""
CHAT: YES 2 "chat #n \"#m\""
SERVERSTART: YES 1 + 2 "Server Startup #t"
SERVERINIT: YES 1 "Server Init #t"
SERVEREND: YES 1 + 2 "Server Shutdown #t"
PERFORMANCEMONITOR: YES 3 "Performance: Time \"#t\" Time taken(sec) \"#f\" Function \"#m\" Name \"#n\"  Ping \"#p\" IP \"#i\""
DISABLECMD: YES 1 "Disabled command: Time \"#t\" Name \"#n\"  Ping \"#p\" IP \"#i\" Command \"#m\""
ENTITYCREATE: YES 5 "Create Entity: Time \"#t\" classname: \"#m\""
ENTITYDELETE: YES 5 "Delete Entity: Time \"#t\" classname: \"#m\""




Logging can also be controlled through the use of two server console commands 
'logevent' and 'logfile'.



logfile:

The format for this command is:

[sv] !logfile view <logfilenum>

  This command lists all the log files setup or just one log file.
  e.g.
  sv !logfile view
  sv !logfile view 1

or

[sv] !logfile edit [filenum(1-32)] [mod] [filename] 

  This command will add/edit the logfiles.
  e.g.

  sv !logfile edit 1 mod q2admin.log
  sv !logfile edit 2 q2adminchat.log

  (NOTE: the %p doesn't work here, it only works in the q2adminlog.txt.  It's 
         easy enough to find out the port from the console with the port 
         command)

or

[sv] !logfile del [filenum(1-32)]

  This will remove the logfile from being used.  This does not delete or touch 
  the logfile in any way.  It just means the log file will not be used any more.

  e.g.
  sv !logfile del 1



logevent:

The format for this command is:

[sv] !logevent view <logtype>

  This command lists all the log events setup or just any log events starting with logtype.
  e.g.
  sv !logevent view
  sv !logevent view zbot
  sv !logevent view client


or

[sv] !logevent edit [logtype] <log [yes/no]> <logfiles [logfile[+logfile...]]> <format \"format\">]

  This command will edit any of the log events. 
  e.g.
  sv !logevent edit zbot log yes
  sv !logevent edit zbot log no
  sv !logevent edit zbotimpulses logfiles 1+2+3
  sv !logevent edit clientconnect format "Connect: Time \q#t\q Name \q#n\q  Ping \q#p\q IP \q#i\q SKIN \q#s\q"
  sv !logevent edit chatban log yes logfiles 3 format "chat ban :#n  \q#m\q"






5. Regular expressions
----------------------

Some parts of q2admin allow the use of regular expressions to 
match data.  The RE syntax that is allowed in q2admin is the
POSIX extended syntax.  

Regular expression syntax is to big of a topic to include this
readme.

To find out more about regular expressions you can download 
the GNU RE document that comes with the GNU RE source.  Beware,
this document is written with programmers in mind, not users.  
It does detail the RE syntax though.

The document can be downloaded from the web site under the
files section.

You can also check out a on-line version of the documentation 
at 

http://yenta.www.media.mit.edu/projects/Yenta/Releases/Documentation/regex-0.12/xregex_toc.html








6. Trouble Shooting
-------------------

Q. I get the server console message "Info string length exceeded" 
   after adding q2admin to the server.  
A. There is a limited space available for server information 
   strings that problems like gamespy display.  Most likely you
   have info strings that give details about your server e.g.

   set admin "Shane Powell, KillerBee" s

   When you exceed this info space you get the "Info string 
   length exceeded" message on the server console.  q2admin
   adds 1 info string: "set Bot "No Bots" s".  This is most
   likely the problem.  You can do 1 of 2 things:

   1. Shorten or remove some of your info strings.
   2. Turn off the q2admin info string by editing the 
      q2admin.txt file and changing the "serverinfoenable" 
      variable to "No". e.g.

      serverinfoenable "No"

   The above problem is more common with q2 mod's that add it's 
   own server info strings.  e.g. Action Quake2


Q. The maximum length of the usernames is smaller than the default
   15 characters after I add the q2admin mod.
A. Quake2 provides a way for the server to be dynamically updated
   whenever a cvar changes.  (e.g. the name cvar)  Only some of the
   cvars are marked to be dynamically updated.  You can see what 
   cvar's that are marked by doing a cvarlist at the console.  The 
   left hand column there are letters by some of the  cvars.  The 
   ones with the 'U' are marked for dynamic updating.  The problem
   is that quake2 only has a very small amount of space set aside 
   for dynamic updating.  When this space is overflowed the values
   of the cvar are truncated.  The only one that stands out though is
   the name cvar.  Basically if the mod is marking cvars as 'U' and
   q2admin marks cvars as 'U' there may be a overflowing.  To fix 
   this you have to reduce the amount of cvars marked as 'U'. You
   can either do this in q2admin or in mod. I would assume that most 
   mods wouldn't give you the option though.  There are 2 features in
   q2admin that mark cvars as 'U'.  The first is the maxfps feature
   and the second is the passwords on ban names.  Try reducing the 
   password length and/or turning off one of both features. 










7. Credits
----------

Written by Shane Powell (KillerBee) 
killerbee@Quake2.bitstreet.net
http://www.teamevolve.com


Mike (McNasty) for beta testing and for bugging me enough 
to write this dll in the first place.

#Cookie for input and advice.

Mentor for allowing me to beta-test on his server.

ElectricMonk for programming advice and input.

Matt (Fisty) for porting to linux.
HeKTik for letting me use his linux machine for compiling 
on demand.

Chief Bastard for linux server testing.

The Solaris port is done by the SolarEclipse.  Go check 
them out for ports of other Quake2 mod's to Solaris OS.
http://www.planetquake.com/eclipse

For the many many server admin's and users that have reported 
problems and made feature suggestions.

A very big thanks goes to the alpha test team that helped me 
make the move from zbot detector to q2admin.  They are the 
people that have made q2admin what it is today.

Brian (Spanky aka Brain), Apocalypse, Thaddeus, Simon, John, 
Sentry, Brandon (Thumper), Michael, Mentor, Zool, James (GenChaos), 
Grozny (Gsu27), Ron (Iron), Phil (Deltoid), Mark (Spiff), Paul 
(NeDDoG), Ian, Jarmo

And finally [KfK]Vithar for modifying the version 1.10 source and
adding the extra protection measures that make up version 1.11.






8. Copyright and Distribution Permissions
-----------------------------------------

This program is freely distributable provided that this 
readme.txt is distributed with it as well and remains unchanged.

All code is Copyright Shane Powell 1998.

DISCLAIMER:  This program is 100% safe, tested, and FDA 
approved, so you should have no trouble with it. But if 
you do.. its not our fault.

DLL's always carry an inherent risk for servers!

The q2admin and the q2admin source code is provided under the 
GNU GENERAL PUBLIC LICENSE. 






9. Release History
------------------

v0.92 beta  - 19 October     - First public release for Win32 version
v0.93 beta  - 21 October     - First public release for Linux version
v0.94 beta  - 22 October     - Fixed bug with normal dm servers not 
                               loading. Win32 version only atm, linux 
                               update soon.
v0.94 beta  - 23 October     - Renamed to 'UQSA ZBOT Detector' and moved 
                               to a new web site and the release of the 
                               linux release.
v0.95 beta  - 26 October     - Added Quake2dirsupport for windows version 
                               only in ZBotdetect.cfg.
                             - Added customclientcmdconnect to ZBotdetect.cfg
                             - Improved performance.
                             - Fixed command overflow problem.
v0.96 Alpha - 01 November    - Fixed blank IP's in log file.
                             - Log and display name changes now.
                             - Kick on client impulse generate option 
                               (default off).
                             - "!version" command added.  Can be used by any 
                               client you don't have to be in admin mode to run.
                             - Added support for double quotes '\"' and newlines 
                               '\n' in command strings. 
                               e.g. customclientcmd "error \"Fuck off ZBOT User\""
                             - Use log file in mod dir option added (default set 
                               to Quake2 directory)
                             - Added ZBotmotd option, this is a file that is 
                               printed to each client on connect and reconnect 
                               (level change).
                             - Checks for overflow now, to do this I force 
                               "logfile 2" on servers.
                             - Added 'play_team' command and 'play_team_enable' 
                               option.
                             - Added 'play_all' command and 'play_all_enable' 
                               option.
                             - Added serverinfoenable with tells the dll to set 
                               the server variable Bot "No Bots".
v0.96 Alpha - 16 November    - Added server console admin interface.
                             - Added support for string admin commands in the 
                                   server console and client console. Added '\q' for 
                               inserting quotes in strings in the console.
                             - Added support for detecting hacked ZBots.
                             - Changed the 1/0 settings to Yes/No for easier to
                               understand admin commands.
                             - Added IP and Name banning.
                             - Added rate and maxfps capping.
                             - Added limited rcon.
                             - Added conned / disconnect logging.
                             - Added selectable impulses to kick on.
                             - Rewrote the readme.txt
v0.96 Alpha - 23 November    - Break logclientconnects into 3 options.
                             - Added BAN ALL command.
                             - Changed BAN IP MASK format to /yy format
                             - Added ZBotmotd command to server & client console.
                             - Prints out the ZBot version on server startup.
                             - Fixed problem with 'gamemap' command (e.g. timelimit,
                               fraglimit)
                             - Added reload ban list command.
                             - Added reload lrcon list command.
                             - Added motd client command to redisplay motd.
                             - Fixed long pauses before certain commands would
                               complete.
                             - Disable client admin console command (NO by default)
                             - Fixed bug with incorrect game dll being loaded in
                               rare cases.
                             - Changed format for some of the logging output to make
                               it nicer I hope :)
                             - Added displaylogfile to server console commands.
                             - Added command to display bans in memory.
                             - Added command to display lrcons in memory.
                             - Added command to delete ban.
                             - Added option (banonconnect) on how banned connecting 
                               players are handled.
                             - Changed name changing to disallow instead of kicking
                               when changing to a banned name.
                             - Adding a ban at the console now reruns all the bans on
                               currently connected clients.
                             - Added nocheck to ban console to NOT rerun the ban on
                               currently connected clients.
                             - Added option to protect against name change flooding.
                             - Added regular expressions to ban names.
                             - extended lrcons format with exact match and regular
                               expression matching.
                             - Added passwords option to include bans.
v0.96 Alpha B4 - 27 November - Fixed MSG message for bans with passwords not displaying.
                             - Fix ip banning disabled and name banning enabled problem. 
                               (Disabled now ignores ip bans with a subnet mask greater 
                               than 0) 
                             - Added build number to version string.
                             - Fixed server crash if banonconnect yes set to 'No' and 
                               the user cash banned. Only some types of mod's will be 
                               effected by this bug namely lmctf. 
                             - Fixed "ban all" bans loading incorrectly from ZBotban.txt 
                               file. 
                             - Fixed problem with "internal error" and the false detection 
                               that can go with it. 
                             - Setting the user admin password (adminpassword) to nothing 
                               will disable client side admin console.   This replaces the 
                               separate variable what was in the Build 3.
                             - Option added to kick on name change to banned name. 
                             - Change flood protection to be more like quakeworld (number,
                               time, seconds). 
                             - Chat filter. 
v0.96 Alpha B5 - 9 December  - Fixed some small bugs with the detection methods causes 
                               internal errors.
                             - Fixed the linux crashing problem.
                             - Improved detection method be allowing for more cases and 
                               increased the retry count.
                             - ZBotdetect.cfg is now loaded from the Quake2 directory AND then 
                               the mod directory.
                             - Removed the setting of the logfile and recommend setting it to two 
                               or 3 in the readme.txt.
v0.96 Alpha B6 - 14 December - Fixed bug in build 5 where banonconnect set to no didn't kick 
                               the person.
                             - Fixed up some internal error (4)'s from happening.
                             - Fixed up a big code 'doh' for not catching times when internal 
                               errors could happen. 
v0.96 Alpha B7 - 17 December - Changed the 'ZBotmotd' command to 'setmotd'.
                             - Changed the 'ZBotdetect.cfg' file to 'q2admin.cfg'.
                             - Changed the name from ZBot detector to q2admin. 
                             - Changed the file 'ZBotban.txt' to 'q2adminban.txt'.
v0.96 Alpha B8 - 23 December - Finished off the chat banning so that MSG part of the chat ban 
                               works now.
                             - Added advanced logging.
                             - Deleted all the old logging commands as they are not needed any 
                               more.
                             - play_person [LIKE/RE] <playername> <wavefile> command added.  
                             - say_person [LIKE/RE] <playername> <message> command added.  
v0.96 Alpha B9 - 29 December - Fixed bug that caused a server crash when players changed names 
                               to names longer than 15 characters.
v0.96 Alpha B10- 11 January  - Fixed bug where setmotd command from the server or client 
                               console would not load the motd file correctly.  Also stuffed 
                               up the motd client command.
                             - lrcon.txt file renamed to q2adminlrcon.txt.
                             - lrcon password now made case sensitive.  Because the password is 
                               now separate from the command, the RE command format doesn't begin 
                               with a '^' before the password any more. 
                               e.g. "RE: ^duck2 map q2dm[128]$" is now "RE: duck2 ^map q2dm[128]$"
                             - Fixed bug where client console commands (e.g. motd or lrcon or 
                               admin command !) didn't work for up to a second after connecting.
                             - ban passwords are now case sensitive.
                             - readme.txt worked over by -=Phil to correct my english.
                             - Fixed linux crash in Build 9.
                             - Fixed problem with internal error (1) coming up for no reason.
                             - Changed name from internal error to internal warning.
                             - Allowing running from a moddir which doesn't have a game dll or so.
                               (i.e. the loading now defaults to baseq2 if the game dll/so doesn't
                               exist in the mod directory)
v0.96 Alpha B10- 13 January  - Fixed problem with linux and created a test server code to test the 
                               linux version before releases.  
                             - Re-released the linux build 10.
v0.96 Alpha B10- 18 January  - Build 10 port to x86 and sparc solaris.
v0.96 Alpha B11- 18 January  - Switched to the GNU regular expression code.  The RE mode that I use 
                               is the POSIX extended mode.
                             - Fixed '#m' for kick log messages not displaying any message.
                             - Changed some of the ban connect code to be more mod friendly.
                             - Added lrcon client command logging.  This logs all correct lrcon 
                               commands sent to the server.
v0.96 Alpha B12- 19 January  - Fixed the I(85) zbot false detection bug.
v0.96 Alpha B13- 20 January  - Fixed the RE not working bug.  
                             - Changed the cl_maxfps code so that it doesn't require the setting of 
                               the cl_maxfps var for unsetup clients.
v0.96 Alpha B14- 21 January  - Fixed client crashing after 3 seconds from connect.  Was caused by the 
                               cl_maxfps code changed :(
                             - Added rate and cl_maxfps descriptions to the readme.txt file.
                             - Added a Trouble Shooting section to the readme.txt file.
v0.96 Alpha B15- 22 January  - Fixed lrcon not being run correctly if the logging of lrcon's was 
                               turned on bug.
v0.96 Alpha B16- 2  February - Added performance monitor logging option.
v1.0 RC1       - 9  February - Fixed hacked zbot checking.
                             - Expanded the "impulsestokickon" server/client console command.
v1.0           - 12 February - Clean up some of the cfg files and readme.txt.
                             - Minor cleanup code changes.
                             - First public release.
v1.1 Beta      - 19 February - Added '%p' to the log file name setup.  This is replaced by the server 
                               port number.
                             - Added 'stuff' command to the server console / client admin console 
                               commands.
                             - Added 'customservercmdconnect' to the config / server console / client 
                               admin console commands.
                             - Added maximpulses option to kick after x number of impulses generated. 
                             - Added server console commands '!say_person' and '!say_group'.  Also 
                               added 'CL' to all the play_ say_ commands for specifying client numbers 
                               instead of names.  Mainly for use with the server console versions of 
                               say_ instead of typing in names.
                             - Added warnings when any of the config files don't load.
                             - Fixed bug where say_person bypassed the chatban checking.
                             - Added extended say where say_group and say_person can be run from the say 
                               and messagemode commands.  This is mainly for the messagemode command so 
                               you don't have to bring down the console to do a say_person or say_group.
                             - Added logfile and logevent server console commands to setup/maintain log 
                               files.
                             - Added lrcon and lrcondel server console commands to setup/maintain 
                               lrcon's.
v1.1 Beta 2    - 20 February - Fixed problem with the stuff command sending it to the incorrect client.
                             - Chat logging now knows who sent the message.
                             - Fixed bug with the extended say on. (so much for coping id code and 
                               guessing that is would work ok... doh)
                             - Added chat flood protection.
                             - Added gamemap to map option.
v1.1 Beta 3    - 25 February - Fixed the server console sv !say_person and sv !say_group commands not 
                               working.
                             - Fixed up some readme.txt problems and change around the admin command 
                               reference.
                             - Added mute command with one caveat, if the underlying mod has flood 
                               protection, it works allot better if it is turned off and the q2admin 
                               flood protection is used (in most cases with mod code based on v3.20, 
                               flood_msg 0 as the server console will turn off flood protection)
                             - Added q2adminpassword client cvar which is checked at client connect to 
                               see if the client should be set to Admin Mode.
                             - Converted the memory routines in the GNU RE lib to the Q2 memory routines. 
                             - Change memory routines to use TAG_LEVEL to make sure a level change frees 
                               all memory. There is possible memory leaks in the GNU RE code that may have 
                               caused problems with a server running for a long time. 
v1.1 Beta 4    - 4 March     - Allow mute for 0 seconds to unmute someone.
                             - Fixed a logging problem with one of the zbot detect routines.
                             - Added permanent mute option to the mute command and the chat flood 
                               protection.
                             - Fixed bugs where the mute / flood protection didn't mute/flood protect 
                               the say_person / say_group commands.
                             - Fixed bugs where the say_person / say_group commands wheren't being logged 
                               in to the chat log.
                             - Changed chat flood protection so that there is a global chat flood protection 
                               and a client chat flood protection.  The client overrides the global if it is 
                               set.  If it isn't then the global settings are used.
                             - Added chat flood protection settings to include ban. This sets the clients 
                               chat flood protection.
                             - Added clientchatfloodprotect command to the server console / client admin 
                               console commands to set a clients chat flood protection.
                             - Spanky did some work on the readme.txt file.
                             - Changed the format of the stuff command to use the "[LIKE/RE/CL] name" 
                               format to it. 
                             - Added q2adminflood.txt file to flooding protection. 
                             - Added reloadfloodfile, listfloods, floodcmd and flooddel commands to manage 
                               the new flood command list. 
                             - Convert the old list display commands over to the new coding method for 
                               displaying lists.
                             - Extended say now works in both messagemode and messagemode2 prompts.
                             - play_all (!a), play_team (!t) and play_person (!e) commands added to extended 
                               say.
v1.1 Beta 5    - 11 March    - Fixed bug where the admin client commands that used the [LIKE/RE/CL] syntax 
                               didn't work correctly.   This included stuff, say_person and say_group server 
                               admin commands.
                             - Improved support for non-standard console chat in the chat flood protection / 
                               muting commands.   The only mod so far that I know does this is LMCTF.
v1.1 Beta 6    - 28 March    - Fixed major problems with the zbot detection feature turned off.
                             - Minor changes to internal features.
v1.1           - 12 April    - Fixed some internal house-keeping bugs.
                             - Added support for a different method for generating chat messages.  Adds 
                               support for OSP and any other mod that uses this method for generating 
                               chat messages.
                             - Renamed q2admin.cfg to q2admin.txt.  This is the make sure that 
                               q2admin.txt will not be exec'd.
                             - Fixed bug with 'chatfloodprotect' causing a 'SZ_GetSpace: overflow without 
                               allowoverflow set' error.
v1.2 Beta 1    -  8 May      - Added 'ip' commnd for displaying a clients IP.
                             - Added 'kick' commnd for kicking one or more clients.
                             - Added 'lock' commnd for locking / unlocking a server.
                             - Added disable entity spawning feature. (q2adminspawn.txt, spawnentities_enable, 
                               spawncmd, spawndel, listspawnsRun)
                             - Fixed a loophole which allowed clients to run client commands on other client 
                               machines.
                             - Command voting.
                             - Added option to load unsafe so files in linux.
v1.2 Beta 2    - 11 May      - Wrote my own standard clib functions to try and make it linux clib/glib 
                               version independent.
                             - Added WhiteFang's zbotcheck code (zbot type movement detection).
v1.2 Beta 3    - 26 May      - Changed ip address printout from chat to normal.
                             - Detect and ignore Nitro2 and BW-Proxy clients.
                             - Change the way I compile the glib support.
v1.2 Beta 4    - 24 July     - Fixed bug that would cause a crash with some mods and genreal crashs sometimes.
v1.2 Beta 5    -  7 August   - Added 2 methods of detecting ratbots (1 quick and 1 slow).
                             - Added proxy_bwproxy and proxy_nitro2 for controlling how BW-Proxy and Nitro2 Proxy
                               are detected.
                             - Added 'consolechat_disable' to disable client console chat.
v1.2 Beta 6    - 10 August   - Added 'cvarset' command for emonk.
                             - Changed ratbot detect routine to just print the message to the client only.
                               It now can't be turned off.
                             - Fixed up the way the second ratbot method works to be more internet safe and 
                               to fix a bug that caused false detects.
                             - Added 'q2adminrunmode' for internal reasons.
                             - Added 'maxclientsperframe' for internal testing.
                             - Updated readme.txt to include ratbot documentation updates.
                             - Added 'framesperprocess' for internal testing.
v1.2 Beta 7    - 12 August   - Fixed bug there name changes where NOT meant to be passed onto mod code.
                             - Added spawn files per mod map.
v1.2 Beta 8    - 28 August   - Added include file support for in banfiles.
                             - Added '\m' in the string processing is replaced by the current mod directory.
                               e.g. in the q2adminban.txt file: INCLUDE: "\m\\includebanlist.txt"
                             - Updated readme.txt to include a string preprocessing section.
v1.2 Beta 9    - 29 August   - Updated the 'ratbot detect test' to support v2.25 of the ratbot.
v1.2 Beta 10   - 25 September- Fixed linux clib problem (this fixes the chasm cam problem with the 
                               ServerConfigMod and the Includes in ban files problem).
                             - Fixed chatflood problem.  This fixes the crash with Superheros2.
                             - Added more warning printouts when bans / chatbans / flood protection / disabled 
                               spawns / voting lines fail.
                             - Fixed change level problems with nitro2 and bw-proxy and reduced the change
                               level overhead for reconnecting players when using the gamemap change level
                               method.
                             - Changed the second ratbot method to be more internet friendly to lagged players.
                             - Increased the RE expression size to 80 characters from 20 characters.
v1.2 beta 11   - 30 October  - Added timescale hack detect.
                             - Added minfps.
                             - Removed clib fix, seamed to cause more problems than it solved.
                             - Added skin flood protection.
                             - Added disable client commands.
v1.2 beta 12   - 3  November - Fixed name change problem.
                             - Changed Nitro2 detection.
                             - Added Xania proxy detection (since it's basically the same as Nitro2 it works 
                               off the proxy_nitro2 variable)
v1.2 beta 13   - 11 November - Fixed problem with the speed cheat detection.
                             - Fixed Nirto2 detection.
                             - Added -5 bot log for a Nitro2 / BW-Proxy / Xania proxy detection.
                             - Changed some of the default proxy detect messages to be more proxy generic than
                               "zbot" only messages.
v1.2 beta 14   -  2 January  - Added cl_pitchspeed change detect.
                             - Changed around the error codes:  -5 is a timescale detect, -6 is a proxy detect
                             - Added -7 log which is a cl_pitchspeed change detect.
v1.3           - 25 January  - Added minrate command.
                             - Added forced reconnect option. 
v1.4           - 28 January  - Fixed bugs in reconnect.
                             - Added voteclientmaxvotes and voteclientmaxvotetimeout variables.
v1.5           - 31 January  - Added spawnentities_internal feature.
                             - Added entity logging.
                             - Commented out the skin change logging in the default log file.
               - 12 February - Added filternonprintabletext command.
v1.6           - 19 February - Added quick detection for the new version of ratbot.
v1.7           - 26 February - Removed -8 ratbot logging because of false logging.
                             - Removed the ratbot and p_version messages from displaying.  Replaced with
                               non display mesages.
                             - Added new ratbot detect of the latest ratbot.
                             - Added swap_attack_use.  Another method for getting around the auto-aim proxy.
                             - Added a 5 retry limit to the reconnect feature so that people can't get into
                               a infinite retry loop.
v1.8          -   9 June     - Added reconnect_checklevel to allow a less strict setup of the reconnect feature.
                             - Added lockoutmsg command for the lock command.
                             - Changed the lock command to allow access to disconnected / disconnecting
                               users.
                             - On map changes, run a <mapname>-pre.cfg script before the map is changed and run
                               <mapname>-post.cfg script after the map is loaded.
                             - Change the timescale checking routines just in case.
                             - Kick clients that change the 'msg' to other than '0'.
                             - Fixed a bug that stoppd Q2 Variant (http://www.ionwerks.net/defunkt/action/q2v.htm) 
                               from working.
v1.9          -  11 June     - Fixed with v1.8 crashing linux servers (Seams there is a difference to the way 
															 that linux quake2 handles events to the win32 quake2 server).
v1.10         -  12 June     - Changed the msg kick to only effect msg mode less than 4. 
v1.11         -   2 May      - Added four new options that can be used in q2admin.txt: dopversion,
                               hackuserdisplay, defaultreconnectmessage & skincrashmsg. An explanation on
                               how to use these new options can be found inside this readme.
                             - Replaced the cvar "Bot No Bots" by "Q2Admin 1.11". This way programs like
                               Gamespy will show Q2Admin's version number as part of the server info.
                             - Made it possible to give a message to reconnecting players.
                             - Made the p_version on level-join serveradmin-configurable.
                             - Added protection for speedbots using a timescale value between 1 and 1.999.
                             - Made the checksum for speedbots random so people cannot get around it anymore.
                             - Added protection for hacked quake2.exe, so people cannot get around the
                               reconnect-function or rate-cap anymore.
                             - Added protection for known proxies that want to bypass the reconnect-function.
                             - Added protection for wrong userinfo so clients can't crash the server
                               anymore or other quake2 players on the server.
                             - Fixed a bug that made the server crash when someone changed their userinfo
                               (i.e. name or skin) while reconnecting.
                             - Added "[Q2Admin]" at the beginning of every default message that is given
                               by Q2Admin (for example: "[Q2Admin] %s is using a zbot!!!").
                             - Modified the readme.txt to include directions on how to use different
                               q2admin.txt files when you run multiple servers from one mod directory
                               (this functionality was already in Q2Admin, but was never documented).
v1.12         -  24 June     - Fixed bug in disable spawn entity bug where it would only disable the 
                               entity for one level when gamemap was being used to change levels.
                             - Added ".disconnect" message to one of the display messages.
                             - Updated readme with information about Macintosh users and the "use" feature.
                             - Corrected the userinfo fix with a better fix that didn't stop other features
                               from working correctly.
                             - Fixed bug in serious bug lrcon that allowed only "part" of a password would be 
                               allowed.
                             - Added in the checking client side variables feature. See section 2.14.
                             - Forced an IP address to be in the userinfo when connecting (I think this will
                               get around people trying to ignore banning).
                             - Added rcon_random_password and lrcon_timeout to get around people dumping
                               lrcon packets packets and finding out the rcon password.
v1.13         -  28 June     - Fixed the userinfo bug at it's source.
v1.14         -   7 July     - Added support for the Win32 loopback startup.
                             - Changed lrcon "rcon_random_password" feature to work for win32 clients.
v1.15         -  12 August   - Fixed bug with "rcon_random_password" feature.
                             - All the map change exec cfg files are now run from the mapcfg directory. (Mustang)
                             - All the map change exec cfg files are controled by the mapcfgexec variable in
                               q2admin.txt.
                             - Add printmessageonplaycmds command to display messages when someone uses a 
                               play_ command. (Mustang)
                             - Added another bot protection supplied to me by Vithar.
                             - Print the IP address to the console window in the banned user message.
                             - Added cl_anglespeedkey detect.
                             - Added -9 log which is a cl_anglespeedkey change detect.
                             - Added q2adminbantxt variable for the same use as the q2admintxt variable.
                             - Added maxmsglevel variable to control the msg level that the maximum client level
                               clients are allowed to have.
                             - Added checkclientipaddress to control if the users ip address is checked on connect.
                               (Note: users that don't have a valid ip address can't be ip banned, also the reason
                               a user may have a invalid ip address is because there userinfo space is overflowing
                               and this can cause other problems by itself)
v1.16         -  15 August   - Fixed spamming problem with ".please.disconnect.all.bots" messages.  Opps my problem.
                             - Added another reconnect protection again the newest bots that get around the 
                               reconnect feature. (manic miner)
v1.17         -  18 August   - Fixed the rest of the spamming problem with the ".please.disconnect.all.bots" messages.
                             - Added in better check for ip addresses.  It now works with userinfo strings that have
                               overflowed.
                             - Added in a check for client userinfo buffers that have overflowed.  It displays a 
                               message to the client and to the server console window.
