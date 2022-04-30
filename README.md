# Multi-Threaded Web Server in C

## 簡介
這是一個用c實現的multi_threaded web server
使用socket建立連線，用pthread建立新的thread處理每個接收到的request，並且設定thead limitation來限制threads的數量以確保server不會炸掉，並可以利用多線程的方式來improve throughput

#### git clone
```
$ git clone git@github.com:yi-shin-cheng/Multi_Thread-http-Server.git
$ cd Socket
```

#### 啟動

啟動server需要先編譯Server.c，然後再啟動
```
$ gcc -pthread Server.c -o Server.out
$ ./Server.out
```

到這裡server就算是成功啟動的，也可以接收request
可以使用各種方式對它發送request，包括postman、apache benchmark等等
或者直接使用Client.c和它進行溝通的測試
```
$ gcc Client.c -o Client.out
$ ./Client.out
```


