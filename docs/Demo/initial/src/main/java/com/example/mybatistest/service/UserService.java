package com.example.mybatistest.service;

import com.example.mybatistest.entity.User;
import com.example.mybatistest.dao.UserDao;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

/**
 * 就当前的项目来说,
 * 换成@Component,@Configuration其实对于将UserService作为一个类放到IOC容器中被其管理都是可行的
 */
@Service
public class UserService {
    private final UserDao userDao;

    public UserService(UserDao userDao) {
        this.userDao = userDao;
    }

    public void insertService() {
//        因为id是AUTO_INCREMENT的,所以直接插入其他信息也就可以了
        userDao.insertUser("SnailClimb", 22, 3000.0);
        userDao.insertUser("Daisy", 19, 3000.0);
    }

    public void deleteService(int id) {
        userDao.deleteUser(id);
    }

    public void updateService() {
        userDao.updateUser("wangyihui", 23, 5000.0, 1);
    }

    public User selectUserByName(String name) {
        return userDao.findUserByName(name);
    }

    public List<User> selectAllUser() {
        return userDao.findAllUser();
    }

    /**
     * 模拟事务。 @Transactional注解并加上rollbackFor属性,事务在遇到非RuntimeException时也回滚
     * 如果转账中途出了意外 SnailClimb 和 Daisy 的钱都不会改变。
     */
    @Transactional(rollbackFor = Exception.class)
    public void changeMoney() {
        userDao.updateUser("SnailClimb", 22, 2000.0, 3);
        // 模拟转账过程中可能遇到的意外状况
        int temp = 1 / 0;
        userDao.updateUser("Daisy", 19, 4000.0, 4);
    }

}
