package com.itheima;

import org.springframework.boot.SpringApplication;
// import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;
// import org.springframework.context.annotation.Bean;
// import org.springframework.context.annotation.ComponentScan;

/**
 * 基于约定或扫描这个注解所在对的包以及子包的所有注解
 */
@SpringBootApplication//引导类,@SpringBootConfiguration,@EnableAutoConfiguration,@ComponentScan
public class SpringbootQuick2Application {

    public static void main(String[] args) {
        // 引导类的字节码作为对象传入
        SpringApplication.run(SpringbootQuick2Application.class, args);
    }
}
