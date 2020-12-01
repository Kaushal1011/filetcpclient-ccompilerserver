# filetcpclient + compile and execute
bare bones file tcp client using packed struct encoding and custom packets. works as a c compiler can't return errors for now

to run

client side :
```
gcc fileclient.c -Wall -o b
./b localhost 8893
enter filename to send :
program1.c
...
...
Hello
```
server side:
```
gcc fileserver.c -Wall -o a
./a 8893
```
