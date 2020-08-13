package com.itheima.mapper;

import com.itheima.domain.User;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
//这里是个接口!!在UserMapper.xml中映射
public interface UserMapper {
    public List<User> queryUserList();
}
