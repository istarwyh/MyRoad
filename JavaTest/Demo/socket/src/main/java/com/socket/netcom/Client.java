package com.socket.netcom;

import java.io.*;
import java.net.Socket;
import java.util.Random;

/**
 * 要求:
 * 向服务器发送请求文件名,接收服务器端返回的文件或消息
 * 成功接收到文件后自动向Server发送一个确认（自行定义）
 * 实现步骤:
 * 1.创建一个客户端Socket对象,绑定服务器IP地址和端口号
 * 2.使用Socket中的方法getOutputStream(),获取网络输出流OutputStream对象
 * 3.使用OutputStream的write(),发送文件名到服务器
 * 4.使用Socket中的方法getInputStream(),获取网络字节输入流对象InputStream
 * 5.使用InputStream中的read()读取服务器回写的数据
 * 6.释放资源(Socket,FileOutputStream)
 *
 * @author MBin_王艺辉istarwyh
 */
public class Client {
    private volatile static Client client;
    /**
     * 双方协商好结束符号"EOF"
     */
    public static String endMessage = "EOF";
    public static String errorMessage = "文件不存在";

    public static void main(String[] args) throws IOException {
        client = new Client();
        client.clientModel();
    }

    public void clientModel() throws IOException {
// *      1.创建一个客户端Socket对象,绑定服务器IP地址和端口号
        Socket socket = new Socket("127.0.0.1", 8081);
// *      2.使用Socket中的方法getOutputStream(),获取网络输出流OutputStream对象
        OutputStream ops = socket.getOutputStream();
// *      3.使用OutputStream的write(),发送文件名到服务器
        ops.write("test.txt".getBytes());
//        类比http换行,服务器端readLine()读取\n为换行符
        ops.write("\n".getBytes());

        int fileState = this.acceptFileClient(socket);
//        7.文件接收成功,使用ops中的write方法给客户端回写相关信息
        switch (fileState) {
            case 1:
                ops.write("文件接收成功\n".getBytes());
                break;
            case 2: {
                ops.write("文件接收失败\n".getBytes());
                this.acceptFileClient(socket);
                break;
            }
            case 3: {
                System.out.println(errorMessage);
                socket.close();
                return;
            }
            default:
                System.out.println("未知情况");
        }

// *      8.释放资源(Socket,FileOutputStream(try()已释放)) 不能提前关闭掉socket
        socket.close();
    }

    public int acceptFileClient(Socket socket) throws IOException {
        // *      4.使用Socket中的方法getInputStream(),获取网络字节输入流对象InputStream
        InputStream ips = socket.getInputStream();
// *      5.使用InputStream中的read()读取服务器回写的数据
        //        提高读取效率,使用数组固定每次从输入流拉取的字节大小
        int messageLen = 2 * endMessage.getBytes().length;
        byte[] bytes = new byte[1024 + messageLen];
        int len = 0;
        //        read()默认返回读到的byte数量,当读不到更多的数据时会返回-1
        if ((len = ips.read(bytes)) != -1) {
//            取出bytes中的数据判断
            if (errorMessage.equals(new String(bytes, 0, len))) {
                return 3;
            } else {
                File filePath = new File("H:\\桌面\\download");
                if (!filePath.exists()) {
                    filePath.mkdirs();
                }
                /**
                 * 自定义一个文件的命名规则:防止同名文件被覆盖
                 *      规则:域名+毫秒值+随机数
                 */
                String fileName = "127.0.0.1." + System.currentTimeMillis() + new Random().nextInt(999999) + ".txt";
                try (FileOutputStream fos = new FileOutputStream(filePath + "\\" + fileName)) {
                    do {
//                      6.将服务器端传来的文件写到本地磁盘上
//                      上一步已经读入bytes,故这里采用do-while
//                      去掉最后的"EOF"结束标志
                        fos.write(bytes, 0, len - messageLen);
                        String m = new String(bytes, len - messageLen, messageLen);
                        if (myEquals(m, endMessage)) {
                            return 1;
                        }
                    } while ((len = ips.read(bytes)) != -1);
                }
            }
        }
        return 2;
    }

    /**
     * 因为默认utf8编码,从字节流中拉取的"EOF"长度为6字节,必须重写String的默认equals()
     * System.out.println(s1.length()+"+"+c1.length);
     *
     * @param s1
     * @param s2
     * @return
     */
    public boolean myEquals(String s1, String s2) {
        char[] c1 = s1.toCharArray();
        char[] c2 = s2.toCharArray();
        for (int i = 0; i < c2.length; i++) {
            if (c1[i] != c2[i]) {
                return false;
            }
        }
        return true;
    }

}
