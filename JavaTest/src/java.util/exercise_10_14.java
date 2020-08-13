/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-15 21:37:12
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-20 09:20:36
 */

import java.util.GregorianCalendar;

class MyDate {

    //xxxx
    private int year;
    //0->11
    private int month;
    //356,355
    private int day;

    public MyDate() {
        GregorianCalendar mydate = new GregorianCalendar();
        //无参构造默认当前时间
        year = mydate.get(GregorianCalendar.YEAR);
        month = mydate.get(GregorianCalendar.MONTH);
        day = mydate.get(GregorianCalendar.DAY_OF_MONTH);
    }

    public MyDate(long elapseTime) {
        GregorianCalendar date = new GregorianCalendar();
        //设置为流逝时间
        date.setTimeInMillis(elapseTime);
        year = date.get(GregorianCalendar.YEAR);
        month = date.get(GregorianCalendar.MONTH);
        day = date.get(GregorianCalendar.DAY_OF_MONTH);
    }

    public MyDate(int year, int month, int day) {
        this.year = year;
        this.month = month;
        this.day = day;
    }

    public int getYear() {
        return year;
    }

    public int getMonth() {
        return month;
    }

    public int getDay() {
        return day;
    }

    public void setDate(long elapseTime) {
        GregorianCalendar date = new GregorianCalendar();
        date.setTimeInMillis(elapseTime);
        year = date.get(GregorianCalendar.YEAR);
        month = date.get(GregorianCalendar.MONTH);
        day = date.get(GregorianCalendar.DAY_OF_MONTH);
    }

}

public class exercise_10_14 {
    public static void main(String[] args) {
        MyDate Date = new MyDate();
        System.out.println(Date.getYear() + "年" + Date.getMonth() + "月" + Date.getDay() + "日");
        Date = new MyDate(34355555133101L);
        System.out.print(Date.getYear() + "年" + Date.getMonth() + "月" + Date.getDay() + "日");
    }
}