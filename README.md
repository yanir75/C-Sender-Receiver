# C-Sender receiver

This project has come to show the diffrences between TCP reno and cubic.
I wrote a script that sends a file through TCP socket (cubic) 5 times then changes the implementation socket to reno and sends the file 5 times again.

This will be devided in to two parts, measure and sender.
Measure will create a socket listening to a decided port and will wait for the connection.
Moreover it will receive the files upon connection and will print the time it took for the file to transfer.
In addition it will print the avg time it took for cubic and reno.

Sender will connect to the socket and send the file 5 times then it will change the tcp implementation to reno and send it 5 times again.

---------
# How to use

First lets clone the repository.
```
git clone
```
compile the files
```
gcc -o measure measure.c
```
```
gcc -o sender sender.c
```
Activate the measure
```
./measure
```
Activate the sender
```
./sender
```
