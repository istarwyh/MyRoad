package com.example.hello.controller;

import com.example.hello.entity.User;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import java.util.ArrayList;
import java.util.List;

@Controller
@RequestMapping("/thymeleaf")
public class ThymeleafController {
    /**
     * 如果找不到方法就会自动找/templates/下的404页面,如果没有就使用默认的404页面(包含错误信息)覆盖
     *
     * @return
     */
    @GetMapping("/test404")
    public String test404() {
        return "index";
    }

    @GetMapping("/test500")
    public String test500() {
//        模拟服务器运行错误
        int i = 1 / 0;
        return "index";
    }

    @GetMapping("/getUserList")
    public String testUserList(Model model) {
        User user1 = new User(1, "王艺辉", "wyh", "15955311246");
        User user2 = new User(2, "陈黎君", "lijun", "18810255972");
        List<User> userList = new ArrayList<>();
        userList.add(user1);
        userList.add(user2);
//        TODO:对象放到model中,但是model谁给的?model的参数又怎么传到thymeleaf页面的?Model支持的多种构造方式默认的应该怎么用?
        model.addAttribute(userList);
        //      指按MVC view的方式解析list,即找templates底下的list.html
        return "userList";
    }
}
