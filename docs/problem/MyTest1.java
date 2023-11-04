import java.util.HashMap;

public class MyTest1 {

    private static Boolean flagA = true;
    private static Boolean flagB = false;
    private static Boolean flagC = false;

    /**
     * 基本思路就是设置三个boolean变量和一个锁。flag控制哪个线程可以走，哪个应该停下来。然后在打印后才i++计数。直到i<10的时候，线程停止。
     * 如果wait出现意外并没有释放锁，那就无法通知notify，会不会造成一个死锁?
     *
     * @param args
     */
    public static void main(String[] args) {
        final Object lock = new Object();

        Thread aThread = new Thread(new Runnable() {

            @Override
            public void run() {
                for (int i = 0; i < 10; ) {
                    //  synchronized一种同步锁,保证资源一个时刻只被一个线程占有
                    synchronized (lock) {

                        if (flagA) {
                            //线程A执行
                            System.out.println("A");
                            flagA = false;
                            flagB = true;
                            flagC = false;
                            lock.notifyAll();
                            i++;

                        } else {
                            try {
                                lock.wait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }

                        }
                    }

                }


            }
        });


        Thread bThread = new Thread(new Runnable() {

            @Override
            public void run() {
                for (int i = 0; i < 10; ) {

                    synchronized (lock) {
                        if (flagB) {
                            //线程执行
                            System.out.println("B");
                            flagA = false;
                            flagB = false;
                            flagC = true;
                            lock.notifyAll();
                            i++;

                        } else {

                            try {
                                lock.wait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }

                        }

                    }

                }


            }
        });


        Thread cThread = new Thread(new Runnable() {

            @Override
            public void run() {
                for (int i = 0; i < 10; ) {

                    synchronized (lock) {

                        if (flagC) {
                            //线程执行
                            System.out.println("C");
                            flagA = true;
                            flagB = false;
                            flagC = false;
                            lock.notifyAll();
                            i++;

                        } else {

                            try {
                                lock.wait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }

                        }


                    }

                }

            }
        });


        cThread.start();
        bThread.start();
        aThread.start();
    }
}