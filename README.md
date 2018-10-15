# TCP Client Server Test in C
Adapted from: Suyash [https://gist.github.com/suyash/2488ff6996c98a8ee3a84fe3198a6f85](https://gist.github.com/suyash/2488ff6996c98a8ee3a84fe3198a6f85)
Multiple client extension adapted from: GeeksforGeeks [https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)

## Build
```bash
cd build
cmake ..
make client
make server
```

## Run
1st Terminal:
```bash
cd build
./server
```

2nd Terminal:
```bash
cd build
./client
```
