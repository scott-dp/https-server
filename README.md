![image](https://github.com/user-attachments/assets/6a9b8f79-4309-419b-97e0-33258623fce8)

<h2>Compile on windows</h2>
 g++ -std=c++20 -D_WIN32_WINNT=0x0A00 main.cpp src/TLSServer.cpp -o httpsServer -lws2_32 -lmswsock -lssl -lcrypto
