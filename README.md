# filetcpclient + compile and execute
bare bones file tcp client using packed struct encoding and custom packets

to run

client side :
```
gcc fileclient.c -Wall -o b
./b localhost 8893
```
server side:
```
gcc fileserver.c -Wall -o a
./a 8893
```
