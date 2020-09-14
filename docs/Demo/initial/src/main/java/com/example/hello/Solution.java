package com.example.hello;

import java.text.SimpleDateFormat;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

/**
 * @author sx_wangyihui
 */
public class Solution {
    public static void main(String[] args) {
        char[] chars = {'a','b','c'};
        new Solution().printSub(chars);
    }
    public void printSub(char[] chars){

        /**
         * 以集合{a,b,c}为例，
         * 于是对所有的子集也可以采用二进制编码的方式：
         * 000 == null
         * 001 == a
         * 010 == b
         * ...
         * 111 == cba
         * 如果想要提取出这样二进制编码代表的子集信息（如打印出来），
         * 只需要元素编码与子集编码做“&”运算，来判断在每个被编码的子集中元素是否出现，
         * 001 & 011 == 001 == a[1<<i的i] == a[0] == a
         * 010 & 011 == 010 == a[1<<i的i] == a[1] == b
         * 100 & 011 == 000
         * 所以这个编码为011的子集则为 a，b
         *
         */
        int len = chars.length;
        int totalNum = (1 << len) ;
        for(int i=0;i<totalNum;i++){
            for( int j = 0; j<len; j++ ){
                int mask = 1 << j;
                if( (i & mask) != 0){
                    System.out.print( chars[j]);
                }
            }
            System.out.println();
        }
    }
    class SimpleLRUCache<K,V>{
        private final int MAX_CACHE_SIZE;
        private final float DEFAULT_LOAD_FACTORY = 0.75f;
        public LinkedHashMap<K,V> map;
        public SimpleLRUCache(int cacheSize){
            MAX_CACHE_SIZE = cacheSize;
            int capacity = (int)Math.ceil(MAX_CACHE_SIZE / DEFAULT_LOAD_FACTORY) +1;
            map = new LinkedHashMap<K,V>(capacity,DEFAULT_LOAD_FACTORY,true){
                private static  final long serialVersionUID = 1L;
                // 这个方法出来就是为了被重写的，LHM内部调用这个方法控制要不要或者何时删除队列中的数据
                @Override
                protected boolean removeEldestEntry(Map.Entry<K,V> eldest){
                    return size() > MAX_CACHE_SIZE;
                }
            };
        }

        public synchronized void put(K key,V value){
            map.put(key, value);
        }
        public synchronized void  remove(K key){
            map.remove(key);
        }
        public synchronized V get(K key){
            return map.get(key);
        }
        public synchronized Set<Map.Entry<K,V>> getAll(){
            return map.entrySet();
        }
        @Override
        public String toStirng(){
            StringBuilder sb = new StringBuilder();
            for(Map.Entry<K,V> entry : map.entrySet()){
                sb.append(String.format("%s: %s ",entry.getKey(),entry.getValue()));
            }
            return sb.toString();
        }
    }
 }
