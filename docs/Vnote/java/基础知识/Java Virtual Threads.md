[JEP](https://openjdk.org/jeps/444)
In early versions of Java, Java's green threads all shared one OS thread (M:1 scheduling) and were eventually outperformed by platform threads, implemented as wrappers for OS threads (1:1 scheduling). Virtual threads employ M:N scheduling, where a large number (M) of virtual threads is scheduled to run on a smaller number (N) of OS threads. Since **virtual threads are implemented in the JDK** and are not tied to any particular OS thread, **they are invisible to the OS**, which is unaware of their existence. Virtual threads are always daemon threads.

⚠️ A thread pool, like any resource pool, is intended to share expensive resources, but virtual threads are not expensive, so **there is never a need to pool them**.

⚠️ In the asynchronous style, **each stage of a request might execute on a different thread**, and e**very thread runs stages belonging to different requests in an interleaved fashion.** This has deep implications for understanding program behavior: 
- Stack traces provide no usable context, 
- debuggers cannot step through request-handling logic, and 
- profilers cannot associate an operation's cost with its caller.
Composing lambda expressions is manageable when using Java's [stream API](https://download.java.net/java/early_access/jdk21/docs/api/java.base/java/util/stream/package-summary.html) to process data in a short pipeline, but problematic when all the request-handling code in an application must be written in this way. This programming style is at odds with the Java Platform because the application's unit of concurrency — the asynchronous pipeline — is no longer the platform's unit of concurrency.