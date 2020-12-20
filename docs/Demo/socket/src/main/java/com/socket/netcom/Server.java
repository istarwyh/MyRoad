package com.socket.netcom;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.*;

/**
 * 要求:
 * 1. 如果请求文件有则返回给客户端,否则返回错误消息
 * 2. 超时3s或错误返回则重传
 * 3. 多线程允许同时为多个客户端服务
 * 实现步骤:
 * 1. 创建一个服务器ServerSocket对象--ss 绑定客户端指定的端口号
 * 2. 使用ss的方法accept()获取到请求的客户端Socket对象,可以循环执行accept()
 * 3. 使用Socket对象中的方法getInputStream(),获取到网络字节输入流InputStream对象
 * (对每一个socket新开一个线程,并在执行所有操作后释放资源)
 * 4. 把ips网络字节输入流对象转换为字符缓冲输入流来读取网络字节输入流的数据(这里即文件名,不需要做多余字符串操作)
 * 5. 判断有无文件,对应执行7
 * 6. 调用客户端Socket的socket对象的getOutputStream(),获取ops对象
 * 7. 使用ops中的write方法给客户端回写相关信息
 * 7.1 如果是本地文件,需要执行本地IO流API
 * 8. 等待3秒,确认客户端返回的确认消息;如果不符合,重发文件
 * 9. 释放资源(FileInputStream,Socket,ServerSocket)
 *
 * @author MBin_王艺辉istarwyh
 */
public class Server {
    /**
     * 使用线程池提高服务器多线程性能,超出1000线程并发则拒绝服务
     */
    private static final ExecutorService POOL;

    static {

        POOL = new ThreadPoolExecutor(0, 1000, 60L, TimeUnit.SECONDS, new SynchronousQueue<Runnable>());
    }

    /**
     * 双方协商好结束符号"EOF"
     */
    public static String endMessage = "EOF";
    public static String goodMessage = "文件接收成功";

    public static void main(String[] args) throws IOException {
//         *      1. 创建一个服务器ServerSocket对象--ss 绑定客户端指定的端口号
        ServerSocket ss = new ServerSocket(8081);

// *      2. 使用ss的方法accept()获取到请求的客户端Socket对象,可以循环执行accept()
        while (true) {
            Socket socket = ss.accept();
            POOL.execute(
                    () -> new Thread(() -> new Server().acceptFileServer(socket)).start()
            );

        }
//      9. 释放资源(FileInputStream,Socket,ServerSocket) ServerSocket一直执行,就不用关闭了
//        ss.close();
    }

    public void acceptFileServer(Socket socket) {
        try {
            String line = this.readLine(socket);
            File targetFile = new File("H:\\桌面\\upload\\" + line);
//                *      5. 判断有无文件,对应执行7
//                *      6. 调用客户端Socket的socket对象的getOutputStream(),获取ops对象
            if (targetFile.exists()) {
                this.sentFile(socket, targetFile);
//                *      8. 等待3秒再确认客户端返回的确认消息;
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

//               8. 如果不符合,重发文件一次
                if (!goodMessage.equals(this.readLine(socket))) {
                    this.sentFile(socket, targetFile);
                    System.out.println("重发了吗");

                }
            } else {
                OutputStream ops = socket.getOutputStream();
                ops.write("文件不存在".getBytes());
            }

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
//                *      9. 释放资源(FileInputStream,Socket,ServerSocket)
                e.printStackTrace();
            }
        }

    }

    public String readLine(Socket socket) throws IOException {
//           3. 使用Socket对象中的方法getInputStream(),获取到网络字节输入流InputStream对象
        InputStream ips = socket.getInputStream();
//           4. 把ips网络字节输入流对象转换为字符缓冲输入流
        BufferedReader br = new BufferedReader(new InputStreamReader(ips));
//           4. 读取客户端的请求,这里即文件名不需要做多余字符串操作
        String line = br.readLine();
        return line;
    }

    public void sentFile(Socket socket, File targetFile) throws IOException {
        OutputStream ops = socket.getOutputStream();
//                *      7.1 如果是本地文件,需要执行本地IO流API
        FileInputStream fis = new FileInputStream(targetFile);
        byte[] bytes = new byte[1024];
        int len = 0;
        while ((len = fis.read(bytes)) != -1) {
            byte[] endMessageBytes = endMessage.getBytes();
            int endMessageLen = endMessageBytes.length;
//            还有足够的空间容纳endMessage时
            if (len < 1024 - endMessageLen) {
                for (byte b : endMessageBytes) {
                    bytes[len++] = b;
                }
                ops.write(bytes, 0, len + endMessageLen);
                break;
            } else if (len < 1024) {
//                防止数组越界,必须进行扩容
                byte[] bytesPlus = new byte[1027];
                int index = 0;
                for (byte b : bytes) {
                    bytesPlus[index++] = b;
                }
                for (byte b : endMessageBytes) {
                    bytesPlus[index++] = b;
                }
                ops.write(bytes, 0, len + endMessageLen);
            } else {
                ops.write(bytes, 0, len);
            }
        }
//      9. 释放资源(FileInputStream,Socket,ServerSocket)
//         但是本地流不能在这里关,后面还会再发送一次
//        fis.close();
    }
}
