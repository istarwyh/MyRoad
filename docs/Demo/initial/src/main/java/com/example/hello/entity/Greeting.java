package com.example.hello.entity;

import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

/**
 * 相当于定义了一个要返回的数据结构
 */
@Getter
@Setter
@RequiredArgsConstructor
public class Greeting {
    private final long id;
    private final String content;

    @Override
    public String toString() {
        return "Greeting{" +
                "id=" + id +
                ", content='" + content + '\'' +
                '}';
    }
}