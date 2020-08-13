package com.example.mybatistest.controller;

import com.example.mybatistest.entity.User;
import com.example.mybatistest.service.UserService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;

@Controller
@ResponseBody
@RequestMapping("/user")
public class UserController {

    private final UserService userService;

    private UserController(UserService userService) {
        this.userService = userService;
    }

    @RequestMapping("/insert")
    public List<User> testInsert() {
        userService.insertService();
        return userService.selectAllUser();
    }

    @RequestMapping("delete")
    public String testDelete() {
        userService.deleteService(3);
        return "OK";
    }

    @RequestMapping("update")
    public String testUpdate() {
        userService.updateService();
        return "OK";
    }

    @RequestMapping("/query")
    public User testQuery() {
        return userService.selectUserByName("Daisy");
    }

    @RequestMapping("/changeMoney")
    public List<User> testChangeMoney() {
        userService.changeMoney();
        return userService.selectAllUser();
    }


}
