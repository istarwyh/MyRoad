package com.socket.netcom;

import java.io.IOException;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 多线程来并发启动100个Client进行测试，
 */
public class ParallelTest {
    private static ExecutorService pool;

    public static void main(String[] args) {
        ClientThreadFactory clientThread = new ClientThreadFactory("Client");
//        Runnable一个没有参数和返回值的异步方法
        Runnable r = () -> {
            Client client = new Client();
            try {
                client.clientModel();
            } catch (IOException e) {
                e.printStackTrace();
            }
        };
        pool = new ThreadPoolExecutor(
                0,
                1000,
                60L,
                TimeUnit.SECONDS,
                new SynchronousQueue<Runnable>(),
                new ClientThreadFactory("test"));
        for (int i = 0; i < 1000; i++) {
            pool.execute(() -> {
                Thread t = clientThread.newThread(r);
                t.start();
            });
        }
//        使用指定1000大小的60s存活的线程池,1000个线程并发,最后只需要476个即可
        System.out.println(pool);

        pool.shutdown();

    }


    public static class ClientThreadFactory implements ThreadFactory {
        private final String namePrefix;
        private final AtomicInteger nextId = new AtomicInteger(1);

        ClientThreadFactory(String whatFeature) {
            namePrefix = whatFeature;
        }

        @Override
        public Thread newThread(Runnable task) {
            String name = namePrefix + nextId.getAndIncrement();
            Thread thread = new Thread(null, task, name);
            System.out.println(thread.getName());
            return thread;
        }
    }

}
