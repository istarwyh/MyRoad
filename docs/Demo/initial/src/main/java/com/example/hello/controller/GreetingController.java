package com.example.hello.controller;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

import com.example.hello.entity.Greeting;
import org.slf4j.Logger;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.slf4j.LoggerFactory;

/**
 * 如果@Controller换为@RestController,就可以不用在底下每个方法上加@ReseponseBody了
 * 不过有单独需要返回view的呢?那可能会单独再开一个Controller类吧
 */
@Controller
/**
 * 告诉Spring MVC不需要使用服务器端视图层(view)渲染对象,而应该直接返回一个ResponseBody
 * Spring Boot 中默认使用的 Json 解析技术框架是 jackson,来自于spring-boot-starter-json 依赖
 */
@ResponseBody
@RequestMapping("/test")
public class GreetingController {
    /**
     * %s给另一个字符串留下位置
     */
    private static final String TEMPLATE = "Hello,%s!";

    private final AtomicLong counter = new AtomicLong();
    /**
     * 使用日志框架slf4j
     */
    private static final Logger logger = LoggerFactory.getLogger(GreetingController.class);

    /**
     * 确保对/greeting的HTTP请求映射到greeting()方法。
     * 默认情况下映射所有的HTTP操作。使用@RequestMapping(method=GET)指定只映射GET请求。
     */
    @RequestMapping("/greeting")

    /**
     把查询字符串中name参数的值绑定到greeting()方法的name参数上;
     该查询参数不是必须的（required=false）；
     如果请求时没有指定该参数，则使用其默认值“World”（defaultValue）
     */
    public Greeting greeting(@RequestParam(value = "name", required = false, defaultValue = "World") String name) {
//        该对象的id属性每次自增1
        return new Greeting(counter.incrementAndGet(), String.format(TEMPLATE, name));
    }

    @RequestMapping("/map")
    public Map<String, Object> getMap() {
        HashMap<String, Object> map = new HashMap<>(16);
        Greeting greeting = new Greeting(counter.incrementAndGet(), "王艺辉");
        map.put("打招呼:", greeting);
        map.put("几点了?", "3点了");
//        对于空值需要单独进行配置
        map.put("你吃了吗?", null);
        return map;
    }

    /**
     * 使用实体类来接收前端传过来的数据???
     *
     * @param greeting
     * @return
     */
    @PostMapping("/form")
    public String testRequestBody(Greeting greeting) {
        System.out.println(greeting.getId());
        System.out.println(greeting.getContent());
        return "success";
    }

}