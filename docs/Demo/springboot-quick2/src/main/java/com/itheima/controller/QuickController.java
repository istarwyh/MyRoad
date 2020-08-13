package com.itheima.controller;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * 默认json或字符串
 */
@RestController
public class QuickController {
    @RequestMapping("/quick")
    public String quick() {
        return "hello SpringBoot";
    }
}
