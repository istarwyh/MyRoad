package com.itheima;

// 导入了junit4

import com.itheima.domain.User;
import com.itheima.mapper.UserMapper;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.List;

// RunWith代表用谁执行
@RunWith(SpringRunner.class)
// 声明是测试类,传入引导类的字节码
@SpringBootTest(classes = SpringbootQuick2Application.class)
public class MybatisTest {
    // 测试谁就注入谁
    @Autowired
    private UserMapper userMapper;

    @Test
    public void test() {
        List<User> users = userMapper.queryUserList();
        System.out.println(users);
    }
}
