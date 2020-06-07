# CLI-Browser
Simulates the behaviour of a Web Browser regarding the management of  Web Pages and tabs using a Stack and a Queue Data Structures.

The tabs are stored using linked lists, so that a new tab is always the last element of the list and the first deleted tab will always be
the last element of the list.

The Global History is stored using a Generic Queue data structure, and the Downloads queue is implemented using a Priority Queue, the
priority of an element being decided by the remaining bytes to be downloaded (generated randomly).

The Back and Forward actions relative to a Tab are stored using a Stack data structure.

**Available Commands:**<br />
 - set_band <bandwidth> - sets the download speed (default: 1 KBps)
 - newtab - adds a new empty tab to the tabs list
 - deltab - deletes the last added tab
 - change_tab <tab_index> - navigates to the specified indexed tab
 - print_open_tabs
 - goto <URL> - accesses the web page given by <URL>
 - back/forward - navigating through the Back/Forward Stacks
 - history - prints the current global history
 - del_history <nr_entries> - deletes the specified number of entries from the global history (del_history 0 to delete it all)
 - downloads - prints the current state of the Downloads Queue
 - list_dl - prints the downloadable resources that the current web page offers
 - download <index_resource> - adds to the Priority Queue the resource with the specified index, from the current Web Page
 - wait <seconds> - advances the downloads' current stage with specified time

**Example Usage:**<br />
./CLIB in_file out_file
*in_file:*<br />
set_band 102400<br />
goto URL1<br />
goto URL2<br />
list_dl<br />
download 1<br />
downloads<br />
newtab<br />
goto URL3<br />
change_tab 0<br />
goto URL4<br />
newtab<br />
goto URL5<br />
print_open_tabs<br />
list_dl<br />
download 2<br />
deltab<br />
downloads<br />
print_open_tabs<br />
wait 25<br />
downloads<br />

*out_file:*<br />
[0 - "URL2-(0)" : 86570845]<br />
[1 - "URL2-(1)" : 7927001]<br />
[2 - "URL2-(2)" : 62482]<br />
[3 - "URL2-(3)" : 76084628]<br />
[4 - "URL2-(4)" : 5305591]<br />
[5 - "URL2-(5)" : 21035621]<br />
[6 - "URL2-(6)" : 13171437]<br />
["URL2-(1)" : 7927001/7927001]<br />
(0: URL4)<br />
(1: URL3)<br />
(2: URL5)<br />
[0 - "URL5-(0)" : 76084628]<br />
[1 - "URL5-(1)" : 5305591]<br />
[2 - "URL5-(2)" : 21035621]<br />
[3 - "URL5-(3)" : 13171437]<br />
...<br />
["URL2-(1)" : 7619801/7927001]<br />
["URL5-(2)" : 21035621/21035621]<br />
(0: URL4)<br />
(1: URL3)<br />
["URL2-(1)" : 5059801/7927001]<br />
["URL5-(2)" : 21035621/21035621]<br />
