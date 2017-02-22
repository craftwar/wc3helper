OpenFoundry Project Page
http://of.openfoundry.org/projects/1274




Requirement

WinXP+

MSXML6

Microsoft Visual C++ 2008 SP1 Redistributable Package
	http://www.microsoft.com/downloads/details.aspx?familyid=A5C84275-3B97-4AB7-A40D-3802B2AF5FC2&displaylang=en
or use files in static folder to replace original files (only recommend in cyber cafe)



all cofings are saved in koala's WC3 helper.xml (don't change name or move/delete)
you can add any xml comment/attribute, change attribute name but not attribute order
"koala's WC3 helper.txt" is an example of how to do this


Run
	execute program
	put shortcut target field value here, double quotes should be reserved to prevent accident
	attribute can be enclosed in single quotes('), and you don't need to escape double quote(")
	read XML reference, if u wana escape single or double quotes


RunWar3
	run War3 if not exist
	0	no
	1	full screen
	2	window mode

	ChangeWindow 		change War3 window existing when program starts
	onTop		ontop display
	x="0" y="0" 	window position (left-top corner)
	size_x="1024" size_y="768" window size¡Aset both to -1 to maximize


Network_latency
	change latency (for Battle.net only, similar function as DR)
	war3 default latency for Battle.net:250ms Lan:100ms
	should run helper before creating game
	0 not change, set 0 when war3 release new version


chat
	input helper
	0 off
	1 on


RestrictMouse
	restrict mouse movement in war3 window region (auto-detection when war3 is in window mode)
	support mutiple war3 instance


FnKey		
	function key, set key="" to disable

Suspend
	enable/disable hotkey switch (auto Chinese IME detection, auto-disable/enable)

Hide_War3
	hide/restore War3 windows, close_helper  close koala's WC3 helper when hide
	program startup auto resotre hidden War3 window

	
FastGame
	fast create/join game, must ctrl+c copy game name before use

FastPublicGame
	O (Observers)
	0 No Observers
	1 Full Observers
	2 Refrees

	create	send alt+c

	vis (Visibility)
	0 Default
	1 Always Visible

FastPrivateGame
	always Refrees

Paste_bot
	file should be utf-8 encoded (Notepad, save as, uft-8 encoding)
	send file content to War3
	Pastebot_load	select file to load	deault F8
	Pastebot_send	      	       send
	Pastebot_delay	send delay between lines (ms)

	application:
	introduce map(how to play)...etc


Mapkey
	<Mapkey_number number="3"/>  number of mapkey (can't be fewer than acutually use)

modifier
	ctrl 1
	alt 2
	shift 4




key	key to trigger mapkey
target	corresponding key or text, set target="" to disable key
	key should be enclosed in {}, text should not, key and text can be mixed (support most language)

key code should be hexadecimal virtual key code
http://msdn.microsoft.com/en-us/library/dd375731(VS.85).aspx


MSDN path (prevent url changes)
MSDN Library -> Win32 and COM Development -> Keyboard Input Reference -> Constants -> Virtual-Key Codes


know issue

if mouse restriction is not working properly, switch to other window and switch back

mouse restiction for mutiple war3 (bug? or war3 design?  dunno)
1)run war3 2)run helper 3)switch to war3
4)run new war3  5)helper->find war3 6)switch to new war3
more instance? repeat step4~6

my blog
http://craftwar.blogspot.com/

my WC3 page
http://koalafile.googlepages.com/koala'sfiles