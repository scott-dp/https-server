![image](https://github.com/user-attachments/assets/9efdd530-c33e-4772-995a-870208920814)
<h2>Compile on windows</h2>
 g++ -std=c++20 -D_WIN32_WINNT=0x0A00 main.cpp src/TLSServer.cpp src/TLSClient.cpp -o httpsServer -lws2_32 -lmswsock -lssl -lcrypto
