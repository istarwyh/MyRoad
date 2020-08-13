package com.itheima.controller;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

//@Controller
//public class QuickGetConfigurationInfoController {
//    @Value("${name}")//获取配置文件信息
//    private String name;
//    // 这样很有针对性,但是要批量的得到信息应该怎么办呢?-->定义配置类并注入IOC容器,用时再导入
//    @Value("${person.addr}")
//    private String addr;
//
//    @RequestMapping("/quick2")
//    @ResponseBody
//    public String quick2(){
//
//        return "name:"+name+",addr:"+addr;
//    }
//}
@Controller
@ConfigurationProperties(prefix = "person")// 找到配置文件中属性的前缀,之后返回对应属性
public class QuickGetConfigurationInfoController {
    private String name;
    private String addr;
    private Integer age;

    @RequestMapping("/quick3")
    @ResponseBody
    public String quick2() {

        return "name:" + name + ",addr:" + addr + ",age:" + age;
    }
    // 必须要有get和set方法,供修改和访问

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAddr() {
        return addr;
    }

    public void setAddr(String addr) {
        this.addr = addr;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }
}
