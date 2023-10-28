package com.domain.controller;

import org.springframework.web.bind.annotation.RequestMapping;


public class indexController {
    @RequestMapping("/")
    public void index(){
        System.out.println("index");
    }
}
