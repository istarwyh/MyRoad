/*
 * @Descripttion:
 * @version: 1.0
 * @Author: Yihui
 * @Date: 2019-10-16 17:20:54
 * @LastEditors: Yihui
 * @LastEditTime: 2019-10-19 23:15:23
 */

import java.lang.String;
import java.lang.Integer;
import java.lang.StringBuilder;

class MyDate {
    //xxxx
    private int year;
    //0->11
    private int month;
    //356,355
    private int day;

    public MyDate() {
    }

    //设定时间的构造方法
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

    public int getDate() {
        //创建一个空的字符串构建器,sb的对象为StringBuilder类
        StringBuilder sb = new StringBuilder(String.valueOf(year));
        //append()方法相当于C++中加号
        sb.append(String.valueOf(month));
        sb.append(String.valueOf(day));
        //必须使用toString()方法才可以将构建器中的字符序列输出给新的String类对象
        String str = sb.toString();
<<<<<<<HEAD
                //不想改后面的int,强制转换一下
                =======
        //不想改后面的int,强制转换一下,这个方法与String.valueOf()对应
>>>>>>>4990d 40 c308033099e680e0460982eb6567484ad
        return Integer.parseInt(str);
    }
}

class Person {
    protected String name;
    protected String address;
    protected String tel;
    protected String email;

    public Person(String name, String address, String tel, String email) {
        this.name = name;
        this.address = address;
        this.tel = tel;
        this.email = email;
    }

    public String toString() {
        return getClass().getName() + "[name=" + name + ",address=" + address + ",tel=" + tel + ",email=" + email + "]";
    }

}

class Student extends Person {
    public static final String FRESHMAN = "大一";
    public static final String SOPHOMORE = "大二";
    public static final String JUNIOR = "大三";
    public static final String SENIOR = "大四";

    protected String status;

    public Student(String name, String address, String tel, String email, String status) {
        super(name, address, tel, email);
        this.status = status;
    }

    public String toString() {
        return super.toString() + "[status=" + status + "]";
    }
}

class Employee extends Person {
    private String office;
    private double salary;
    private long staffDate;

    public Employee(String name, String address, String tel, String email, String office, double salary, long staffDate) {
<<<<<<<HEAD
                =======
        //调用父类的构造方法
>>>>>>>4990d 40 c308033099e680e0460982eb6567484ad
        super(name, address, tel, email);
        this.office = office;
        this.salary = salary;
        this.staffDate = staffDate;
    }

    public String toString() {
<<<<<<<HEAD
                =======
        //调用父类的toString()方法
>>>>>>>4990d 40 c308033099e680e0460982eb6567484ad
        return super.toString() + "[office=" + office + ",salary=" + salary + ",staffDate=" + staffDate + "]";
    }

    public String getOffice() {
        return office;
    }

    public double getSalary() {
<<<<<<<HEAD
                =======
        
>>>>>>>4990d 40 c308033099e680e0460982eb6567484ad
        return salary;
    }

    public long getStaffDate() {
        return staffDate;
    }
}

class Faculty extends Employee {
    private String worktime;
    private String grade;

    //子类继承了父类之后还要继承好多东西啊
    public Faculty(String name, String address, String tel, String email, String office, double salary, long staffDate, String worktime, String grade) {
        super(name, address, tel, email, office, salary, staffDate);
        this.worktime = worktime;
        this.grade = grade;
    }

    public String toString() {
        return getClass().getName() + "[name=" + name + ",address=" + address + ",tel=" + tel + ",email=" + email + ",office=" + super.getOffice() + ",salary=" + super.getSalary() + ",staffDate=" + super.getStaffDate() + ",worktime=" + worktime + ",grade=" + grade + "]";
    }
}

class Staff extends Employee {
    private String title;

    public Staff(String name, String address, String tel, String email, String office, double salary, long staffDate, String title) {
        super(name, address, tel, email, office, salary, staffDate);
        this.title = title;
    }

    public String toString() {
        return super.toString() + "[title=" + title + "]";
    }
}

public class exercise_11_20 {
    public static void main(String[] args) {
        MyDate mydate = new MyDate(2019, 10, 10);
        Person person = new Person("王艺辉", "安徽", "15955311246", "yihui@mbin.cc");
        Student s = new Student("小王", "无为", "18823450987", "mbing_s@qq.com", Student.FRESHMAN);
        Employee e = new Employee("小艺", "石河子", "1502541345", "luchi@gmail.com", "CAMPUS", 20000, mydate.getDate());
        Faculty f = new Faculty("小辉", "重庆", "13648992354", "istarwyh@gmail.com", "CAMPUS", 20000, mydate.getDate(), "8h", "P12");
        Staff st = new Staff("Mbin", "ShenZhen", "19943458798", "mbin@qq.com", "CAMPUS", 20000, mydate.getDate(), "senior architect");
        System.out.println("" + person);
        System.out.println("" + s);
        System.out.println("" + e);
        System.out.println("" + f);
        System.out.println("" + st);
        //System.out.println(System.out);

    }
} 